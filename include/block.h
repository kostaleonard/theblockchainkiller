/**
 * @brief Defines a block in the blockchain.
 */

#ifndef INCLUDE_BLOCK_H_
#define INCLUDE_BLOCK_H_

#include <stdint.h>
#include <sys/time.h>
#include "include/linked_list.h"
#include "include/hash.h"
#include "include/return_codes.h"

typedef struct block_t {
    uint32_t index;
    struct timespec created_at; //TODO add timezone aware datetimes
    linked_list_t *transaction_list;
    uint32_t proof_of_work;
    sha_256_t previous_block_hash;
} block_t;

//TODO docstring
block_t *block_create(
    uint32_t index,
    linked_list_t *transaction_list,
    uint32_t proof_of_work,
    sha_256_t previous_block_hash
);

//TODO docstring
return_code_t block_destroy(block_t *block);

#endif  // INCLUDE_BLOCK_H_
