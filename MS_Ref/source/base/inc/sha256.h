#ifndef _SHA256_H
#define _SHA256_H

//#include "osi_api.h"
#include "anti.h"

typedef struct {
	uint32_t total[2];
	uint32_t state[8];
	uint8_t buffer[64];
} sha256_context;

void sha256_starts(sha256_context * ctx);

void sha256_update(sha256_context *ctx, const uint8_t *input, uint32_t length);

void sha256_finish(sha256_context * ctx, uint8_t digest[32]);

void sha256_encrypt(const uint8_t *input, uint32_t ilen, uint8_t *output);

#endif /* _SHA256_H */
