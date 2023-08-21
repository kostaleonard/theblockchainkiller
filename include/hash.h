/**
 * @brief Defines hash data types and operations.
 */

#ifndef INCLUDE_HASH_H_
#define INCLUDE_HASH_H_

#include <stdint.h>

typedef struct sha_256_t {
    uint8_t data[32];
} sha_256_t;

#endif  // INCLUDE_HASH_H_
