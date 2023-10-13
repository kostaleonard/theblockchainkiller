/**
 * @brief Defines the blockchain.
 * 
 * See https://github.com/dvf/blockchain for a helpful Python blockchain
 * implementation.
 */

#ifndef INCLUDE_BLOCKCHAIN_H_
#define INCLUDE_BLOCKCHAIN_H_

#include <stdint.h>
#include <sys/time.h>
#include "include/block.h"
#include "include/return_codes.h"

/**
 * @brief Represents a blockchain.
 * 
 * @param block_list The list of blocks in the chain.
 */
typedef struct blockchain_t {
    linked_list_t *block_list;
} blockchain_t;

/**
 * @brief Fills blockchain with a pointer to the newly allocated blockchain.
 * 
 * @param blockchain A pointer to fill with the blockchain's address.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_create(blockchain_t **blockchain);

/**
 * @brief Frees all memory associated with a blockchain.
 * 
 * @param blockchain The blockchain to destroy.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_destroy(blockchain_t *blockchain);

/**
 * @brief Appends a block to the blockchain.
 * 
 * @param blockchain The blockchain.
 * @param block The block to add.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_add_block(blockchain_t *blockchain, block_t *block);

/**
 * @brief Fills block's proof_of_work with a number that produces a valid hash.
 * 
 * @param blockchain The blockchain.
 * @param block The block for which to calculate a proof of work.
 * @return return_code_t A return code indicating success or failure.
 */
//return_code_t blockchain_mine_block(blockchain_t *blockchain, block_t *block);
// TODO add blockchain_mine_block

//TODO add blockchain_is_valid_proof(blockchain_t *, block_t *)

/**
 * @brief Prints the blockchain.
 */
void blockchain_print(blockchain_t *blockchain);

#endif  // INCLUDE_BLOCKCHAIN_H_
