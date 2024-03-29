/**
 * @brief Defines a block in the blockchain.
 */

#ifndef INCLUDE_BLOCK_H_
#define INCLUDE_BLOCK_H_
#define GENESIS_BLOCK_PROOF_OF_WORK 2017

#include <stdint.h>
#include <sys/time.h>
#include "include/linked_list.h"
#include "include/hash.h"
#include "include/return_codes.h"

/**
 * @brief Represents a block in the blockchain.
 * 
 * @param created_at The datetime at which the user created this block.
 * @param transaction_list A list of the transactions this block contains.
 * @param proof_of_work A number such that the hash of the block_t contains
 * some number of leading zeros. It has no meaning other than as part of the
 * hash.
 * @param previous_block_hash The hash of the previous block.
 */
typedef struct block_t {
    time_t created_at;
    linked_list_t *transaction_list;
    uint64_t proof_of_work;
    sha_256_t previous_block_hash;
} block_t;

/**
 * @brief Fills block with a pointer to the newly allocated block.
 * 
 * @param block The pointer to fill with the new block.
 * @param transaction_list A list of the transactions this block contains.
 * @param proof_of_work A number such that the hash of the block_t contains
 * some number of leading zeros. It has no meaning other than as part of the
 * hash.
 * @param previous_block_hash The hash of the previous block.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t block_create(
    block_t **block,
    linked_list_t *transaction_list,
    uint64_t proof_of_work,
    sha_256_t previous_block_hash
);

/**
 * @brief Fills block with a pointer to the newly allocated genesis block.
 * 
 * The genesis block is the first block in the blockchain. Since there is no
 * previous block, the genesis block contains special values for the previous
 * block hash and proof of work.
 * 
 * @param block The pointer to fill with the new genesis block.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t block_create_genesis_block(block_t **block);

/**
 * @brief Frees all memory associated with the block.
 * 
 * @param block The block to destroy.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t block_destroy(block_t *block);

/**
 * @brief Fills hash with the block's hash.
 * 
 * @param block The block.
 * @param hash A pointer to fill with the block's hash.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t block_hash(block_t *block, sha_256_t *hash);

#endif  // INCLUDE_BLOCK_H_
