#include <stdio.h>
#include "include/hash.h"

void hash_print(sha_256_t *hash) {
    if (NULL == hash) {
        return;
    }
    for (size_t idx = 0; idx < sizeof(hash->digest); idx++) {
        printf("%02x", hash->digest[idx]);
    }
    printf("\n");
}