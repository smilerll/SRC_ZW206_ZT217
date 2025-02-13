#ifdef ENABLE_LIBYUV

#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "libyuv.h"

#define SUBSAMPLE(v, a, s) (v < 0) ? (-((-v + a) >> s)) : ((v + a) >> s)

// Simplify the filtering based on scale factors.
enum FilterMode ScaleFilterReduce(int src_width,
                                  int src_height,
                                  int dst_width,
                                  int dst_height,
                                  enum FilterMode filtering) {
  if (src_width < 0) {
    src_width = -src_width;
  }
  if (src_height < 0) {
    src_height = -src_height;
  }
  if (filtering == kFilterBox) {
    // If scaling both axis to 0.5 or larger, switch from Box to Bilinear.
    if (dst_width * 2 >= src_width && dst_height * 2 >= src_height) {
      filtering = kFilterBilinear;
    }
  }
  if (filtering == kFilterBilinear) {
    if (src_height == 1) {
      filtering = kFilterLinear;
    }
    // TODO(fbarchard): Detect any odd scale factor and reduce to Linear.
    if (dst_height == src_height || dst_height * 3 == src_height) {
      filtering = kFilterLinear;
    }
    // TODO(fbarchard): Remove 1 pixel wide filter restriction, which is to
    // avoid reading 2 pixels horizontally that causes memory exception.
    if (src_width == 1) {
      filtering = kFilterNone;
    }
  }
  if (filtering == kFilterLinear) {
    if (src_width == 1) {
      filtering = kFilterNone;
    }
    // TODO(fbarchard): Detect any odd scale factor and reduce to None.
    if (dst_width == src_width || dst_width * 3 == src_width) {
      filtering = kFilterNone;
    }
  }
  return filtering;
}

void CopyRow_C(const uint8_t* src, uint8_t* dst, int count) {
  memcpy(dst, src, count);
}

void CopyPlane(const uint8_t* src_y,
               int src_stride_y,
               uint8_t* dst_y,
               int dst_stride_y,
               int width,
               int height) {
  int y;
  void (*CopyRow)(const uint8_t* src, uint8_t* dst, int width) = CopyRow_C;
  // Negative height means invert the image.
  if (height < 0) {
    height = -height;
    dst_y = dst_y + (height - 1) * dst_stride_y;
    dst_stride_y = -dst_stride_y;
  }
  // Coalesce rows.
  if (src_stride_y == width && dst_stride_y == width) {
    width *= height;
    height = 1;
    src_stride_y = dst_stride_y = 0;
  }
  // Nothing to do.
  if (src_y == dst_y && src_stride_y == dst_stride_y) {
    return;
  }

#if defined(HAS_COPYROW_SSE2)
  if (TestCpuFlag(kCpuHasSSE2)) {
    CopyRow = IS_ALIGNED(width, 32) ? CopyRow_SSE2 : CopyRow_Any_SSE2;
  }
#endif
#if defined(HAS_COPYROW_AVX)
  if (TestCpuFlag(kCpuHasAVX)) {
    CopyRow = IS_ALIGNED(width, 64) ? CopyRow_AVX : CopyRow_Any_AVX;
  }
#endif
#if defined(HAS_COPYROW_ERMS)
  if (TestCpuFlag(kCpuHasERMS)) {
    CopyRow = CopyRow_ERMS;
  }
#endif
#if defined(HAS_COPYROW_NEON)
  if (TestCpuFlag(kCpuHasNEON)) {
    CopyRow = IS_ALIGNED(width, 32) ? CopyRow_NEON : CopyRow_Any_NEON;
  }
#endif

  // Copy plane
  for (y = 0; y < height; ++y) {
    CopyRow(src_y, dst_y, width);
    src_y += src_stride_y;
    dst_y += dst_stride_y;
  }
}

int FixedDiv_C(int num, int div) {
  return (int)(((int64_t)(num) << 16) / div);
}
#define FixedDiv FixedDiv_C

static void HalfRow_C(const uint8_t* src_uv,
                      ptrdiff_t src_uv_stride,
                      uint8_t* dst_uv,
                      int width) {
  int x;
  for (x = 0; x < width; ++x) {
    dst_uv[x] = (src_uv[x] + src_uv[src_uv_stride + x] + 1) >> 1;
  }
}

void InterpolateRow_C(uint8_t* dst_ptr,
                      const uint8_t* src_ptr,
                      ptrdiff_t src_stride,
                      int width,
                      int source_y_fraction) {
  int y1_fraction = source_y_fraction;
  int y0_fraction = 256 - y1_fraction;
  const uint8_t* src_ptr1 = src_ptr + src_stride;
  int x;
  if (y1_fraction == 0) {
    memcpy(dst_ptr, src_ptr, width);
    return;
  }
  if (y1_fraction == 128) {
    HalfRow_C(src_ptr, src_stride, dst_ptr, width);
    return;
  }
  for (x = 0; x < width - 1; x += 2) {
    dst_ptr[0] =
        (src_ptr[0] * y0_fraction + src_ptr1[0] * y1_fraction + 128) >> 8;
    dst_ptr[1] =
        (src_ptr[1] * y0_fraction + src_ptr1[1] * y1_fraction + 128) >> 8;
    src_ptr += 2;
    src_ptr1 += 2;
    dst_ptr += 2;
  }
  if (width & 1) {
    dst_ptr[0] =
        (src_ptr[0] * y0_fraction + src_ptr1[0] * y1_fraction + 128) >> 8;
  }
}

void ScalePlaneVertical(int src_height,
                        int dst_width,
                        int dst_height,
                        int src_stride,
                        int dst_stride,
                        const uint8_t* src_argb,
                        uint8_t* dst_argb,
                        int x,
                        int y,
                        int dy,
                        int bpp,
                        enum FilterMode filtering) {
  // TODO(fbarchard): Allow higher bpp.
  int dst_width_bytes = dst_width * bpp;
  void (*InterpolateRow)(uint8_t * dst_argb, const uint8_t* src_argb,
                         ptrdiff_t src_stride, int dst_width,
                         int source_y_fraction) = InterpolateRow_C;
  const int max_y = (src_height > 1) ? ((src_height - 1) << 16) - 1 : 0;
  int j;
  assert(bpp >= 1 && bpp <= 4);
  assert(src_height != 0);
  assert(dst_width > 0);
  assert(dst_height > 0);
  src_argb += (x >> 16) * bpp;
#if defined(HAS_INTERPOLATEROW_SSSE3)
  if (TestCpuFlag(kCpuHasSSSE3)) {
    InterpolateRow = InterpolateRow_Any_SSSE3;
    if (IS_ALIGNED(dst_width_bytes, 16)) {
      InterpolateRow = InterpolateRow_SSSE3;
    }
  }
#endif
#if defined(HAS_INTERPOLATEROW_AVX2)
  if (TestCpuFlag(kCpuHasAVX2)) {
    InterpolateRow = InterpolateRow_Any_AVX2;
    if (IS_ALIGNED(dst_width_bytes, 32)) {
      InterpolateRow = InterpolateRow_AVX2;
    }
  }
#endif
#if defined(HAS_INTERPOLATEROW_NEON)
  if (TestCpuFlag(kCpuHasNEON)) {
    InterpolateRow = InterpolateRow_Any_NEON;
    if (IS_ALIGNED(dst_width_bytes, 16)) {
      InterpolateRow = InterpolateRow_NEON;
    }
  }
#endif
#if defined(HAS_INTERPOLATEROW_MMI)
  if (TestCpuFlag(kCpuHasMMI)) {
    InterpolateRow = InterpolateRow_Any_MMI;
    if (IS_ALIGNED(dst_width_bytes, 8)) {
      InterpolateRow = InterpolateRow_MMI;
    }
  }
#endif
#if defined(HAS_INTERPOLATEROW_MSA)
  if (TestCpuFlag(kCpuHasMSA)) {
    InterpolateRow = InterpolateRow_Any_MSA;
    if (IS_ALIGNED(dst_width_bytes, 32)) {
      InterpolateRow = InterpolateRow_MSA;
    }
  }
#endif
  for (j = 0; j < dst_height; ++j) {
    int yi;
    int yf;
    if (y > max_y) {
      y = max_y;
    }
    yi = y >> 16;
    yf = filtering ? ((y >> 8) & 255) : 0;
    InterpolateRow(dst_argb, src_argb + yi * src_stride, src_stride,
                   dst_width_bytes, yf);
    dst_argb += dst_stride;
    y += dy;
  }
}

static int Abs(int v) {
  return v >= 0 ? v : -v;
}

void ScaleRowDown34_C(const uint8_t* src_ptr,
                      ptrdiff_t src_stride,
                      uint8_t* dst,
                      int dst_width) {
  int x;
  (void)src_stride;
  assert((dst_width % 3 == 0) && (dst_width > 0));
  for (x = 0; x < dst_width; x += 3) {
    dst[0] = src_ptr[0];
    dst[1] = src_ptr[1];
    dst[2] = src_ptr[3];
    dst += 3;
    src_ptr += 4;
  }
}

// Filter rows 0 and 1 together, 3 : 1
void ScaleRowDown34_0_Box_C(const uint8_t* src_ptr,
                            ptrdiff_t src_stride,
                            uint8_t* d,
                            int dst_width) {
  const uint8_t* s = src_ptr;
  const uint8_t* t = src_ptr + src_stride;
  int x;
  assert((dst_width % 3 == 0) && (dst_width > 0));
  for (x = 0; x < dst_width; x += 3) {
    uint8_t a0 = (s[0] * 3 + s[1] * 1 + 2) >> 2;
    uint8_t a1 = (s[1] * 1 + s[2] * 1 + 1) >> 1;
    uint8_t a2 = (s[2] * 1 + s[3] * 3 + 2) >> 2;
    uint8_t b0 = (t[0] * 3 + t[1] * 1 + 2) >> 2;
    uint8_t b1 = (t[1] * 1 + t[2] * 1 + 1) >> 1;
    uint8_t b2 = (t[2] * 1 + t[3] * 3 + 2) >> 2;
    d[0] = (a0 * 3 + b0 + 2) >> 2;
    d[1] = (a1 * 3 + b1 + 2) >> 2;
    d[2] = (a2 * 3 + b2 + 2) >> 2;
    d += 3;
    s += 4;
    t += 4;
  }
}

// Filter rows 1 and 2 together, 1 : 1
void ScaleRowDown34_1_Box_C(const uint8_t* src_ptr,
                            ptrdiff_t src_stride,
                            uint8_t* d,
                            int dst_width) {
  const uint8_t* s = src_ptr;
  const uint8_t* t = src_ptr + src_stride;
  int x;
  assert((dst_width % 3 == 0) && (dst_width > 0));
  for (x = 0; x < dst_width; x += 3) {
    uint8_t a0 = (s[0] * 3 + s[1] * 1 + 2) >> 2;
    uint8_t a1 = (s[1] * 1 + s[2] * 1 + 1) >> 1;
    uint8_t a2 = (s[2] * 1 + s[3] * 3 + 2) >> 2;
    uint8_t b0 = (t[0] * 3 + t[1] * 1 + 2) >> 2;
    uint8_t b1 = (t[1] * 1 + t[2] * 1 + 1) >> 1;
    uint8_t b2 = (t[2] * 1 + t[3] * 3 + 2) >> 2;
    d[0] = (a0 + b0 + 1) >> 1;
    d[1] = (a1 + b1 + 1) >> 1;
    d[2] = (a2 + b2 + 1) >> 1;
    d += 3;
    s += 4;
    t += 4;
  }
}

// Scale plane down, 3/4
static void ScalePlaneDown34(int src_width,
                             int src_height,
                             int dst_width,
                             int dst_height,
                             int src_stride,
                             int dst_stride,
                             const uint8_t* src_ptr,
                             uint8_t* dst_ptr,
                             enum FilterMode filtering) {
  int y;
  void (*ScaleRowDown34_0)(const uint8_t* src_ptr, ptrdiff_t src_stride,
                           uint8_t* dst_ptr, int dst_width);
  void (*ScaleRowDown34_1)(const uint8_t* src_ptr, ptrdiff_t src_stride,
                           uint8_t* dst_ptr, int dst_width);
  const int filter_stride = (filtering == kFilterLinear) ? 0 : src_stride;
  (void)src_width;
  (void)src_height;
  assert(dst_width % 3 == 0);
  if (!filtering) {
    ScaleRowDown34_0 = ScaleRowDown34_C;
    ScaleRowDown34_1 = ScaleRowDown34_C;
  } else {
    ScaleRowDown34_0 = ScaleRowDown34_0_Box_C;
    ScaleRowDown34_1 = ScaleRowDown34_1_Box_C;
  }
#if defined(HAS_SCALEROWDOWN34_NEON)
  if (TestCpuFlag(kCpuHasNEON)) {
    if (!filtering) {
      ScaleRowDown34_0 = ScaleRowDown34_Any_NEON;
      ScaleRowDown34_1 = ScaleRowDown34_Any_NEON;
    } else {
      ScaleRowDown34_0 = ScaleRowDown34_0_Box_Any_NEON;
      ScaleRowDown34_1 = ScaleRowDown34_1_Box_Any_NEON;
    }
    if (dst_width % 24 == 0) {
      if (!filtering) {
        ScaleRowDown34_0 = ScaleRowDown34_NEON;
        ScaleRowDown34_1 = ScaleRowDown34_NEON;
      } else {
        ScaleRowDown34_0 = ScaleRowDown34_0_Box_NEON;
        ScaleRowDown34_1 = ScaleRowDown34_1_Box_NEON;
      }
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN34_MMI)
  if (TestCpuFlag(kCpuHasMMI)) {
    if (!filtering) {
      ScaleRowDown34_0 = ScaleRowDown34_Any_MMI;
      ScaleRowDown34_1 = ScaleRowDown34_Any_MMI;
      if (dst_width % 24 == 0) {
        ScaleRowDown34_0 = ScaleRowDown34_MMI;
        ScaleRowDown34_1 = ScaleRowDown34_MMI;
      }
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN34_MSA)
  if (TestCpuFlag(kCpuHasMSA)) {
    if (!filtering) {
      ScaleRowDown34_0 = ScaleRowDown34_Any_MSA;
      ScaleRowDown34_1 = ScaleRowDown34_Any_MSA;
    } else {
      ScaleRowDown34_0 = ScaleRowDown34_0_Box_Any_MSA;
      ScaleRowDown34_1 = ScaleRowDown34_1_Box_Any_MSA;
    }
    if (dst_width % 48 == 0) {
      if (!filtering) {
        ScaleRowDown34_0 = ScaleRowDown34_MSA;
        ScaleRowDown34_1 = ScaleRowDown34_MSA;
      } else {
        ScaleRowDown34_0 = ScaleRowDown34_0_Box_MSA;
        ScaleRowDown34_1 = ScaleRowDown34_1_Box_MSA;
      }
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN34_SSSE3)
  if (TestCpuFlag(kCpuHasSSSE3)) {
    if (!filtering) {
      ScaleRowDown34_0 = ScaleRowDown34_Any_SSSE3;
      ScaleRowDown34_1 = ScaleRowDown34_Any_SSSE3;
    } else {
      ScaleRowDown34_0 = ScaleRowDown34_0_Box_Any_SSSE3;
      ScaleRowDown34_1 = ScaleRowDown34_1_Box_Any_SSSE3;
    }
    if (dst_width % 24 == 0) {
      if (!filtering) {
        ScaleRowDown34_0 = ScaleRowDown34_SSSE3;
        ScaleRowDown34_1 = ScaleRowDown34_SSSE3;
      } else {
        ScaleRowDown34_0 = ScaleRowDown34_0_Box_SSSE3;
        ScaleRowDown34_1 = ScaleRowDown34_1_Box_SSSE3;
      }
    }
  }
#endif

  for (y = 0; y < dst_height - 2; y += 3) {
    ScaleRowDown34_0(src_ptr, filter_stride, dst_ptr, dst_width);
    src_ptr += src_stride;
    dst_ptr += dst_stride;
    ScaleRowDown34_1(src_ptr, filter_stride, dst_ptr, dst_width);
    src_ptr += src_stride;
    dst_ptr += dst_stride;
    ScaleRowDown34_0(src_ptr + src_stride, -filter_stride, dst_ptr, dst_width);
    src_ptr += src_stride * 2;
    dst_ptr += dst_stride;
  }

  // Remainder 1 or 2 rows with last row vertically unfiltered
  if ((dst_height % 3) == 2) {
    ScaleRowDown34_0(src_ptr, filter_stride, dst_ptr, dst_width);
    src_ptr += src_stride;
    dst_ptr += dst_stride;
    ScaleRowDown34_1(src_ptr, 0, dst_ptr, dst_width);
  } else if ((dst_height % 3) == 1) {
    ScaleRowDown34_0(src_ptr, 0, dst_ptr, dst_width);
  }
}

void ScaleRowDown2_C(const uint8_t* src_ptr,
                     ptrdiff_t src_stride,
                     uint8_t* dst,
                     int dst_width) {
  int x;
  (void)src_stride;
  for (x = 0; x < dst_width - 1; x += 2) {
    dst[0] = src_ptr[1];
    dst[1] = src_ptr[3];
    dst += 2;
    src_ptr += 4;
  }
  if (dst_width & 1) {
    dst[0] = src_ptr[1];
  }
}

void ScaleRowDown2Linear_C(const uint8_t* src_ptr,
                           ptrdiff_t src_stride,
                           uint8_t* dst,
                           int dst_width) {
  const uint8_t* s = src_ptr;
  int x;
  (void)src_stride;
  for (x = 0; x < dst_width - 1; x += 2) {
    dst[0] = (s[0] + s[1] + 1) >> 1;
    dst[1] = (s[2] + s[3] + 1) >> 1;
    dst += 2;
    s += 4;
  }
  if (dst_width & 1) {
    dst[0] = (s[0] + s[1] + 1) >> 1;
  }
}

void ScaleRowDown2Box_C(const uint8_t* src_ptr,
                        ptrdiff_t src_stride,
                        uint8_t* dst,
                        int dst_width) {
  const uint8_t* s = src_ptr;
  const uint8_t* t = src_ptr + src_stride;
  int x;
  for (x = 0; x < dst_width - 1; x += 2) {
    dst[0] = (s[0] + s[1] + t[0] + t[1] + 2) >> 2;
    dst[1] = (s[2] + s[3] + t[2] + t[3] + 2) >> 2;
    dst += 2;
    s += 4;
    t += 4;
  }
  if (dst_width & 1) {
    dst[0] = (s[0] + s[1] + t[0] + t[1] + 2) >> 2;
  }
}

static void ScalePlaneDown2(int src_width,
                            int src_height,
                            int dst_width,
                            int dst_height,
                            int src_stride,
                            int dst_stride,
                            const uint8_t* src_ptr,
                            uint8_t* dst_ptr,
                            enum FilterMode filtering) {
  int y;
  void (*ScaleRowDown2)(const uint8_t* src_ptr, ptrdiff_t src_stride,
                        uint8_t* dst_ptr, int dst_width) =
      filtering == kFilterNone
          ? ScaleRowDown2_C
          : (filtering == kFilterLinear ? ScaleRowDown2Linear_C
                                        : ScaleRowDown2Box_C);
  int row_stride = src_stride << 1;
  (void)src_width;
  (void)src_height;
  if (!filtering) {
    src_ptr += src_stride;  // Point to odd rows.
    src_stride = 0;
  }

#if defined(HAS_SCALEROWDOWN2_NEON)
  if (TestCpuFlag(kCpuHasNEON)) {
    ScaleRowDown2 =
        filtering == kFilterNone
            ? ScaleRowDown2_Any_NEON
            : (filtering == kFilterLinear ? ScaleRowDown2Linear_Any_NEON
                                          : ScaleRowDown2Box_Any_NEON);
    if (IS_ALIGNED(dst_width, 16)) {
      ScaleRowDown2 = filtering == kFilterNone ? ScaleRowDown2_NEON
                                               : (filtering == kFilterLinear
                                                      ? ScaleRowDown2Linear_NEON
                                                      : ScaleRowDown2Box_NEON);
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN2_SSSE3)
  if (TestCpuFlag(kCpuHasSSSE3)) {
    ScaleRowDown2 =
        filtering == kFilterNone
            ? ScaleRowDown2_Any_SSSE3
            : (filtering == kFilterLinear ? ScaleRowDown2Linear_Any_SSSE3
                                          : ScaleRowDown2Box_Any_SSSE3);
    if (IS_ALIGNED(dst_width, 16)) {
      ScaleRowDown2 =
          filtering == kFilterNone
              ? ScaleRowDown2_SSSE3
              : (filtering == kFilterLinear ? ScaleRowDown2Linear_SSSE3
                                            : ScaleRowDown2Box_SSSE3);
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN2_AVX2)
  if (TestCpuFlag(kCpuHasAVX2)) {
    ScaleRowDown2 =
        filtering == kFilterNone
            ? ScaleRowDown2_Any_AVX2
            : (filtering == kFilterLinear ? ScaleRowDown2Linear_Any_AVX2
                                          : ScaleRowDown2Box_Any_AVX2);
    if (IS_ALIGNED(dst_width, 32)) {
      ScaleRowDown2 = filtering == kFilterNone ? ScaleRowDown2_AVX2
                                               : (filtering == kFilterLinear
                                                      ? ScaleRowDown2Linear_AVX2
                                                      : ScaleRowDown2Box_AVX2);
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN2_MMI)
  if (TestCpuFlag(kCpuHasMMI)) {
    ScaleRowDown2 =
        filtering == kFilterNone
            ? ScaleRowDown2_Any_MMI
            : (filtering == kFilterLinear ? ScaleRowDown2Linear_Any_MMI
                                          : ScaleRowDown2Box_Any_MMI);
    if (IS_ALIGNED(dst_width, 8)) {
      ScaleRowDown2 = filtering == kFilterNone ? ScaleRowDown2_MMI
                                               : (filtering == kFilterLinear
                                                      ? ScaleRowDown2Linear_MMI
                                                      : ScaleRowDown2Box_MMI);
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN2_MSA)
  if (TestCpuFlag(kCpuHasMSA)) {
    ScaleRowDown2 =
        filtering == kFilterNone
            ? ScaleRowDown2_Any_MSA
            : (filtering == kFilterLinear ? ScaleRowDown2Linear_Any_MSA
                                          : ScaleRowDown2Box_Any_MSA);
    if (IS_ALIGNED(dst_width, 32)) {
      ScaleRowDown2 = filtering == kFilterNone ? ScaleRowDown2_MSA
                                               : (filtering == kFilterLinear
                                                      ? ScaleRowDown2Linear_MSA
                                                      : ScaleRowDown2Box_MSA);
    }
  }
#endif

  if (filtering == kFilterLinear) {
    src_stride = 0;
  }
  // TODO(fbarchard): Loop through source height to allow odd height.
  for (y = 0; y < dst_height; ++y) {
    ScaleRowDown2(src_ptr, src_stride, dst_ptr, dst_width);
    src_ptr += row_stride;
    dst_ptr += dst_stride;
  }
}

void ScaleRowDown38_C(const uint8_t* src_ptr,
                      ptrdiff_t src_stride,
                      uint8_t* dst,
                      int dst_width) {
  int x;
  (void)src_stride;
  assert(dst_width % 3 == 0);
  for (x = 0; x < dst_width; x += 3) {
    dst[0] = src_ptr[0];
    dst[1] = src_ptr[3];
    dst[2] = src_ptr[6];
    dst += 3;
    src_ptr += 8;
  }
}

// 8x3 -> 3x1
void ScaleRowDown38_3_Box_C(const uint8_t* src_ptr,
                            ptrdiff_t src_stride,
                            uint8_t* dst_ptr,
                            int dst_width) {
  intptr_t stride = src_stride;
  int i;
  assert((dst_width % 3 == 0) && (dst_width > 0));
  for (i = 0; i < dst_width; i += 3) {
    dst_ptr[0] =
        (src_ptr[0] + src_ptr[1] + src_ptr[2] + src_ptr[stride + 0] +
         src_ptr[stride + 1] + src_ptr[stride + 2] + src_ptr[stride * 2 + 0] +
         src_ptr[stride * 2 + 1] + src_ptr[stride * 2 + 2]) *
            (65536 / 9) >>
        16;
    dst_ptr[1] =
        (src_ptr[3] + src_ptr[4] + src_ptr[5] + src_ptr[stride + 3] +
         src_ptr[stride + 4] + src_ptr[stride + 5] + src_ptr[stride * 2 + 3] +
         src_ptr[stride * 2 + 4] + src_ptr[stride * 2 + 5]) *
            (65536 / 9) >>
        16;
    dst_ptr[2] =
        (src_ptr[6] + src_ptr[7] + src_ptr[stride + 6] + src_ptr[stride + 7] +
         src_ptr[stride * 2 + 6] + src_ptr[stride * 2 + 7]) *
            (65536 / 6) >>
        16;
    src_ptr += 8;
    dst_ptr += 3;
  }
}

// 8x2 -> 3x1
void ScaleRowDown38_2_Box_C(const uint8_t* src_ptr,
                            ptrdiff_t src_stride,
                            uint8_t* dst_ptr,
                            int dst_width) {
  intptr_t stride = src_stride;
  int i;
  assert((dst_width % 3 == 0) && (dst_width > 0));
  for (i = 0; i < dst_width; i += 3) {
    dst_ptr[0] = (src_ptr[0] + src_ptr[1] + src_ptr[2] + src_ptr[stride + 0] +
                  src_ptr[stride + 1] + src_ptr[stride + 2]) *
                     (65536 / 6) >>
                 16;
    dst_ptr[1] = (src_ptr[3] + src_ptr[4] + src_ptr[5] + src_ptr[stride + 3] +
                  src_ptr[stride + 4] + src_ptr[stride + 5]) *
                     (65536 / 6) >>
                 16;
    dst_ptr[2] =
        (src_ptr[6] + src_ptr[7] + src_ptr[stride + 6] + src_ptr[stride + 7]) *
            (65536 / 4) >>
        16;
    src_ptr += 8;
    dst_ptr += 3;
  }
}

static void ScalePlaneDown38(int src_width,
                             int src_height,
                             int dst_width,
                             int dst_height,
                             int src_stride,
                             int dst_stride,
                             const uint8_t* src_ptr,
                             uint8_t* dst_ptr,
                             enum FilterMode filtering) {
  int y;
  void (*ScaleRowDown38_3)(const uint8_t* src_ptr, ptrdiff_t src_stride,
                           uint8_t* dst_ptr, int dst_width);
  void (*ScaleRowDown38_2)(const uint8_t* src_ptr, ptrdiff_t src_stride,
                           uint8_t* dst_ptr, int dst_width);
  const int filter_stride = (filtering == kFilterLinear) ? 0 : src_stride;
  assert(dst_width % 3 == 0);
  (void)src_width;
  (void)src_height;
  if (!filtering) {
    ScaleRowDown38_3 = ScaleRowDown38_C;
    ScaleRowDown38_2 = ScaleRowDown38_C;
  } else {
    ScaleRowDown38_3 = ScaleRowDown38_3_Box_C;
    ScaleRowDown38_2 = ScaleRowDown38_2_Box_C;
  }

#if defined(HAS_SCALEROWDOWN38_NEON)
  if (TestCpuFlag(kCpuHasNEON)) {
    if (!filtering) {
      ScaleRowDown38_3 = ScaleRowDown38_Any_NEON;
      ScaleRowDown38_2 = ScaleRowDown38_Any_NEON;
    } else {
      ScaleRowDown38_3 = ScaleRowDown38_3_Box_Any_NEON;
      ScaleRowDown38_2 = ScaleRowDown38_2_Box_Any_NEON;
    }
    if (dst_width % 12 == 0) {
      if (!filtering) {
        ScaleRowDown38_3 = ScaleRowDown38_NEON;
        ScaleRowDown38_2 = ScaleRowDown38_NEON;
      } else {
        ScaleRowDown38_3 = ScaleRowDown38_3_Box_NEON;
        ScaleRowDown38_2 = ScaleRowDown38_2_Box_NEON;
      }
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN38_SSSE3)
  if (TestCpuFlag(kCpuHasSSSE3)) {
    if (!filtering) {
      ScaleRowDown38_3 = ScaleRowDown38_Any_SSSE3;
      ScaleRowDown38_2 = ScaleRowDown38_Any_SSSE3;
    } else {
      ScaleRowDown38_3 = ScaleRowDown38_3_Box_Any_SSSE3;
      ScaleRowDown38_2 = ScaleRowDown38_2_Box_Any_SSSE3;
    }
    if (dst_width % 12 == 0 && !filtering) {
      ScaleRowDown38_3 = ScaleRowDown38_SSSE3;
      ScaleRowDown38_2 = ScaleRowDown38_SSSE3;
    }
    if (dst_width % 6 == 0 && filtering) {
      ScaleRowDown38_3 = ScaleRowDown38_3_Box_SSSE3;
      ScaleRowDown38_2 = ScaleRowDown38_2_Box_SSSE3;
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN38_MSA)
  if (TestCpuFlag(kCpuHasMSA)) {
    if (!filtering) {
      ScaleRowDown38_3 = ScaleRowDown38_Any_MSA;
      ScaleRowDown38_2 = ScaleRowDown38_Any_MSA;
    } else {
      ScaleRowDown38_3 = ScaleRowDown38_3_Box_Any_MSA;
      ScaleRowDown38_2 = ScaleRowDown38_2_Box_Any_MSA;
    }
    if (dst_width % 12 == 0) {
      if (!filtering) {
        ScaleRowDown38_3 = ScaleRowDown38_MSA;
        ScaleRowDown38_2 = ScaleRowDown38_MSA;
      } else {
        ScaleRowDown38_3 = ScaleRowDown38_3_Box_MSA;
        ScaleRowDown38_2 = ScaleRowDown38_2_Box_MSA;
      }
    }
  }
#endif

  for (y = 0; y < dst_height - 2; y += 3) {
    ScaleRowDown38_3(src_ptr, filter_stride, dst_ptr, dst_width);
    src_ptr += src_stride * 3;
    dst_ptr += dst_stride;
    ScaleRowDown38_3(src_ptr, filter_stride, dst_ptr, dst_width);
    src_ptr += src_stride * 3;
    dst_ptr += dst_stride;
    ScaleRowDown38_2(src_ptr, filter_stride, dst_ptr, dst_width);
    src_ptr += src_stride * 2;
    dst_ptr += dst_stride;
  }

  // Remainder 1 or 2 rows with last row vertically unfiltered
  if ((dst_height % 3) == 2) {
    ScaleRowDown38_3(src_ptr, filter_stride, dst_ptr, dst_width);
    src_ptr += src_stride * 3;
    dst_ptr += dst_stride;
    ScaleRowDown38_3(src_ptr, 0, dst_ptr, dst_width);
  } else if ((dst_height % 3) == 1) {
    ScaleRowDown38_3(src_ptr, 0, dst_ptr, dst_width);
  }
}

#define align_buffer_64(var, size)                                           \
  uint8_t* var##_mem = (uint8_t*)(malloc((size) + 63));         /* NOLINT */ \
  uint8_t* var = (uint8_t*)(((intptr_t)(var##_mem) + 63) & ~63) /* NOLINT */

#define free_aligned_buffer_64(var) \
  free(var##_mem);                  \
  var = 0

#define MIN1(x) ((x) < 1 ? 1 : (x))

static uint32_t SumPixels(int iboxwidth, const uint16_t* src_ptr) {
  uint32_t sum = 0u;
  int x;
  assert(iboxwidth > 0);
  for (x = 0; x < iboxwidth; ++x) {
    sum += src_ptr[x];
  }
  return sum;
}

static void ScaleAddCols2_C(int dst_width,
                            int boxheight,
                            int x,
                            int dx,
                            const uint16_t* src_ptr,
                            uint8_t* dst_ptr) {
  int i;
  int scaletbl[2];
  int minboxwidth = dx >> 16;
  int boxwidth;
  scaletbl[0] = 65536 / (MIN1(minboxwidth) * boxheight);
  scaletbl[1] = 65536 / (MIN1(minboxwidth + 1) * boxheight);
  for (i = 0; i < dst_width; ++i) {
    int ix = x >> 16;
    x += dx;
    boxwidth = MIN1((x >> 16) - ix);
    *dst_ptr++ =
        SumPixels(boxwidth, src_ptr + ix) * scaletbl[boxwidth - minboxwidth] >>
        16;
  }
}

static void ScaleAddCols1_C(int dst_width,
                            int boxheight,
                            int x,
                            int dx,
                            const uint16_t* src_ptr,
                            uint8_t* dst_ptr) {
  int boxwidth = MIN1(dx >> 16);
  int scaleval = 65536 / (boxwidth * boxheight);
  int i;
  x >>= 16;
  for (i = 0; i < dst_width; ++i) {
    *dst_ptr++ = SumPixels(boxwidth, src_ptr + x) * scaleval >> 16;
    x += boxwidth;
  }
}

static void ScaleAddCols0_C(int dst_width,
                            int boxheight,
                            int x,
                            int dx,
                            const uint16_t* src_ptr,
                            uint8_t* dst_ptr) {
  int scaleval = 65536 / boxheight;
  int i;
  (void)dx;
  src_ptr += (x >> 16);
  for (i = 0; i < dst_width; ++i) {
    *dst_ptr++ = src_ptr[i] * scaleval >> 16;
  }
}

void ScaleAddRow_C(const uint8_t* src_ptr, uint16_t* dst_ptr, int src_width) {
  int x;
  assert(src_width > 0);
  for (x = 0; x < src_width - 1; x += 2) {
    dst_ptr[0] += src_ptr[0];
    dst_ptr[1] += src_ptr[1];
    src_ptr += 2;
    dst_ptr += 2;
  }
  if (src_width & 1) {
    dst_ptr[0] += src_ptr[0];
  }
}

#define CENTERSTART(dx, s) (dx < 0) ? -((-dx >> 1) + s) : ((dx >> 1) + s)
int FixedDiv1_C(int num, int div) {
  return (int)((((int64_t)(num) << 16) - 0x00010001) / (div - 1));
}
#define FixedDiv1 FixedDiv1_C

// Compute slope values for stepping.
void ScaleSlope(int src_width,
                int src_height,
                int dst_width,
                int dst_height,
                enum FilterMode filtering,
                int* x,
                int* y,
                int* dx,
                int* dy) {
  assert(x != NULL);
  assert(y != NULL);
  assert(dx != NULL);
  assert(dy != NULL);
  assert(src_width != 0);
  assert(src_height != 0);
  assert(dst_width > 0);
  assert(dst_height > 0);
  // Check for 1 pixel and avoid FixedDiv overflow.
  if (dst_width == 1 && src_width >= 32768) {
    dst_width = src_width;
  }
  if (dst_height == 1 && src_height >= 32768) {
    dst_height = src_height;
  }
  if (filtering == kFilterBox) {
    // Scale step for point sampling duplicates all pixels equally.
    *dx = FixedDiv(Abs(src_width), dst_width);
    *dy = FixedDiv(src_height, dst_height);
    *x = 0;
    *y = 0;
  } else if (filtering == kFilterBilinear) {
    // Scale step for bilinear sampling renders last pixel once for upsample.
    if (dst_width <= Abs(src_width)) {
      *dx = FixedDiv(Abs(src_width), dst_width);
      *x = CENTERSTART(*dx, -32768);  // Subtract 0.5 (32768) to center filter.
    } else if (dst_width > 1) {
      *dx = FixedDiv1(Abs(src_width), dst_width);
      *x = 0;
    }
    if (dst_height <= src_height) {
      *dy = FixedDiv(src_height, dst_height);
      *y = CENTERSTART(*dy, -32768);  // Subtract 0.5 (32768) to center filter.
    } else if (dst_height > 1) {
      *dy = FixedDiv1(src_height, dst_height);
      *y = 0;
    }
  } else if (filtering == kFilterLinear) {
    // Scale step for bilinear sampling renders last pixel once for upsample.
    if (dst_width <= Abs(src_width)) {
      *dx = FixedDiv(Abs(src_width), dst_width);
      *x = CENTERSTART(*dx, -32768);  // Subtract 0.5 (32768) to center filter.
    } else if (dst_width > 1) {
      *dx = FixedDiv1(Abs(src_width), dst_width);
      *x = 0;
    }
    *dy = FixedDiv(src_height, dst_height);
    *y = *dy >> 1;
  } else {
    // Scale step for point sampling duplicates all pixels equally.
    *dx = FixedDiv(Abs(src_width), dst_width);
    *dy = FixedDiv(src_height, dst_height);
    *x = CENTERSTART(*dx, 0);
    *y = CENTERSTART(*dy, 0);
  }
  // Negative src_width means horizontally mirror.
  if (src_width < 0) {
    *x += (dst_width - 1) * *dx;
    *dx = -*dx;
    // src_width = -src_width;   // Caller must do this.
  }
}
#undef CENTERSTART

static void ScalePlaneBox(int src_width,
                          int src_height,
                          int dst_width,
                          int dst_height,
                          int src_stride,
                          int dst_stride,
                          const uint8_t* src_ptr,
                          uint8_t* dst_ptr) {
  int j, k;
  // Initial source x/y coordinate and step values as 16.16 fixed point.
  int x = 0;
  int y = 0;
  int dx = 0;
  int dy = 0;
  const int max_y = (src_height << 16);
  ScaleSlope(src_width, src_height, dst_width, dst_height, kFilterBox, &x, &y,
             &dx, &dy);
  src_width = Abs(src_width);
  {
    // Allocate a row buffer of uint16_t.
    align_buffer_64(row16, src_width * 2);
    void (*ScaleAddCols)(int dst_width, int boxheight, int x, int dx,
                         const uint16_t* src_ptr, uint8_t* dst_ptr) =
        (dx & 0xffff) ? ScaleAddCols2_C
                      : ((dx != 0x10000) ? ScaleAddCols1_C : ScaleAddCols0_C);
    void (*ScaleAddRow)(const uint8_t* src_ptr, uint16_t* dst_ptr,
                        int src_width) = ScaleAddRow_C;
#if defined(HAS_SCALEADDROW_SSE2)
    if (TestCpuFlag(kCpuHasSSE2)) {
      ScaleAddRow = ScaleAddRow_Any_SSE2;
      if (IS_ALIGNED(src_width, 16)) {
        ScaleAddRow = ScaleAddRow_SSE2;
      }
    }
#endif
#if defined(HAS_SCALEADDROW_AVX2)
    if (TestCpuFlag(kCpuHasAVX2)) {
      ScaleAddRow = ScaleAddRow_Any_AVX2;
      if (IS_ALIGNED(src_width, 32)) {
        ScaleAddRow = ScaleAddRow_AVX2;
      }
    }
#endif
#if defined(HAS_SCALEADDROW_NEON)
    if (TestCpuFlag(kCpuHasNEON)) {
      ScaleAddRow = ScaleAddRow_Any_NEON;
      if (IS_ALIGNED(src_width, 16)) {
        ScaleAddRow = ScaleAddRow_NEON;
      }
    }
#endif
#if defined(HAS_SCALEADDROW_MMI)
    if (TestCpuFlag(kCpuHasMMI)) {
      ScaleAddRow = ScaleAddRow_Any_MMI;
      if (IS_ALIGNED(src_width, 8)) {
        ScaleAddRow = ScaleAddRow_MMI;
      }
    }
#endif
#if defined(HAS_SCALEADDROW_MSA)
    if (TestCpuFlag(kCpuHasMSA)) {
      ScaleAddRow = ScaleAddRow_Any_MSA;
      if (IS_ALIGNED(src_width, 16)) {
        ScaleAddRow = ScaleAddRow_MSA;
      }
    }
#endif

    for (j = 0; j < dst_height; ++j) {
      int boxheight;
      int iy = y >> 16;
      const uint8_t* src = src_ptr + iy * src_stride;
      y += dy;
      if (y > max_y) {
        y = max_y;
      }
      boxheight = MIN1((y >> 16) - iy);
      memset(row16, 0, src_width * 2);
      for (k = 0; k < boxheight; ++k) {
        ScaleAddRow(src, (uint16_t*)(row16), src_width);
        src += src_stride;
      }
      ScaleAddCols(dst_width, boxheight, x, dx, (uint16_t*)(row16), dst_ptr);
      dst_ptr += dst_stride;
    }
    free_aligned_buffer_64(row16);
  }
}

void ScaleRowDown4Box_C(const uint8_t* src_ptr,
                        ptrdiff_t src_stride,
                        uint8_t* dst,
                        int dst_width) {
  intptr_t stride = src_stride;
  int x;
  for (x = 0; x < dst_width - 1; x += 2) {
    dst[0] = (src_ptr[0] + src_ptr[1] + src_ptr[2] + src_ptr[3] +
              src_ptr[stride + 0] + src_ptr[stride + 1] + src_ptr[stride + 2] +
              src_ptr[stride + 3] + src_ptr[stride * 2 + 0] +
              src_ptr[stride * 2 + 1] + src_ptr[stride * 2 + 2] +
              src_ptr[stride * 2 + 3] + src_ptr[stride * 3 + 0] +
              src_ptr[stride * 3 + 1] + src_ptr[stride * 3 + 2] +
              src_ptr[stride * 3 + 3] + 8) >>
             4;
    dst[1] = (src_ptr[4] + src_ptr[5] + src_ptr[6] + src_ptr[7] +
              src_ptr[stride + 4] + src_ptr[stride + 5] + src_ptr[stride + 6] +
              src_ptr[stride + 7] + src_ptr[stride * 2 + 4] +
              src_ptr[stride * 2 + 5] + src_ptr[stride * 2 + 6] +
              src_ptr[stride * 2 + 7] + src_ptr[stride * 3 + 4] +
              src_ptr[stride * 3 + 5] + src_ptr[stride * 3 + 6] +
              src_ptr[stride * 3 + 7] + 8) >>
             4;
    dst += 2;
    src_ptr += 8;
  }
  if (dst_width & 1) {
    dst[0] = (src_ptr[0] + src_ptr[1] + src_ptr[2] + src_ptr[3] +
              src_ptr[stride + 0] + src_ptr[stride + 1] + src_ptr[stride + 2] +
              src_ptr[stride + 3] + src_ptr[stride * 2 + 0] +
              src_ptr[stride * 2 + 1] + src_ptr[stride * 2 + 2] +
              src_ptr[stride * 2 + 3] + src_ptr[stride * 3 + 0] +
              src_ptr[stride * 3 + 1] + src_ptr[stride * 3 + 2] +
              src_ptr[stride * 3 + 3] + 8) >>
              4;
  }
}

void ScaleRowDown4_C(const uint8_t* src_ptr,
                     ptrdiff_t src_stride,
                     uint8_t* dst,
                     int dst_width) {
  int x;
  (void)src_stride;
  for (x = 0; x < dst_width - 1; x += 2) {
    dst[0] = src_ptr[2];
    dst[1] = src_ptr[6];
    dst += 2;
    src_ptr += 8;
  }
  if (dst_width & 1) {
    dst[0] = src_ptr[2];
  }
}

static void ScalePlaneDown4(int src_width,
                            int src_height,
                            int dst_width,
                            int dst_height,
                            int src_stride,
                            int dst_stride,
                            const uint8_t* src_ptr,
                            uint8_t* dst_ptr,
                            enum FilterMode filtering) {
  int y;
  void (*ScaleRowDown4)(const uint8_t* src_ptr, ptrdiff_t src_stride,
                        uint8_t* dst_ptr, int dst_width) =
      filtering ? ScaleRowDown4Box_C : ScaleRowDown4_C;
  int row_stride = src_stride << 2;
  (void)src_width;
  (void)src_height;
  if (!filtering) {
    src_ptr += src_stride * 2;  // Point to row 2.
    src_stride = 0;
  }
#if defined(HAS_SCALEROWDOWN4_NEON)
  if (TestCpuFlag(kCpuHasNEON)) {
    ScaleRowDown4 =
        filtering ? ScaleRowDown4Box_Any_NEON : ScaleRowDown4_Any_NEON;
    if (IS_ALIGNED(dst_width, 8)) {
      ScaleRowDown4 = filtering ? ScaleRowDown4Box_NEON : ScaleRowDown4_NEON;
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN4_SSSE3)
  if (TestCpuFlag(kCpuHasSSSE3)) {
    ScaleRowDown4 =
        filtering ? ScaleRowDown4Box_Any_SSSE3 : ScaleRowDown4_Any_SSSE3;
    if (IS_ALIGNED(dst_width, 8)) {
      ScaleRowDown4 = filtering ? ScaleRowDown4Box_SSSE3 : ScaleRowDown4_SSSE3;
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN4_AVX2)
  if (TestCpuFlag(kCpuHasAVX2)) {
    ScaleRowDown4 =
        filtering ? ScaleRowDown4Box_Any_AVX2 : ScaleRowDown4_Any_AVX2;
    if (IS_ALIGNED(dst_width, 16)) {
      ScaleRowDown4 = filtering ? ScaleRowDown4Box_AVX2 : ScaleRowDown4_AVX2;
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN4_MMI)
  if (TestCpuFlag(kCpuHasMMI)) {
    ScaleRowDown4 =
        filtering ? ScaleRowDown4Box_Any_MMI : ScaleRowDown4_Any_MMI;
    if (IS_ALIGNED(dst_width, 8)) {
      ScaleRowDown4 = filtering ? ScaleRowDown4Box_MMI : ScaleRowDown4_MMI;
    }
  }
#endif
#if defined(HAS_SCALEROWDOWN4_MSA)
  if (TestCpuFlag(kCpuHasMSA)) {
    ScaleRowDown4 =
        filtering ? ScaleRowDown4Box_Any_MSA : ScaleRowDown4_Any_MSA;
    if (IS_ALIGNED(dst_width, 16)) {
      ScaleRowDown4 = filtering ? ScaleRowDown4Box_MSA : ScaleRowDown4_MSA;
    }
  }
#endif

  if (filtering == kFilterLinear) {
    src_stride = 0;
  }
  for (y = 0; y < dst_height; ++y) {
    ScaleRowDown4(src_ptr, src_stride, dst_ptr, dst_width);
    src_ptr += row_stride;
    dst_ptr += dst_stride;
  }
}

// (1-f)a + fb can be replaced with a + f(b-a)
//#if defined(__arm__) || defined(__aarch64__)
#define BLENDER(a, b, f) \
  (uint8_t)((int)(a) + ((((int)((f)) * ((int)(b) - (int)(a))) + 0x8000) >> 16))
//#else
// Intel uses 7 bit math with rounding.
//#define BLENDER(a, b, f) \
//  (uint8_t)((int)(a) + (((int)((f) >> 9) * ((int)(b) - (int)(a)) + 0x40) >> 7))
//#endif

void ScaleFilterCols_C(uint8_t* dst_ptr,
                       const uint8_t* src_ptr,
                       int dst_width,
                       int x,
                       int dx) {
  int j;
  for (j = 0; j < dst_width - 1; j += 2) {
    int xi = x >> 16;
    int a = src_ptr[xi];
    int b = src_ptr[xi + 1];
    dst_ptr[0] = BLENDER(a, b, x & 0xffff);
    x += dx;
    xi = x >> 16;
    a = src_ptr[xi];
    b = src_ptr[xi + 1];
    dst_ptr[1] = BLENDER(a, b, x & 0xffff);
    x += dx;
    dst_ptr += 2;
  }
  if (dst_width & 1) {
    int xi = x >> 16;
    int a = src_ptr[xi];
    int b = src_ptr[xi + 1];
    dst_ptr[0] = BLENDER(a, b, x & 0xffff);
  }
}

// Scales a single row of pixels using point sampling.
void ScaleCols_C(uint8_t* dst_ptr,
                 const uint8_t* src_ptr,
                 int dst_width,
                 int x,
                 int dx) {
  int j;
  for (j = 0; j < dst_width - 1; j += 2) {
    dst_ptr[0] = src_ptr[x >> 16];
    x += dx;
    dst_ptr[1] = src_ptr[x >> 16];
    x += dx;
    dst_ptr += 2;
  }
  if (dst_width & 1) {
    dst_ptr[0] = src_ptr[x >> 16];
  }
}

void ScaleFilterCols64_C(uint8_t* dst_ptr,
                         const uint8_t* src_ptr,
                         int dst_width,
                         int x32,
                         int dx) {
  int64_t x = (int64_t)(x32);
  int j;
  for (j = 0; j < dst_width - 1; j += 2) {
    int64_t xi = x >> 16;
    int a = src_ptr[xi];
    int b = src_ptr[xi + 1];
    dst_ptr[0] = BLENDER(a, b, x & 0xffff);
    x += dx;
    xi = x >> 16;
    a = src_ptr[xi];
    b = src_ptr[xi + 1];
    dst_ptr[1] = BLENDER(a, b, x & 0xffff);
    x += dx;
    dst_ptr += 2;
  }
  if (dst_width & 1) {
    int64_t xi = x >> 16;
    int a = src_ptr[xi];
    int b = src_ptr[xi + 1];
    dst_ptr[0] = BLENDER(a, b, x & 0xffff);
  }
}

// Scales a single row of pixels up by 2x using point sampling.
void ScaleColsUp2_C(uint8_t* dst_ptr,
                    const uint8_t* src_ptr,
                    int dst_width,
                    int x,
                    int dx) {
  int j;
  (void)x;
  (void)dx;
  for (j = 0; j < dst_width - 1; j += 2) {
    dst_ptr[1] = dst_ptr[0] = src_ptr[0];
    src_ptr += 1;
    dst_ptr += 2;
  }
  if (dst_width & 1) {
    dst_ptr[0] = src_ptr[0];
  }
}

// Scale up down with bilinear interpolation.
void ScalePlaneBilinearUp(int src_width,
                          int src_height,
                          int dst_width,
                          int dst_height,
                          int src_stride,
                          int dst_stride,
                          const uint8_t* src_ptr,
                          uint8_t* dst_ptr,
                          enum FilterMode filtering) {
  int j;
  // Initial source x/y coordinate and step values as 16.16 fixed point.
  int x = 0;
  int y = 0;
  int dx = 0;
  int dy = 0;
  const int max_y = (src_height - 1) << 16;
  void (*InterpolateRow)(uint8_t * dst_ptr, const uint8_t* src_ptr,
                         ptrdiff_t src_stride, int dst_width,
                         int source_y_fraction) = InterpolateRow_C;
  void (*ScaleFilterCols)(uint8_t * dst_ptr, const uint8_t* src_ptr,
                          int dst_width, int x, int dx) =
      filtering ? ScaleFilterCols_C : ScaleCols_C;
  ScaleSlope(src_width, src_height, dst_width, dst_height, filtering, &x, &y,
             &dx, &dy);
  src_width = Abs(src_width);

#if defined(HAS_INTERPOLATEROW_SSSE3)
  if (TestCpuFlag(kCpuHasSSSE3)) {
    InterpolateRow = InterpolateRow_Any_SSSE3;
    if (IS_ALIGNED(dst_width, 16)) {
      InterpolateRow = InterpolateRow_SSSE3;
    }
  }
#endif
#if defined(HAS_INTERPOLATEROW_AVX2)
  if (TestCpuFlag(kCpuHasAVX2)) {
    InterpolateRow = InterpolateRow_Any_AVX2;
    if (IS_ALIGNED(dst_width, 32)) {
      InterpolateRow = InterpolateRow_AVX2;
    }
  }
#endif
#if defined(HAS_INTERPOLATEROW_NEON)
  if (TestCpuFlag(kCpuHasNEON)) {
    InterpolateRow = InterpolateRow_Any_NEON;
    if (IS_ALIGNED(dst_width, 16)) {
      InterpolateRow = InterpolateRow_NEON;
    }
  }
#endif

  if (filtering && src_width >= 32768) {
    ScaleFilterCols = ScaleFilterCols64_C;
  }
#if defined(HAS_SCALEFILTERCOLS_SSSE3)
  if (filtering && TestCpuFlag(kCpuHasSSSE3) && src_width < 32768) {
    ScaleFilterCols = ScaleFilterCols_SSSE3;
  }
#endif
#if defined(HAS_SCALEFILTERCOLS_NEON)
  if (filtering && TestCpuFlag(kCpuHasNEON) && src_width < 32768) {
    ScaleFilterCols = ScaleFilterCols_Any_NEON;
    if (IS_ALIGNED(dst_width, 8)) {
      ScaleFilterCols = ScaleFilterCols_NEON;
    }
  }
#endif
#if defined(HAS_SCALEFILTERCOLS_MSA)
  if (filtering && TestCpuFlag(kCpuHasMSA) && src_width < 32768) {
    ScaleFilterCols = ScaleFilterCols_Any_MSA;
    if (IS_ALIGNED(dst_width, 16)) {
      ScaleFilterCols = ScaleFilterCols_MSA;
    }
  }
#endif
  if (!filtering && src_width * 2 == dst_width && x < 0x8000) {
    ScaleFilterCols = ScaleColsUp2_C;
#if defined(HAS_SCALECOLS_SSE2)
    if (TestCpuFlag(kCpuHasSSE2) && IS_ALIGNED(dst_width, 8)) {
      ScaleFilterCols = ScaleColsUp2_SSE2;
    }
#endif
#if defined(HAS_SCALECOLS_MMI)
    if (TestCpuFlag(kCpuHasMMI) && IS_ALIGNED(dst_width, 8)) {
      ScaleFilterCols = ScaleColsUp2_MMI;
    }
#endif
  }

  if (y > max_y) {
    y = max_y;
  }
  {
    int yi = y >> 16;
    const uint8_t* src = src_ptr + yi * src_stride;

    // Allocate 2 row buffers.
    const int kRowSize = (dst_width + 31) & ~31;
    align_buffer_64(row, kRowSize * 2);

    uint8_t* rowptr = row;
    int rowstride = kRowSize;
    int lasty = yi;

    ScaleFilterCols(rowptr, src, dst_width, x, dx);
    if (src_height > 1) {
      src += src_stride;
    }
    ScaleFilterCols(rowptr + rowstride, src, dst_width, x, dx);
    src += src_stride;

    for (j = 0; j < dst_height; ++j) {
      yi = y >> 16;
      if (yi != lasty) {
        if (y > max_y) {
          y = max_y;
          yi = y >> 16;
          src = src_ptr + yi * src_stride;
        }
        if (yi != lasty) {
          ScaleFilterCols(rowptr, src, dst_width, x, dx);
          rowptr += rowstride;
          rowstride = -rowstride;
          lasty = yi;
          src += src_stride;
        }
      }
      if (filtering == kFilterLinear) {
        InterpolateRow(dst_ptr, rowptr, 0, dst_width, 0);
      } else {
        int yf = (y >> 8) & 255;
        InterpolateRow(dst_ptr, rowptr, rowstride, dst_width, yf);
      }
      dst_ptr += dst_stride;
      y += dy;
    }
    free_aligned_buffer_64(row);
  }
}

// Scale plane down with bilinear interpolation.
void ScalePlaneBilinearDown(int src_width,
                            int src_height,
                            int dst_width,
                            int dst_height,
                            int src_stride,
                            int dst_stride,
                            const uint8_t* src_ptr,
                            uint8_t* dst_ptr,
                            enum FilterMode filtering) {
  // Initial source x/y coordinate and step values as 16.16 fixed point.
  int x = 0;
  int y = 0;
  int dx = 0;
  int dy = 0;
  // TODO(fbarchard): Consider not allocating row buffer for kFilterLinear.
  // Allocate a row buffer.
  align_buffer_64(row, src_width);

  const int max_y = (src_height - 1) << 16;
  int j;
  void (*ScaleFilterCols)(uint8_t * dst_ptr, const uint8_t* src_ptr,
                          int dst_width, int x, int dx) =
      (src_width >= 32768) ? ScaleFilterCols64_C : ScaleFilterCols_C;
  void (*InterpolateRow)(uint8_t * dst_ptr, const uint8_t* src_ptr,
                         ptrdiff_t src_stride, int dst_width,
                         int source_y_fraction) = InterpolateRow_C;
  ScaleSlope(src_width, src_height, dst_width, dst_height, filtering, &x, &y,
             &dx, &dy);
  src_width = Abs(src_width);

#if defined(HAS_INTERPOLATEROW_SSSE3)
  if (TestCpuFlag(kCpuHasSSSE3)) {
    InterpolateRow = InterpolateRow_Any_SSSE3;
    if (IS_ALIGNED(src_width, 16)) {
      InterpolateRow = InterpolateRow_SSSE3;
    }
  }
#endif
#if defined(HAS_INTERPOLATEROW_AVX2)
  if (TestCpuFlag(kCpuHasAVX2)) {
    InterpolateRow = InterpolateRow_Any_AVX2;
    if (IS_ALIGNED(src_width, 32)) {
      InterpolateRow = InterpolateRow_AVX2;
    }
  }
#endif
#if defined(HAS_INTERPOLATEROW_NEON)
  if (TestCpuFlag(kCpuHasNEON)) {
    InterpolateRow = InterpolateRow_Any_NEON;
    if (IS_ALIGNED(src_width, 16)) {
      InterpolateRow = InterpolateRow_NEON;
    }
  }
#endif
#if defined(HAS_INTERPOLATEROW_MMI)
  if (TestCpuFlag(kCpuHasMMI)) {
    InterpolateRow = InterpolateRow_Any_MMI;
    if (IS_ALIGNED(src_width, 16)) {
      InterpolateRow = InterpolateRow_MMI;
    }
  }
#endif
#if defined(HAS_INTERPOLATEROW_MSA)
  if (TestCpuFlag(kCpuHasMSA)) {
    InterpolateRow = InterpolateRow_Any_MSA;
    if (IS_ALIGNED(src_width, 32)) {
      InterpolateRow = InterpolateRow_MSA;
    }
  }
#endif

#if defined(HAS_SCALEFILTERCOLS_SSSE3)
  if (TestCpuFlag(kCpuHasSSSE3) && src_width < 32768) {
    ScaleFilterCols = ScaleFilterCols_SSSE3;
  }
#endif
#if defined(HAS_SCALEFILTERCOLS_NEON)
  if (TestCpuFlag(kCpuHasNEON) && src_width < 32768) {
    ScaleFilterCols = ScaleFilterCols_Any_NEON;
    if (IS_ALIGNED(dst_width, 8)) {
      ScaleFilterCols = ScaleFilterCols_NEON;
    }
  }
#endif
#if defined(HAS_SCALEFILTERCOLS_MSA)
  if (TestCpuFlag(kCpuHasMSA) && src_width < 32768) {
    ScaleFilterCols = ScaleFilterCols_Any_MSA;
    if (IS_ALIGNED(dst_width, 16)) {
      ScaleFilterCols = ScaleFilterCols_MSA;
    }
  }
#endif
  if (y > max_y) {
    y = max_y;
  }

  for (j = 0; j < dst_height; ++j) {
    int yi = y >> 16;
    const uint8_t* src = src_ptr + yi * src_stride;
    if (filtering == kFilterLinear) {
      ScaleFilterCols(dst_ptr, src, dst_width, x, dx);
    } else {
      int yf = (y >> 8) & 255;
      InterpolateRow(row, src, src_stride, src_width, yf);
      ScaleFilterCols(dst_ptr, row, dst_width, x, dx);
    }
    dst_ptr += dst_stride;
    y += dy;
    if (y > max_y) {
      y = max_y;
    }
  }
  free_aligned_buffer_64(row);
}

// Scale Plane to/from any dimensions, without interpolation.
// Fixed point math is used for performance: The upper 16 bits
// of x and dx is the integer part of the source position and
// the lower 16 bits are the fixed decimal part.

static void ScalePlaneSimple(int src_width,
                             int src_height,
                             int dst_width,
                             int dst_height,
                             int src_stride,
                             int dst_stride,
                             const uint8_t* src_ptr,
                             uint8_t* dst_ptr) {
  int i;
  void (*ScaleCols)(uint8_t * dst_ptr, const uint8_t* src_ptr, int dst_width,
                    int x, int dx) = ScaleCols_C;
  // Initial source x/y coordinate and step values as 16.16 fixed point.
  int x = 0;
  int y = 0;
  int dx = 0;
  int dy = 0;
  ScaleSlope(src_width, src_height, dst_width, dst_height, kFilterNone, &x, &y,
             &dx, &dy);
  src_width = Abs(src_width);

  if (src_width * 2 == dst_width && x < 0x8000) {
    ScaleCols = ScaleColsUp2_C;
#if defined(HAS_SCALECOLS_SSE2)
    if (TestCpuFlag(kCpuHasSSE2) && IS_ALIGNED(dst_width, 8)) {
      ScaleCols = ScaleColsUp2_SSE2;
    }
#endif
#if defined(HAS_SCALECOLS_MMI)
    if (TestCpuFlag(kCpuHasMMI) && IS_ALIGNED(dst_width, 8)) {
      ScaleCols = ScaleColsUp2_MMI;
    }
#endif
  }

  for (i = 0; i < dst_height; ++i) {
    ScaleCols(dst_ptr, src_ptr + (y >> 16) * src_stride, dst_width, x, dx);
    dst_ptr += dst_stride;
    y += dy;
  }
}

// Scale a plane.
// This function dispatches to a specialized scaler based on scale factor.
void ScalePlane(const uint8_t* src,
                int src_stride,
                int src_width,
                int src_height,
                uint8_t* dst,
                int dst_stride,
                int dst_width,
                int dst_height,
                enum FilterMode filtering) {
  // Simplify filtering when possible.
  filtering = ScaleFilterReduce(src_width, src_height, dst_width, dst_height,
                                filtering);

  // Negative height means invert the image.
  if (src_height < 0) {
    src_height = -src_height;
    src = src + (src_height - 1) * src_stride;
    src_stride = -src_stride;
  }

  // Use specialized scales to improve performance for common resolutions.
  // For example, all the 1/2 scalings will use ScalePlaneDown2()
  if (dst_width == src_width && dst_height == src_height) {
    // Straight copy.
    CopyPlane(src, src_stride, dst, dst_stride, dst_width, dst_height);
    return;
  }
  if (dst_width == src_width && filtering != kFilterBox) {
    int dy = FixedDiv_C(src_height, dst_height);
    // Arbitrary scale vertically, but unscaled horizontally.
    ScalePlaneVertical(src_height, dst_width, dst_height, src_stride,
                       dst_stride, src, dst, 0, 0, dy, 1, filtering);
    return;
  }
  if (dst_width <= Abs(src_width) && dst_height <= src_height) {
    // Scale down.
    if (4 * dst_width == 3 * src_width && 4 * dst_height == 3 * src_height) {
      // optimized, 3/4
      ScalePlaneDown34(src_width, src_height, dst_width, dst_height, src_stride,
                       dst_stride, src, dst, filtering);
      return;
    }
    if (2 * dst_width == src_width && 2 * dst_height == src_height) {
      // optimized, 1/2
      ScalePlaneDown2(src_width, src_height, dst_width, dst_height, src_stride,
                      dst_stride, src, dst, filtering);
      return;
    }
    // 3/8 rounded up for odd sized chroma height.
    if (8 * dst_width == 3 * src_width && 8 * dst_height == 3 * src_height) {
      // optimized, 3/8
      ScalePlaneDown38(src_width, src_height, dst_width, dst_height, src_stride,
                       dst_stride, src, dst, filtering);
      return;
    }
    if (4 * dst_width == src_width && 4 * dst_height == src_height &&
        (filtering == kFilterBox || filtering == kFilterNone)) {
      // optimized, 1/4
      ScalePlaneDown4(src_width, src_height, dst_width, dst_height, src_stride,
                      dst_stride, src, dst, filtering);
      return;
    }
  }
  if (filtering == kFilterBox && dst_height * 2 < src_height) {
    ScalePlaneBox(src_width, src_height, dst_width, dst_height, src_stride,
                  dst_stride, src, dst);
    return;
  }
  if (filtering && dst_height > src_height) {
    ScalePlaneBilinearUp(src_width, src_height, dst_width, dst_height,
                         src_stride, dst_stride, src, dst, filtering);
    return;
  }
  if (filtering) {
    ScalePlaneBilinearDown(src_width, src_height, dst_width, dst_height,
                           src_stride, dst_stride, src, dst, filtering);
    return;
  }
  ScalePlaneSimple(src_width, src_height, dst_width, dst_height, src_stride,
                   dst_stride, src, dst);
}

int I420Scale(const uint8_t* src_y,
              int src_stride_y,
              const uint8_t* src_u,
              int src_stride_u,
              const uint8_t* src_v,
              int src_stride_v,
              int src_width,
              int src_height,
              uint8_t* dst_y,
              int dst_stride_y,
              uint8_t* dst_u,
              int dst_stride_u,
              uint8_t* dst_v,
              int dst_stride_v,
              int dst_width,
              int dst_height,
              enum FilterMode filtering) {
  int src_halfwidth = SUBSAMPLE(src_width, 1, 1);
  int src_halfheight = SUBSAMPLE(src_height, 1, 1);
  int dst_halfwidth = SUBSAMPLE(dst_width, 1, 1);
  int dst_halfheight = SUBSAMPLE(dst_height, 1, 1);
  if (!src_y || !src_u || !src_v || src_width == 0 || src_height == 0 ||
      src_width > 32768 || src_height > 32768 || !dst_y || !dst_u || !dst_v ||
      dst_width <= 0 || dst_height <= 0) {
    return -1;
  }

  ScalePlane(src_y, src_stride_y, src_width, src_height, dst_y, dst_stride_y,
             dst_width, dst_height, filtering);
  ScalePlane(src_u, src_stride_u, src_halfwidth, src_halfheight, dst_u,
             dst_stride_u, dst_halfwidth, dst_halfheight, filtering);
  ScalePlane(src_v, src_stride_v, src_halfwidth, src_halfheight, dst_v,
             dst_stride_v, dst_halfwidth, dst_halfheight, filtering);
  return 0;
}
#endif