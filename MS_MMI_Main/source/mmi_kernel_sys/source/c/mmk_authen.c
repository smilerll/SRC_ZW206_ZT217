#ifdef DYNAMIC_MODULE_SUPPORT
/****************************************************************************
** File Name:      mmk_authen.c                                            *
** Author:         Wei.Zhou                                                *
** Date:           03/21/2009                                              *
** Copyright:      2008 Spreadtrum, Incoporated. All Rights Reserved.       *
** Description:    This file is used to describe the application modual.   *
****************************************************************************
**                         Important Edit History                          *
** ------------------------------------------------------------------------*
** DATE           NAME             DESCRIPTION                             *
** 03/2009        Wei.Zhou         Create
** 
****************************************************************************/
#include "sci_types.h"
#include "mmk_modinfo.h"
#include "mmk_authen.h"
#include "mmk_app.h"
#include "os_api.h"
#include "sfs.h"


/**--------------------------------------------------------------------------*
**                         FILE DESCRIPTION                                  *
**--------------------------------------------------------------------------*/
/*
    The file upper part. include two arithmetic, SHA1 & MD5. arithmetic source from RFC3174 & RFC1321
    Author:
        SHA1:   Donald E. Eastlake, 3rd & Paul E. Jones
        MD5:    Ronald L. Rivest

    The application part by wei.zhou
*/


/*
*
*  Description:
*      This file implements the Secure Hashing Algorithm 1 as
*      defined in FIPS PUB 180-1 published April 17, 1995.
*
*      The SHA-1, produces a 160-bit message digest for a given
*      data stream.  It should take about 2**n steps to find a
*      message with the same digest as a given message and
*      2**(n/2) to find any two messages with the same digest,
*      when n is the digest size in bits.  Therefore, this
*      algorithm can serve as a means of providing a
*      "fingerprint" for a message.
*
*  Portability Issues:
*      SHA-1 is defined in terms of 32-bit "words".  This code
*      uses <stdint.h> (included via "sha1.h" to define 32 and 8
*      bit unsigned integer types.  If your C compiler does not
*      support 32 bit unsigned integers, this code is not
*      appropriate.
*
*  Caveats:
*      SHA-1 is designed to work with messages less than 2^64 bits
*      long.  Although SHA-1 allows a message digest to be generated
*      for messages of any number of bits less than 2^64, this
*      implementation only works with messages with a length that is
*      a multiple of the size of an 8-bit character.
*
 */

/*
 *  Define the SHA1 circular left shift macro
 */
#define SHA1CircularShift(bits,word) \
                (((word) << (bits)) | ((word) >> (32-(bits))))

extern uint32 GUI_WstrToGB(						// return the length of gb2312_ptr
                           uint8  *gb_ptr,			// store the gb2312 string code
                           const wchar  *wstr_ptr,  // the ucs2 stringcode that need to be converted.
                           uint32   wstr_len         //the unicode length
                           );

extern uint32 GUI_GBToWstr(wchar *wstr_ptr, const uint8 *gb_ptr,uint32 len);

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/* Local Function Prototyptes */
LOCAL void SHA1PadMessage(SHA1Context *);
LOCAL void SHA1ProcessMessageBlock(SHA1Context *);

LOCAL int SHA1Reset(SHA1Context *);
LOCAL int SHA1Input(SHA1Context *,  const uint8 *, unsigned int);
LOCAL int SHA1Result(SHA1Context *, uint8 Message_Digest[SHA1HashSize]);




/*
 *  SHA1Reset
 *
 *  Description:
 *      This function will initialize the SHA1Context in preparation
 *      for computing a new SHA1 message digest.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to reset.
 *
 *  Returns:
 *      sha Error Code.
 *
 */
int SHA1Reset(SHA1Context *context)
{
    if (!context)
    {
        return shaNull;
    }

    context->Length_Low             = 0;
    context->Length_High            = 0;
    context->Message_Block_Index    = 0;

    context->Intermediate_Hash[0]   = 0x67452301;
    context->Intermediate_Hash[1]   = 0xEFCDAB89;
    context->Intermediate_Hash[2]   = 0x98BADCFE;
    context->Intermediate_Hash[3]   = 0x10325476;
    context->Intermediate_Hash[4]   = 0xC3D2E1F0;

    context->Computed   = 0;
    context->Corrupted  = 0;

    return shaSuccess;
}

/*
 *  SHA1Result
 *
 *  Description:
 *      This function will return the 160-bit message digest into the
 *      Message_Digest array  provided by the caller.
 *      NOTE: The first octet of hash is stored in the 0th element,
 *            the last octet of hash in the 19th element.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to use to calculate the SHA-1 hash.
 *      Message_Digest: [out]
 *          Where the digest is returned.
 *
 *  Returns:
 *      sha Error Code.
 *
 */
int SHA1Result( SHA1Context *context,
                uint8 Message_Digest[SHA1HashSize])
{
    int i;

    if (!context || !Message_Digest)
    {
        return shaNull;
    }

    if (context->Corrupted)
    {
        return context->Corrupted;
    }

    if (!context->Computed)
    {
        SHA1PadMessage(context);
        for(i=0; i<64; ++i)
        {
            /* message may be sensitive, clear it out */
            context->Message_Block[i] = 0;
        }
        context->Length_Low = 0;    /* and clear length */
        context->Length_High = 0;
        context->Computed = 1;

    }

    for(i = 0; i < SHA1HashSize; ++i)
    {
        Message_Digest[i] = context->Intermediate_Hash[i>>2]
                            >> 8 * ( 3 - ( i & 0x03 ) );/*lint !e504*/
    }

    return shaSuccess;
}

/*
 *  SHA1Input
 *
 *  Description:
 *      This function accepts an array of octets as the next portion
 *      of the message.
 *
 *  Parameters:
 *      context: [in/out]
 *          The SHA context to update
 *      message_array: [in]
 *          An array of characters representing the next portion of
 *          the message.
 *      length: [in]
 *          The length of the message in message_array
 *
 *  Returns:
 *      sha Error Code.
 *
 */
int SHA1Input(    SHA1Context    *context,
                  const uint8  *message_array,
                  unsigned       length)
{
    if (!length)
    {
        return shaSuccess;
    }

    if (!context || !message_array)
    {
        return shaNull;
    }

    if (context->Computed)
    {
        context->Corrupted = shaStateError;
        return shaStateError;
    }

    if (context->Corrupted)
    {
         return context->Corrupted;
    }
    while(length-- && !context->Corrupted)
    {
        context->Message_Block[context->Message_Block_Index++] =
                        (*message_array & 0xFF);

        context->Length_Low += 8;
        if (context->Length_Low == 0)
        {
            context->Length_High++;
            if (context->Length_High == 0)
            {
                /* Message is too long */
                context->Corrupted = 1;
            }
        }

        if (context->Message_Block_Index == 64)
        {
            SHA1ProcessMessageBlock(context);
        }

        message_array++;
    }

    return shaSuccess;
}

/*
 *  SHA1ProcessMessageBlock
 *
 *  Description:
 *      This function will process the next 512 bits of the message
 *      stored in the Message_Block array.
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *      Many of the variable names in this code, especially the
 *      single character names, were used because those were the
 *      names used in the publication.
 *
 *
 */
void SHA1ProcessMessageBlock(SHA1Context *context)
{
    const uint32 K[] =    {       /* Constants defined in SHA-1   */
                            0x5A827999,
                            0x6ED9EBA1,
                            0x8F1BBCDC,
                            0xCA62C1D6
                            };
    int           t;                 /* Loop counter                */
    uint32      temp;              /* Temporary word value        */
    uint32      W[80];             /* Word sequence               */
    uint32      A, B, C, D, E;     /* Word buffers                */

    /*
     *  Initialize the first 16 words in the array W
     */
    for(t = 0; t < 16; t++)
    {
        W[t] = context->Message_Block[t * 4] << 24;
        W[t] |= context->Message_Block[t * 4 + 1] << 16;
        W[t] |= context->Message_Block[t * 4 + 2] << 8;
        W[t] |= context->Message_Block[t * 4 + 3];
    }

    for(t = 16; t < 80; t++)
    {
       W[t] = SHA1CircularShift(1,W[t-3] ^ W[t-8] ^ W[t-14] ^ W[t-16]);
    }

    A = context->Intermediate_Hash[0];
    B = context->Intermediate_Hash[1];
    C = context->Intermediate_Hash[2];
    D = context->Intermediate_Hash[3];
    E = context->Intermediate_Hash[4];

    for(t = 0; t < 20; t++)
    {
        temp =  SHA1CircularShift(5,A) +
                ((B & C) | ((~B) & D)) + E + W[t] + K[0];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    for(t = 20; t < 40; t++)
    {
        temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[1];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    for(t = 40; t < 60; t++)
    {
        temp = SHA1CircularShift(5,A) +
               ((B & C) | (B & D) | (C & D)) + E + W[t] + K[2];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    for(t = 60; t < 80; t++)
    {
        temp = SHA1CircularShift(5,A) + (B ^ C ^ D) + E + W[t] + K[3];
        E = D;
        D = C;
        C = SHA1CircularShift(30,B);
        B = A;
        A = temp;
    }

    context->Intermediate_Hash[0] += A;
    context->Intermediate_Hash[1] += B;
    context->Intermediate_Hash[2] += C;
    context->Intermediate_Hash[3] += D;
    context->Intermediate_Hash[4] += E;

    context->Message_Block_Index = 0;
}


/*
 *  SHA1PadMessage
 *
 *  Description:
 *      According to the standard, the message must be padded to an even
 *      512 bits.  The first padding bit must be a '1'.  The last 64
 *      bits represent the length of the original message.  All bits in
 *      between should be 0.  This function will pad the message
 *      according to those rules by filling the Message_Block array
 *      accordingly.  It will also call the ProcessMessageBlock function
 *      provided appropriately.  When it returns, it can be assumed that
 *      the message digest has been computed.
 *
 *  Parameters:
 *      context: [in/out]
 *          The context to pad
 *      ProcessMessageBlock: [in]
 *          The appropriate SHA*ProcessMessageBlock function
 *  Returns:
 *      Nothing.
 *
 */

void SHA1PadMessage(SHA1Context *context)
{
    /*
     *  Check to see if the current message block is too small to hold
     *  the initial padding bits and length.  If so, we will pad the
     *  block, process it, and then continue padding into a second
     *  block.
     */
    if (context->Message_Block_Index > 55)
    {
        context->Message_Block[context->Message_Block_Index++] = 0x80;
        while(context->Message_Block_Index < 64)
        {
            context->Message_Block[context->Message_Block_Index++] = 0;
        }

        SHA1ProcessMessageBlock(context);

        while(context->Message_Block_Index < 56)
        {
            context->Message_Block[context->Message_Block_Index++] = 0;
        }
    }
    else
    {
        context->Message_Block[context->Message_Block_Index++] = 0x80;
        while(context->Message_Block_Index < 56)
        {
            context->Message_Block[context->Message_Block_Index++] = 0;
        }
    }

    /*
     *  Store the message length as the last 8 octets
     */
    context->Message_Block[56] = context->Length_High >> 24;
    context->Message_Block[57] = context->Length_High >> 16;
    context->Message_Block[58] = context->Length_High >> 8;
    context->Message_Block[59] = context->Length_High;
    context->Message_Block[60] = context->Length_Low >> 24;
    context->Message_Block[61] = context->Length_Low >> 16;
    context->Message_Block[62] = context->Length_Low >> 8;
    context->Message_Block[63] = context->Length_Low;

    SHA1ProcessMessageBlock(context);
}

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */

/* Constants for MD5Transform routine.
 */

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

LOCAL void MD5Transform(uint32 [4], unsigned char [64]);
LOCAL void Encode(unsigned char *, uint32 *, unsigned int);
LOCAL void Decode(uint32 *, unsigned char *, unsigned int);
LOCAL void MD5_memcpy(char *, char *, unsigned int);
LOCAL void MD5_memset(char *, int, unsigned int);

LOCAL unsigned char PADDING[64] = {
  0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* F, G, H and I are basic MD5 functions.
 */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
Rotation is separate from addition to prevent recomputation.
 */
#define FF(a, b, c, d, x, s, ac) { \
 (a) += F ((b), (c), (d)) + (x) + (uint32)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define GG(a, b, c, d, x, s, ac) { \
 (a) += G ((b), (c), (d)) + (x) + (uint32)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define HH(a, b, c, d, x, s, ac) { \
 (a) += H ((b), (c), (d)) + (x) + (uint32)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }
#define II(a, b, c, d, x, s, ac) { \
 (a) += I ((b), (c), (d)) + (x) + (uint32)(ac); \
 (a) = ROTATE_LEFT ((a), (s)); \
 (a) += (b); \
  }

/* MD5 initialization. Begins an MD5 operation, writing a new context.
 */
/* context */
void MD5Init (MD5_CTX *context)
{
  context->count[0] = context->count[1] = 0;
  /* Load magic initialization constants.
*/
  context->state[0] = 0x67452301;
  context->state[1] = 0xefcdab89;
  context->state[2] = 0x98badcfe;
  context->state[3] = 0x10325476;
}

/* MD5 block update operation. Continues an MD5 message-digest
  operation, processing another message block, and updating the
  context.
 */
/* context */
/* input block */
/* length of input block */
void MD5Update (MD5_CTX *context, unsigned char *input, unsigned int inputLen)
{
  unsigned int i, index, partLen;

  /* Compute number of bytes mod 64 */
  index = (unsigned int)((context->count[0] >> 3) & 0x3F);

  /* Update number of bits */
  if ((context->count[0] += ((uint32)inputLen << 3))
   < ((uint32)inputLen << 3))
 context->count[1]++;
  context->count[1] += ((uint32)inputLen >> 29);

  partLen = 64 - index;

  /* Transform as many times as possible.
*/
  if (inputLen >= partLen) {
 MD5_memcpy
   ((char *)&context->buffer[index], (char *)input, partLen);
 MD5Transform (context->state, context->buffer);

 for (i = partLen; i + 63 < inputLen; i += 64)
   MD5Transform (context->state, &input[i]);

 index = 0;
  }
  else
 i = 0;

  /* Buffer remaining input */
  MD5_memcpy
 ((char *)&context->buffer[index], (char *)&input[i],
  inputLen-i);
}

/* MD5 finalization. Ends an MD5 message-digest operation, writing the
  the message digest and zeroizing the context.
 */
/* message digest */
/* context */
void MD5Final (unsigned char digest[16], MD5_CTX *context)
{
  unsigned char bits[8];
  unsigned int index, padLen;

  /* Save number of bits */
  Encode (bits, context->count, 8);

  /* Pad out to 56 mod 64. */
  index = (unsigned int)((context->count[0] >> 3) & 0x3f);
  padLen = (index < 56) ? (56 - index) : (120 - index);
  MD5Update (context, PADDING, padLen);

  /* Append length (before padding) */
  MD5Update (context, bits, 8);

  /* Store state in digest */
  Encode (digest, context->state, 16);

  /* Zeroize sensitive information.
*/
  MD5_memset ((char *)context, 0, sizeof (*context));
}


/* MD5 basic transformation. Transforms state based on block.
 */
LOCAL void MD5Transform (uint32 state[4], unsigned char block[64])
{
  uint32 a = state[0], b = state[1], c = state[2], d = state[3], x[16];

  Decode (x, block, 64);

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
  MD5_memset ((char *)x, 0, sizeof (x));
}

/* Encodes input (uint32) into output (unsigned char). Assumes len is
  a multiple of 4.
 */
LOCAL void Encode (unsigned char *output, uint32 *input, unsigned int len)
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4) {
      output[j] = (unsigned char)(input[i] & 0xff);
      output[j+1] = (unsigned char)((input[i] >> 8) & 0xff);    /*lint !e661 */
      output[j+2] = (unsigned char)((input[i] >> 16) & 0xff);   /*lint !e661 !e662 */
      output[j+3] = (unsigned char)((input[i] >> 24) & 0xff);   /*lint !e661 !e662 */
  }
}

/* Decodes input (unsigned char) into output (uint32). Assumes len is
  a multiple of 4.
 */
LOCAL void Decode (uint32 *output, unsigned char *input, unsigned int len)
{
  unsigned int i, j;

  for (i = 0, j = 0; j < len; i++, j += 4)
 output[i] = ((uint32)input[j]) | (((uint32)input[j+1]) << 8) |   /*lint !e661 */
   (((uint32)input[j+2]) << 16) | (((uint32)input[j+3]) << 24);   /*lint !e661 !e662 */
}

/* Note: Replace "for loop" with standard memcpy if possible.
 */

LOCAL void MD5_memcpy (char * output, char * input, unsigned int len)
{
  unsigned int i;

  for (i = 0; i < len; i++)
 		output[i] = input[i];
}

/* Note: Replace "for loop" with standard memset if possible.
 */
LOCAL void MD5_memset (char * output, int value, unsigned int len)
{
  unsigned int i;

  for (i = 0; i < len; i++)
 ((char *)output)[i] = (char)value;
}

/**--------------------------------------------------------------------------*
**                         FILE DESCRIPTION                                  *
**--------------------------------------------------------------------------*/
/*
    The file lower part, file authen mechanism
    Author: wei.zhou
*/

/**--------------------------------------------------------------------------*
**                         MACRO DEFINITION                                 *
**--------------------------------------------------------------------------*/

#define     SIGN        "sgn"
#define     FILEBUF     1600
#define     SIGNLEN     16


/**--------------------------------------------------------------------------*
**                         CONST DEFINITION                                 *
**--------------------------------------------------------------------------*/
LOCAL const uint8 privatecode[]={"Ad&127dA^!+lMka)d`~Pn3_2dn"};

/*---------------------------------------------------------------------------*/
/*                          LOCAL FUNCTION DECLARE                           */
/*---------------------------------------------------------------------------*/

/*****************************************************************************/
// 	Description : produce sign file by source file.
//	Global resource dependence : 
//  Author:Wei.Zhou
//	Note:
/*****************************************************************************/
LOCAL int32 MMK_MakeSgnFile(const wchar *filename_ptr, const wchar *signfilename_ptr);

/*****************************************************************************/
// 	Description : Get the content of the source FILE digst, use SHA1 arithmetic
//	Global resource dependence : 
//  Author:Wei.Zhou
//	Note:
/*****************************************************************************/
LOCAL int32 MMK_GetFileDigst(SFS_HANDLE fh, uint8 *digst);

/*****************************************************************************/
// 	Description : Convert the stream to MD5 context, use MD5 arithmetic
//	Global resource dependence : 
//  Author:Wei.Zhou
//	Note:
/*****************************************************************************/
LOCAL int32 MMK_ProduceSgnStream(uint8 *secreat, int32 scrlen, uint8 *sign);

/*****************************************************************************/
// 	Description : Read the data sign file content
//	Global resource dependence : 
//  Author:Wei.Zhou
//	Note:
/*****************************************************************************/
LOCAL int32 MMK_GetSgnFileData(SFS_HANDLE fsgn, uint32 scrlen, uint8 *sign);

/*****************************************************************************/
// 	Description : produce sign file by source file.
//	Global resource dependence : 
//  Author:Wei.Zhou
//	Note:
/*****************************************************************************/
LOCAL int32 MMK_MakeSgnFile(const wchar *filename_ptr, const wchar *signfilename_ptr)
{
    SFS_HANDLE f_handle;
    SFS_HANDLE f_signhandle;
    uint8   Message_Digest[20];
    uint8   Signtxt[16];
    uint8   SignFiletxt[16];
    uint8   signbuf[64];
    int32     ret = -1;
    
    SCI_MEMSET(signbuf, 0, 64);

    f_handle = SFS_CreateFile(filename_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/
    f_signhandle = SFS_CreateFile(signfilename_ptr, SFS_MODE_READ|SFS_MODE_OPEN_EXISTING, NULL, NULL);/*lint !e655*/

    if((f_handle == 0) || (f_signhandle == 0))
    {
        if(f_handle)
        {
            SFS_CloseFile(f_handle);
        }
        if(f_signhandle)
        {
            SFS_CloseFile(f_signhandle);
        }
        return ret;
    }

    MMK_GetFileDigst(f_handle, Message_Digest);
    memcpy(signbuf, Message_Digest, sizeof(Message_Digest));
    memcpy(signbuf + sizeof(Message_Digest), privatecode, strlen((char *)privatecode));
    MMK_ProduceSgnStream(signbuf, sizeof(Message_Digest) + strlen((char *)privatecode), Signtxt);
    MMK_GetSgnFileData(f_signhandle, SIGNLEN, SignFiletxt);

    ret = memcmp(Signtxt, SignFiletxt, SIGNLEN);

    SFS_CloseFile(f_handle);
    SFS_CloseFile(f_signhandle);

    return ret;
}

/*****************************************************************************/
// 	Description : Get the content of the source FILE digst, use SHA1 arithmetic
//	Global resource dependence : 
//  Author:Wei.Zhou
//	Note:
/*****************************************************************************/
LOCAL int32 MMK_GetFileDigst(SFS_HANDLE fh, uint8 *digst)
{
    int32         err;
    SHA1Context sha;
    uint8       *pBuf;
    uint32      len, filelen, oldfilelen;

    err = SHA1Reset(&sha);

    pBuf = (uint8 *)SCI_ALLOC_APP(FILEBUF);
    SCI_ASSERT(pBuf);/*assert verified*/

    filelen = 0;
    SFS_GetFileSize(fh, &oldfilelen);
    SFS_SetFilePointer(fh, (__int64)0, SFS_SEEK_BEGIN);

    while(TRUE)/*lint !e716*/
    {
        SFS_ReadFile(fh, pBuf, FILEBUF, &len, PNULL);

        if(len == 0)
        {
            break;
        }
        filelen += len;
        err = SHA1Input(&sha, (const uint8 *)pBuf, len);
    }
    
    err = SHA1Result(&sha, digst);
    SCI_FREE(pBuf);
    err = SHA1Reset(0);
    return 0;
}

/*****************************************************************************/
// 	Description : Convert the stream to MD5 context, use MD5 arithmetic
//	Global resource dependence : 
//  Author:Wei.Zhou
//	Note:
/*****************************************************************************/
LOCAL int32 MMK_ProduceSgnStream(uint8 *secreat, int32 scrlen, uint8 *sign)
{
    MD5_CTX context;
    
    MD5Init (&context);
    MD5Update (&context, secreat, scrlen);
    MD5Final (sign, &context);

    return 0;
}

/*****************************************************************************/
// 	Description : Read the data sign file content
//	Global resource dependence : 
//  Author:Wei.Zhou
//	Note:
/*****************************************************************************/
LOCAL int32 MMK_GetSgnFileData(SFS_HANDLE fsgn, uint32 scrlen, uint8 *sign)
{
    uint32 len;

    SFS_ReadFile(fsgn, sign, scrlen, &len, PNULL);
    SCI_ASSERT(len == scrlen);/*assert verified*/

    return 0;    
}

/*****************************************************************************/
// 	Description : verify the authentic file passed or not, file must be "bin" or "dll"
//	Global resource dependence : 
//  Author:Wei.Zhou
//	Note:
/*****************************************************************************/
PUBLIC BOOLEAN MMK_VerifySignFile(const wchar *filename_ptr)
{
    int32   ret;
    wchar   *signfilename_ptr;
    char    *file_ptr;
    char    *ptr;

    signfilename_ptr = (wchar *)SCI_ALLOC_APP((CAF_FILENAME_MAX_LEN + 1) * sizeof(wchar));
    file_ptr        = (char *)SCI_ALLOC_APP(CAF_FILENAME_MAX_LEN + 1);

    SCI_MEMSET(signfilename_ptr, 0, ((CAF_FILENAME_MAX_LEN + 1) * sizeof(wchar)));
    SCI_MEMSET(file_ptr, 0, (CAF_FILENAME_MAX_LEN + 1));

    MMIAPICOM_Wstrncpy( signfilename_ptr, filename_ptr, MMIAPICOM_Wstrlen(filename_ptr));

    //MMIAPICOM_WstrToStr(signfilename_ptr, file_ptr);
    GUI_WstrToGB((uint8*)file_ptr,signfilename_ptr,(uint16)MMIAPICOM_Wstrlen(signfilename_ptr) );

    ptr = strstr(file_ptr, ".bin");
    if(!ptr)
    {
        ptr = strstr(file_ptr, ".dll");
        if(!ptr)
        {
            SCI_ASSERT(0);/*assert verified*/
        }
    }

    *ptr = '_';
    strcat(file_ptr, ".sgn");

    //MMIAPICOM_StrToWstr( file_ptr, signfilename_ptr);
    GUI_GBToWstr(signfilename_ptr,(uint8*)file_ptr,strlen(file_ptr) );

    ret = MMK_MakeSgnFile(filename_ptr, signfilename_ptr);
    SCI_FREE(file_ptr);
    SCI_FREE(signfilename_ptr);
    if(ret != 0)
    {
        return FALSE;
    }

    return TRUE;
}
#endif
