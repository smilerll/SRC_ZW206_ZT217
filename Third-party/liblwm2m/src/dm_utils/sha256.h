/**
 * \file sha256.h
 *
 * \brief The SHA-224 and SHA-256 cryptographic hash function.
 */
/*
 *  Copyright (C) 2006-2018, Arm Limited (or its affiliates), All Rights Reserved
 *  SPDX-License-Identifier: GPL-2.0
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  This file is part of Mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_SHA256_H
#define MBEDTLS_SHA256_H

#include <stddef.h>
#include <stdint.h>

#define MBEDTLS_ERR_SHA256_HW_ACCEL_FAILED                -0x0037  /**< SHA-256 hardware accelerator failed */

#if ( defined(__ARMCC_VERSION) || defined(_MSC_VER) ) && \
    !defined(inline) && !defined(__cplusplus)
#define inline __inline
#endif
#if !defined(MBEDTLS_SHA256_ALT)
// Regular implementation
//

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          The SHA-256 context structure.
 *
 *                 The structure is used both for SHA-256 and for SHA-224
 *                 checksum calculations. The choice between these two is
 *                 made in the call to mbedtls_sha256_starts_ret().
 */
typedef struct
{
    uint32_t total[2];          /*!< The number of Bytes processed.  */
    uint32_t state[8];          /*!< The intermediate digest state.  */
    unsigned char buffer[64];   /*!< The data block being processed. */
    int is224;                  /*!< Determines which function to use.
                                     <ul><li>0: Use SHA-256.</li>
                                     <li>1: Use SHA-224.</li></ul> */
}
mbedtls_sha256_context;

/**
 * \brief          This function initializes a SHA-256 context.
 *
 * \param ctx      The SHA-256 context to initialize.
 */
void mbedtls_sha256_init_ex( mbedtls_sha256_context *ctx );

/**
 * \brief          This function clears a SHA-256 context.
 *
 * \param ctx      The SHA-256 context to clear.
 */
void mbedtls_sha256_free_ex( mbedtls_sha256_context *ctx );

/**
 * \brief          This function clones the state of a SHA-256 context.
 *
 * \param dst      The destination context.
 * \param src      The context to clone.
 */
void mbedtls_sha256_clone_ex( mbedtls_sha256_context *dst,
                           const mbedtls_sha256_context *src );

/**
 * \brief          This function starts a SHA-224 or SHA-256 checksum
 *                 calculation.
 *
 * \param ctx      The context to initialize.
 * \param is224    Determines which function to use.
 *                 <ul><li>0: Use SHA-256.</li>
 *                 <li>1: Use SHA-224.</li></ul>
 *
 * \return         \c 0 on success.
 */
int mbedtls_sha256_starts_ret( mbedtls_sha256_context *ctx, int is224 );

/**
 * \brief          This function feeds an input buffer into an ongoing
 *                 SHA-256 checksum calculation.
 *
 * \param ctx      SHA-256 context
 * \param input    buffer holding the data
 * \param ilen     length of the input data
 *
 * \return         \c 0 on success.
 */
int mbedtls_sha256_update_ret( mbedtls_sha256_context *ctx,
                               const  char *input,
                               size_t ilen );

/**
 * \brief          This function finishes the SHA-256 operation, and writes
 *                 the result to the output buffer.
 *
 * \param ctx      The SHA-256 context.
 * \param output   The SHA-224 or SHA-256 checksum result.
 *
 * \return         \c 0 on success.
 */
int mbedtls_sha256_finish_ret( mbedtls_sha256_context *ctx,
                                char output[32] );

/**
 * \brief          This function processes a single data block within
 *                 the ongoing SHA-256 computation. This function is for
 *                 internal use only.
 *
 * \param ctx      The SHA-256 context.
 * \param data     The buffer holding one block of data.
 *
 * \return         \c 0 on success.
 */
int mbedtls_internal_sha256_process( mbedtls_sha256_context *ctx,
                                     const  char data[64] );

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
#if defined(MBEDTLS_DEPRECATED_WARNING)
#define MBEDTLS_DEPRECATED      __attribute__((deprecated))
#else
#define MBEDTLS_DEPRECATED
#endif
/**
 * \brief          This function starts a SHA-256 checksum calculation.
 *
 * \deprecated     Superseded by mbedtls_sha256_starts_ret() in 2.7.0.
 *
 * \param ctx      The SHA-256 context to initialize.
 * \param is224    Determines which function to use.
 *                 <ul><li>0: Use SHA-256.</li>
 *                 <li>1: Use SHA-224.</li></ul>
 */
MBEDTLS_DEPRECATED static inline void mbedtls_sha256_starts(
                                                mbedtls_sha256_context *ctx,
                                                int is224 )
{
    mbedtls_sha256_starts_ret( ctx, is224 );
}

/**
 * \brief          This function feeds an input buffer into an ongoing
 *                 SHA-256 checksum calculation.
 *
 * \deprecated     Superseded by mbedtls_sha256_update_ret() in 2.7.0.
 *
 * \param ctx      The SHA-256 context to initialize.
 * \param input    The buffer holding the data.
 * \param ilen     The length of the input data.
 */
MBEDTLS_DEPRECATED static inline void mbedtls_sha256_update(
                                                mbedtls_sha256_context *ctx,
                                                const  char *input,
                                                size_t ilen )
{
    mbedtls_sha256_update_ret( ctx, input, ilen );
}

/**
 * \brief          This function finishes the SHA-256 operation, and writes
 *                 the result to the output buffer.
 *
 * \deprecated     Superseded by mbedtls_sha256_finish_ret() in 2.7.0.
 *
 * \param ctx      The SHA-256 context.
 * \param output   The SHA-224or SHA-256 checksum result.
 */
MBEDTLS_DEPRECATED static inline void mbedtls_sha256_finish(
                                                mbedtls_sha256_context *ctx,
                                                 char output[32] )
{
    mbedtls_sha256_finish_ret( ctx, output );
}

/**
 * \brief          This function processes a single data block within
 *                 the ongoing SHA-256 computation. This function is for
 *                 internal use only.
 *
 * \deprecated     Superseded by mbedtls_internal_sha256_process() in 2.7.0.
 *
 * \param ctx      The SHA-256 context.
 * \param data     The buffer holding one block of data.
 */
MBEDTLS_DEPRECATED static inline void mbedtls_sha256_process(
                                                mbedtls_sha256_context *ctx,
                                                const  char data[64] )
{
    mbedtls_internal_sha256_process( ctx, data );
}

#undef MBEDTLS_DEPRECATED
#endif /* !MBEDTLS_DEPRECATED_REMOVED */
#ifdef __cplusplus
}
#endif
#else  /* MBEDTLS_SHA256_ALT */
#include "sha256_alt.h"
#endif /* MBEDTLS_SHA256_ALT */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          This function calculates the SHA-224 or SHA-256
 *                 checksum of a buffer.
 *
 *                 The function allocates the context, performs the
 *                 calculation, and frees the context.
 *
 *                 The SHA-256 result is calculated as
 *                 output = SHA-256(input buffer).
 *
 * \param input    The buffer holding the input data.
 * \param ilen     The length of the input data.
 * \param output   The SHA-224 or SHA-256 checksum result.
 * \param is224    Determines which function to use.
 *                 <ul><li>0: Use SHA-256.</li>
 *                 <li>1: Use SHA-224.</li></ul>
 */
int mbedtls_sha256_ret( const  char *input,
                        size_t ilen,
                         char output[32],
                        int is224 );

#if !defined(MBEDTLS_DEPRECATED_REMOVED)
#if defined(MBEDTLS_DEPRECATED_WARNING)
#define MBEDTLS_DEPRECATED      __attribute__((deprecated))
#else
#define MBEDTLS_DEPRECATED
#endif

/**
 * \brief          This function calculates the SHA-224 or SHA-256 checksum
 *                 of a buffer.
 *
 *                 The function allocates the context, performs the
 *                 calculation, and frees the context.
 *
 *                 The SHA-256 result is calculated as
 *                 output = SHA-256(input buffer).
 *
 * \deprecated     Superseded by mbedtls_sha256_ret() in 2.7.0.
 *
 * \param input    The buffer holding the data.
 * \param ilen     The length of the input data.
 * \param output   The SHA-224 or SHA-256 checksum result.
 * \param is224    Determines which function to use.
 *                 <ul><li>0: Use SHA-256.</li>
 *                 <li>1: Use SHA-224.</li></ul>
 */
MBEDTLS_DEPRECATED static inline void mbedtls_sha256(
                                                    const  char *input,
                                                    size_t ilen,
                                                     char output[32],
                                                    int is224 )
{
    mbedtls_sha256_ret( input, ilen, output, is224 );
}

#undef MBEDTLS_DEPRECATED
#endif /* !MBEDTLS_DEPRECATED_REMOVED */

/**
 * \brief          The SHA-224 and SHA-256 checkup routine.
 *
 * \return         \c 0 on success, or \c 1 on failure.
 */
int mbedtls_sha256_self_test( int verbose );

#ifdef __cplusplus
}
#endif

#endif /* mbedtls_sha256.h */
