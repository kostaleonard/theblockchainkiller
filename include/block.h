/**
 * @brief Defines a block in the blockchain.
 */

#ifndef INCLUDE_BLOCK_H_
#define INCLUDE_BLOCK_H_

#include <stdint.h>
#include <sys/time.h>
#include "include/transaction.h"
#include "include/hash.h"
#include "include/return_codes.h"

typedef struct block_t {
    uint32_t index;
    struct timespec created_at; //TODO add timezone aware datetimes
    uint32_t num_transactions;
    transaction_t **transactions;
    uint32_t proof_of_work;
    sha_256_t previous_block_hash;
} block_t;

typedef struct blockchain_t {
    uint32_t num_blocks;
    block_t **blocks;
} blockchain_t;

//TODO docstring
block_t *create_block(
    uint32_t index,
    uint32_t num_transactions,
    transaction_t **transactions,
    uint32_t proof_of_work,
    sha_256_t previous_block_hash
);

//TODO docstring
return_code_t destroy_block(block_t *block);

//TODO docstring
blockchain_t *create_blockchain();

//TODO docstring
return_code_t destroy_blockchain(blockchain_t *blockchain);

#endif  // INCLUDE_BLOCK_H_
