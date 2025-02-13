

#ifndef RV_DEPACK_H
#define RV_DEPACK_H

#include "helix_types.h"
#include "helix_result.h"
#include "rm_error.h"
#include "rm_memory.h"
#include "rm_stream.h"
#include "rm_packet.h"
#include "rv_format_info.h"

#ifdef __cplusplus
extern "C" {
#endif  /* #ifdef __cplusplus */

/* Callback functions */
typedef HX_RESULT (*rv_frame_avail_func_ptr) (void* pAvail, UINT32 ulSubStreamNum, rv_frame* frame);

/*
 * rv_depack definition. Opaque to user.
 */
typedef void rv_depack;

/*
 * rv_depack_create
 */
/*rv_depack* rv_depack_create(void*                   pAvail,
                            rv_frame_avail_func_ptr fpAvail,
                            void*                   pUserError,
                            rm_error_func_ptr       fpError);*/

/*
 * rv_depack_create2
 *
 * This is the same as rv_depack_create(), but it allows
 * the user to use custom memory allocation and free functions.
 */
rv_depack* rv_depack_create2(void*                   pAvail,
                             rv_frame_avail_func_ptr fpAvail,
                             void*                   pUserError,
                             rm_error_func_ptr       fpError,
                             void*                   pUserMem,
                             rm_malloc_func_ptr      fpMalloc,
                             rm_free_func_ptr        fpFree);

/*
 * rv_depack_init
 *
 * This is initialized with a RealVideo rm_stream_header struct.
 */
HX_RESULT rv_depack_init(rv_depack* pDepack, rm_stream_header* header);

/*
 * rv_depack_get_num_substreams
 *
 * This accessor function tells how many video substreams there are
 * in this video stream. For single-rate video streams, this will be 1. For
 * SureStream video streams, this could be greater than 1.
 */
UINT32 rv_depack_get_num_substreams(rv_depack* pDepack);

/*
 * rv_depack_get_codec_4cc
 *
 * This accessor function returns the 4cc of the codec. This 4cc
 * will be used to determine which codec to use. RealVideo always
 * uses the same codec for all substreams.
 */
UINT32 rv_depack_get_codec_4cc(rv_depack* pDepack);

/*
 * rv_depack_get_codec_init_info
 *
 * This function fills in the structure which is used to initialize the codec.
 */
HX_RESULT rv_depack_get_codec_init_info(rv_depack* pDepack, rv_format_info** ppInfo);

/*
 * rv_depack_destroy_codec_init_info
 *
 * This function frees the memory associated with the rv_format_info object
 * created by rv_depack_get_codec_init_info().
 */
void rv_depack_destroy_codec_init_info(rv_depack* pDepack, rv_format_info** ppInfo);

/*
 * rv_depack_add_packet
 *
 * Put a video packet into the depacketizer. When enough data is
 * present to depacketize, then the user will be called back
 * on the rv_frame_avail_func_ptr set in rv_depack_create().
 */
HX_RESULT rv_depack_add_packet(rv_depack* pDepack, rm_packet* packet);

/*
 * rv_depack_destroy_frame
 *
 * This cleans up a frame that was returned via
 * the rv_frame_avail_func_ptr.
 */
void rv_depack_destroy_frame(rv_depack* pDepack, rv_frame** ppFrame);

/*
 * rv_depack_seek
 *
 * The user calls this function if the stream is seeked. It
 * should be called before passing any post-seek packets. After calling
 * rv_depack_seek(), no more pre-seek packets should be
 * passed into rv_depack_add_packet().
 */
HX_RESULT rv_depack_seek(rv_depack* pDepack, UINT32 ulTime);

/*
 * rv_depack_destroy
 *
 * This cleans up all memory allocated by the rv_depack_* calls
 */
void rv_depack_destroy(rv_depack** ppDepack);


#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus */

#endif /* #ifndef RV_DEPACK_H */
