#include "mmidm_app.h"
#include "mmi_id.h"
#include "mmidm_text.h"
//#include "mmi_appmsg.h"
#include "mmidm_internal.h"
#include "mmidm_parsexml.h" 
#ifdef WIN32
  #include "std_header.h"
 #endif
 #define _DM_MD5_BASE64_C_


#define MAX_COLUMNS 45

#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

static unsigned char PADDING[64] = {
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


typedef unsigned long int UINT4;

typedef struct {
  unsigned long int state[4];                                   /* state (ABCD) */
  unsigned long int count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;/*lint !e770*/

  /*  全局变量声明*/
#define HASHLEN 16
typedef char HASH[HASHLEN];
#define HASHHEXLEN 32
typedef char HASHHEX[HASHHEXLEN+1];

#define F(x,y,z)	(z ^ (x & (y ^ z)))
#define G(x,y,z)	(y ^ (z & (y ^ x)))
#define H(x,y,z)	(x^y^z)
#define I(x,y,z)	(y^(x|(~z)))
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
  
  
 #define xppMemset(s, c, n)               memset(s, c, n)
 #define xppMemcpy(target, source, cnt)   memcpy(target, source, cnt)
 #define xppMemmove(target, source, cnt)  memmove(target, source, cnt)
 #define xppMemcmp(target, source, cnt)   memcmp(target, source, cnt)
 #define xppMalloc(size)                  SCI_ALLOC(size)
 #define xppFree(ptr)                     SCI_FREE(ptr)
/****************************************************************/
// The routine MD5Init initializes the message-digest context
 //  mdContext. All fields are set to zero.
/****************************************************************/
LOCAL void DMMD5Init(MD5_CTX *context);
/****************************************************************/
// Decodes input (unsigned char) into output (UINT4). Assumes len is
//  a multiple of 4.
/****************************************************************/
LOCAL void DM_Decode(unsigned long int *output,
                                unsigned char *input,
                                unsigned int len
                               );
/****************************************************************/
// MD5 basic UPDATE. UPDATE state based on block.
/****************************************************************/
LOCAL void DMMD5Update(MD5_CTX *context,                                        /* context */
                                      char *input,                               /* input block */
                                      unsigned int inputLen                     /* length of input block */
                                      );
/****************************************************************/
// MD5 basic FINAL. UPDATE state based on block.
/****************************************************************/
LOCAL void DMMD5Final( char digest[16],                         /* message digest */
                                   MD5_CTX *context                                    /* context */
                                 );
/*****************************************************************/
// Function: pre-compute the size of the base64 encoded document */
/****************************************************************/
LOCAL unsigned long mmidm_base64Encode(unsigned char* pbTarget,       // o: target buffer
                     unsigned long cbTargetSize,   // i: target buffer size
                     unsigned char* pbData,         // i: Data buffer
                     unsigned long *pcbDataLength, // i/o: Data buffer size
                     unsigned long *pcbOffset,     // i/o: absolute # of bytes encoded so far
                     unsigned int bLast,          // i: 0=first block, 1= next block, 2=last block
                     unsigned char *pbSaveBytes);  // i/o: last incomplete data block;


static void DM_MD5_memset(unsigned char* output,
                                           int value,
                                           unsigned int len
                                           )
{
  unsigned int i;

  for (i = 0; i < len; i++)
 ((char *)output)[i] = (char)value;
}

static void DM_MD5_memcpy(unsigned char* output,
                                            unsigned char* input,
                                            unsigned int len
                                            )
{
  unsigned int i;

  for (i = 0; i < len; i++)
    output[i] = input[i];
}
/****************************************************************/
// The routine MD5Init initializes the message-digest context
//   mdContext. All fields are set to zero.
/****************************************************************/
LOCAL void DMMD5Init(MD5_CTX *context)
{
  context->count[0] = context->count[1] = 0;
  /* Load magic initialization constants.
*/
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;
}
/****************************************************************/
// Encodes input (UINT4) into output (unsigned char). Assumes len is
//  a multiple of 4.
 /****************************************************************/
static void DM_Encode(unsigned char *output,
                                unsigned long int *input,
                                unsigned int len
                                )
{
  unsigned int i, j;

      for (i = 0, j = 0; j < len-3; i++, j += 4)  //modify by wangxiaolin 2009.12.20 ,"j<len"->"j<len-3" for warning
     {
             output[j] = (unsigned char)(input[i] & 0xff);
             output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);
             output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);
             output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);
      }
}
/****************************************************************/
// Decodes input (unsigned char) into output (UINT4). Assumes len is
//  a multiple of 4.
/****************************************************************/
LOCAL void DM_Decode(unsigned long int *output,
                                unsigned char *input,
                                unsigned int len
                               )
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len-3; i++, j += 4)  //modify by wangxiaolin 2009.12.20,"j<len"->"j<len-3" for warning
  {
     output[i] = ((unsigned long int)input[j]) | (((unsigned long int)input[j+1]) << 8) |
                     (((unsigned long int)input[j+2]) << 16) | (((unsigned long int)input[j+3]) << 24);
  }
 }
/****************************************************************/
// MD5 basic transformation. Transforms state based on block.
/****************************************************************/
LOCAL void DM_MD5Transform(unsigned long int state[4],unsigned char block[64])
{
  unsigned long int a = state[0], b = state[1], c = state[2], d = state[3], x[16];

  DM_Decode(x, block, 64);

  /* Round 1 */
  FF (a, b, c, d, x[ 0], S11, 0xd76aa478); /* 1 */
  FF (d, a, b, c, x[ 1], S12, 0xe8c7b756); /* 2 */
  FF (c, d, a, b, x[ 2], S13, 0x242070db); /* 3 */
  FF (b, c, d, a, x[ 3], S14, 0xc1bdceee); /* 4 */
  FF (a, b, c, d, x[ 4], S11, 0xf57c0faf); /* 5 */
  FF (d, a, b, c, x[ 5], S12, 0x4787c62a); /* 6 */
  FF (c, d, a, b, x[ 6], S13, 0xa8304613); /* 7 */
  FF (b, c, d, a, x[ 7], S14, 0xfd469501); /* 8 */
  FF (a, b, c, d, x[ 8], S11, 0x698098d8); /* 9 */
  FF (d, a, b, c, x[ 9], S12, 0x8b44f7af); /* 10 */
  FF (c, d, a, b, x[10], S13, 0xffff5bb1); /* 11 */
  FF (b, c, d, a, x[11], S14, 0x895cd7be); /* 12 */
  FF (a, b, c, d, x[12], S11, 0x6b901122); /* 13 */
  FF (d, a, b, c, x[13], S12, 0xfd987193); /* 14 */
  FF (c, d, a, b, x[14], S13, 0xa679438e); /* 15 */
  FF (b, c, d, a, x[15], S14, 0x49b40821); /* 16 */

 /* Round 2 */
  GG (a, b, c, d, x[ 1], S21, 0xf61e2562); /* 17 */
  GG (d, a, b, c, x[ 6], S22, 0xc040b340); /* 18 */
  GG (c, d, a, b, x[11], S23, 0x265e5a51); /* 19 */
  GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa); /* 20 */
  GG (a, b, c, d, x[ 5], S21, 0xd62f105d); /* 21 */
  GG (d, a, b, c, x[10], S22,  0x2441453); /* 22 */
  GG (c, d, a, b, x[15], S23, 0xd8a1e681); /* 23 */
  GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8); /* 24 */
  GG (a, b, c, d, x[ 9], S21, 0x21e1cde6); /* 25 */
  GG (d, a, b, c, x[14], S22, 0xc33707d6); /* 26 */
  GG (c, d, a, b, x[ 3], S23, 0xf4d50d87); /* 27 */
  GG (b, c, d, a, x[ 8], S24, 0x455a14ed); /* 28 */
  GG (a, b, c, d, x[13], S21, 0xa9e3e905); /* 29 */
  GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8); /* 30 */
  GG (c, d, a, b, x[ 7], S23, 0x676f02d9); /* 31 */
  GG (b, c, d, a, x[12], S24, 0x8d2a4c8a); /* 32 */

  /* Round 3 */
  HH (a, b, c, d, x[ 5], S31, 0xfffa3942); /* 33 */
  HH (d, a, b, c, x[ 8], S32, 0x8771f681); /* 34 */
  HH (c, d, a, b, x[11], S33, 0x6d9d6122); /* 35 */
  HH (b, c, d, a, x[14], S34, 0xfde5380c); /* 36 */
  HH (a, b, c, d, x[ 1], S31, 0xa4beea44); /* 37 */
  HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9); /* 38 */
  HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60); /* 39 */
  HH (b, c, d, a, x[10], S34, 0xbebfbc70); /* 40 */
  HH (a, b, c, d, x[13], S31, 0x289b7ec6); /* 41 */
  HH (d, a, b, c, x[ 0], S32, 0xeaa127fa); /* 42 */
  HH (c, d, a, b, x[ 3], S33, 0xd4ef3085); /* 43 */
  HH (b, c, d, a, x[ 6], S34,  0x4881d05); /* 44 */
  HH (a, b, c, d, x[ 9], S31, 0xd9d4d039); /* 45 */
  HH (d, a, b, c, x[12], S32, 0xe6db99e5); /* 46 */
  HH (c, d, a, b, x[15], S33, 0x1fa27cf8); /* 47 */
  HH (b, c, d, a, x[ 2], S34, 0xc4ac5665); /* 48 */

  /* Round 4 */
  II (a, b, c, d, x[ 0], S41, 0xf4292244); /* 49 */
  II (d, a, b, c, x[ 7], S42, 0x432aff97); /* 50 */
  II (c, d, a, b, x[14], S43, 0xab9423a7); /* 51 */
  II (b, c, d, a, x[ 5], S44, 0xfc93a039); /* 52 */
  II (a, b, c, d, x[12], S41, 0x655b59c3); /* 53 */
  II (d, a, b, c, x[ 3], S42, 0x8f0ccc92); /* 54 */
  II (c, d, a, b, x[10], S43, 0xffeff47d); /* 55 */
  II (b, c, d, a, x[ 1], S44, 0x85845dd1); /* 56 */
  II (a, b, c, d, x[ 8], S41, 0x6fa87e4f); /* 57 */
  II (d, a, b, c, x[15], S42, 0xfe2ce6e0); /* 58 */
  II (c, d, a, b, x[ 6], S43, 0xa3014314); /* 59 */
  II (b, c, d, a, x[13], S44, 0x4e0811a1); /* 60 */
  II (a, b, c, d, x[ 4], S41, 0xf7537e82); /* 61 */
  II (d, a, b, c, x[11], S42, 0xbd3af235); /* 62 */
  II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb); /* 63 */
  II (b, c, d, a, x[ 9], S44, 0xeb86d391); /* 64 */

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;

  /* Zeroize sensitive information.*/
  DM_MD5_memset((unsigned char*)x, 0, sizeof (x));
}
/****************************************************************/
// MD5 basic UPDATE. UPDATE state based on block.
/****************************************************************/
LOCAL void DMMD5Update(MD5_CTX *context,                                        /* context */
                                      char *input,                               /* input block */
                                      unsigned int inputLen                     /* length of input block */
                                      )
{
  unsigned int i, index, partLen;

  /* Compute number of bytes mod 64 */
  index = (unsigned int)((context->count[0] >> 3) & 0x3F);

  /* Update number of bits */
  if ((context->count[0] += ((unsigned long int)inputLen << 3))
   < ((unsigned long int)inputLen << 3))
 context->count[1]++;
  context->count[1] += ((unsigned long int)inputLen >> 29);

  partLen = 64 - index;

  /* Transform as many times as possible.
*/
  if (inputLen >= partLen) {
 DM_MD5_memcpy((unsigned char*)&context->buffer[index], (unsigned char*)input, partLen);
 DM_MD5Transform(context->state, context->buffer);

 for(i = partLen; i + 63 < inputLen; i += 64)
   DM_MD5Transform(context->state, (unsigned char *)&input[i]);

 index = 0;
  }
  else
 i = 0;

  /* Buffer remaining input */
  DM_MD5_memcpy((unsigned char*)&context->buffer[index], (unsigned char*)&input[i],inputLen-i);
}
/****************************************************************/
// MD5 basic FINAL. UPDATE state based on block.
/****************************************************************/
LOCAL void DMMD5Final ( char digest[16],                         /* message digest */
                                   MD5_CTX *context                                    /* context */
                                 )
{
  unsigned char bits[8];
  unsigned int index, padLen;

  /* Save number of bits */
  DM_Encode(bits, context->count, 8);

  /* Pad out to 56 mod 64.
*/
  index = (unsigned int)((context->count[0] >> 3) & 0x3f);
  padLen = (index < 56) ? (56 - index) : (120 - index);
  DMMD5Update (context, (char *)PADDING, padLen);

  /* Append length (before padding) */
  DMMD5Update (context, (char *)bits, 8);
  /* Store state in digest */
  DM_Encode((unsigned char *)digest, context->state, 16);

  /* Zeroize sensitive information.
*/
  DM_MD5_memset((unsigned char*)context, 0, sizeof (*context));
}

/******************************************************************* 
** 函数名:mmidm_calc_md5_cred
** 功能描述：计算md5 authentication所用的cred值
** 输  出: 
** 作  者:MARYXIAO

*******************************************************************/
PUBLIC char*  mmidm_calc_md5_cred(char* creddata)
{
    /*******************************************/
    /* Digest authentication:                  */
    /* compute the digest according to RFC2617 */
    /*******************************************/

    MD5_CTX Md5Ctx;
    HASH HA1;
    //int i = 0;
    DMMD5Init(&Md5Ctx);
    DMMD5Update(&Md5Ctx, creddata, strlen(creddata));
    DMMD5Final(HA1, &Md5Ctx);
    xppMemcpy(creddata, HA1, HASHLEN);
    creddata[HASHLEN] = 0;
    return creddata;

}

/*****************************************************************/
/* Function: pre-compute the size of the base64 encoded document */
/****************************************************************/

unsigned long mmidm_base64Encode (unsigned char* pbTarget,       // o: target buffer
                     unsigned long cbTargetSize,   // i: target buffer size
                     unsigned char* pbData,         // i: Data buffer
                     unsigned long *pcbDataLength, // i/o: Data buffer size
                     unsigned long *pcbOffset,     // i/o: absolute # of bytes encoded so far
                     unsigned int bLast,          // i: 0=first block, 1= next block, 2=last block
                     unsigned char *pbSaveBytes)  // i/o: last incomplete data block
   {
   unsigned char* pbSourceData = pbData;
   unsigned long cbCopySize = 0;
   unsigned long cbDataProcessed = *pcbDataLength;
   unsigned int i0, i1, i2, i3;
   unsigned int byt;
   int iSave = 1;
   static char t [] =   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"    // 26
                        "abcdefghijklmnopqrstuvwxyz"    // 26
                        "0123456789+/"                  // 12
                        "=";                            // 1
   i0=i1=i2=i3=0;
   // Check for NULL data buffer,
   if (pbData == NULL ) {
       // See if last block and there is any "saved" data that needs to go now.
       if ( bLast && ( pbSaveBytes && pbSaveBytes [0] )) {
          /**************************************/
          /* Check if it is time to send a CRLF */
          /**************************************/
          if ((*pcbOffset) > 0 && ((*pcbOffset) % MAX_COLUMNS == 0))
             {
             //if (cbTargetSize < 6)        // there is not enough space in the target buffer:
             //   break;                    // return to the caller.
             *pbTarget = '\r';
             *(pbTarget+1) = '\n';
             cbCopySize += 2;
             cbTargetSize -= 2;
             pbTarget += 2;
             }

          byt = (unsigned int) pbSaveBytes [iSave]; iSave ++; pbSaveBytes [0] --;

          i0 =  byt >> 2;
          i1 =  (byt & 0x0003) << 4;

          (*pcbOffset) ++;

#if 0//del by wangxiaolin   2009.12.20  for del warning
          if (pbSaveBytes && pbSaveBytes [0]) /*lint !e774*/
#endif
             {
              byt = (unsigned int) pbSaveBytes [iSave]; iSave ++; pbSaveBytes [0] --;
              i1 += (byt >> 4);
              i2 =  (byt & 0x000F) << 2;
             }
#if 0//del by wangxiaolin   2009.12.20 for del warning
             else
             {
                 i2 = i3 = 64;  // index to the padding char '=';
             }
#endif

          pbTarget [0] = t[i0];
          pbTarget [1] = t[i1];
          pbTarget [2] = t[i2];
          pbTarget [3] = t[i3];

          cbCopySize += 4;
          cbTargetSize -= 4;
          pbTarget += 4;
       }

   } else {
       while ((cbTargetSize >= 4) &&
              ( ((cbDataProcessed >= 3) && (bLast == 0)) ||
                ((cbDataProcessed >  0) && (bLast == 1)) ))
          {
          /**************************************/
          /* Check if it is time to send a CRLF */
          /**************************************/
          if ((*pcbOffset) > 0 && ((*pcbOffset) % MAX_COLUMNS == 0))
             {
             if (cbTargetSize < 6)        // there is not enough space in the target buffer:
                break;                    // return to the caller.
             *pbTarget = '\r';
             *(pbTarget+1) = '\n';
             cbCopySize += 2;
             cbTargetSize -= 2;
             pbTarget += 2;
             }

          if (pbSaveBytes && pbSaveBytes [0])
             { byt = (unsigned int) pbSaveBytes [iSave]; iSave ++; pbSaveBytes [0] --; }
          else
             { byt = (unsigned int) *pbSourceData; pbSourceData ++; cbDataProcessed --;}

          i0 =  byt >> 2;
          i1 =  (byt & 0x0003) << 4;

          (*pcbOffset) ++;

          if (cbDataProcessed > 0)
             {
             if (pbSaveBytes && pbSaveBytes [0])
                { byt = (unsigned int) pbSaveBytes [iSave]; iSave ++; pbSaveBytes [0] --; }
             else
                { byt = (unsigned int) *pbSourceData; pbSourceData ++; cbDataProcessed --;}

             i1 += (byt >> 4);
             i2 =  (byt & 0x000F) << 2;

             (*pcbOffset) ++;

             if (cbDataProcessed > 0)
                {
                if (pbSaveBytes && pbSaveBytes [0])
                   { byt = (unsigned int) pbSaveBytes [iSave]; iSave ++; pbSaveBytes [0] --; }
                else
                   { byt = (unsigned int) *pbSourceData; pbSourceData ++; cbDataProcessed --;}

                i2 += (byt & 0x00C0) >> 6;
                i3 =  byt & 0x003F;
                (*pcbOffset) ++;
                }
             else
                i3 = 64;  // index to the padding char '=';
             }
          else
             i2 = i3 = 64;  // index to the padding char '=';
          pbTarget [0] = t[i0];
          pbTarget [1] = t[i1];
          pbTarget [2] = t[i2];
          pbTarget [3] = t[i3];

          cbCopySize += 4;
          cbTargetSize -= 4;
          pbTarget += 4;
       }
   }



   /*************************************************************/
   /* Save the bytes that must be processed in the following    */
   /* call (max. 2 Bytes).                                      */
   /*************************************************************/
   if ((bLast == 0) && (cbDataProcessed <= 2) && (pbSaveBytes != NULL))
      {
      pbSaveBytes[0] = (unsigned char)cbDataProcessed;
      while (cbDataProcessed)
         {
         *(++pbSaveBytes) = pbSourceData[0];
         cbDataProcessed --; pbSourceData ++;
         }
      }

   /*****************************************************************/
   /* Shift all non-processed data to the start of the input buffer */
   /*****************************************************************/

   if (cbDataProcessed > 0)
      {
         if(NULL!=pbData)  //add by wangxiaolin 2009.12.20 for del warning
         {
            xppMemmove(pbData, pbSourceData, cbDataProcessed);/*lint !e668*/
         }
      }
   *pcbDataLength = cbDataProcessed;

   return cbCopySize;
   }
/******************************************************************* 
** 函数名:mmidm_calc_b64_cred
** 功能描述：计算b64 authentication所用的cred值
** 输  出: 
** 作  者:mary.xiao

*******************************************************************/
PUBLIC void mmidm_calc_b64_cred(char* creddata, unsigned long cbLength)
{
    /**************************************/
    /* Basic authentication:              */
    /* calculate a base64-encoding of the */
    /* string <user>:<password>           */
    /**************************************/

    char abSave[4];
    int cbCopied=0;
    unsigned long cbDigestSize = 80;
    unsigned long cbOffset = 0; 
    unsigned char* outputdata;
    abSave [0] = abSave [1] = abSave [2] = abSave [3] = '\0';
    if (0 == cbLength)
    {
        creddata[0] = '\0'; // chenxiang 20071225
        return;
    }
    outputdata = xppMalloc(cbLength * 2 + 4);
    cbCopied = mmidm_base64Encode ((unsigned char*)outputdata, cbDigestSize,
                     (unsigned char*) creddata,
                     &cbLength, &cbOffset, 1, (unsigned char *)abSave);
    outputdata[cbCopied] = '\0';
    xppMemcpy(creddata, outputdata, cbCopied);
    creddata[cbCopied] = '\0';
    xppFree(outputdata);

}