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

typedef struct blockchain_t {
    linked_list_t *block_list;
} blockchain_t;

//TODO docstring
blockchain_t *blockchain_create();

//TODO docstring
return_code_t blockchain_destroy(blockchain_t *blockchain);

//TODO docstring
return_code_t blockchain_add_block(blockchain_t *blockchain, block_t *block);

//TODO docstring
void blockchain_print(blockchain_t *blockchain);

#endif  // INCLUDE_BLOCKCHAIN_H_
