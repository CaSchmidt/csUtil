#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int siphash(const void *in, const size_t inlen, const void *k, uint8_t *out,
            const size_t outlen,
            const int cROUNDS, const int dROUNDS);

#ifdef __cplusplus
}
#endif
