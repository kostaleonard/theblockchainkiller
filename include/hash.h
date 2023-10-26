/**
 * @brief Defines hash data types and operations.
 */

#ifndef INCLUDE_HASH_H_
#define INCLUDE_HASH_H_

#include <stdint.h>
#include <openssl/sha.h>

typedef struct sha_256_t {
    unsigned char digest[SHA256_DIGEST_LENGTH];
} sha_256_t;

/**
 * @brief Prints the hash value.
 */
void hash_print(sha_256_t *hash);

#endif  // INCLUDE_HASH_H_
