/**
 * @brief Contains functions for mining and consensus.
 */

#ifndef INCLUDE_MINER_H_
#define INCLUDE_MINER_H_

#include "include/return_codes.h"
#include "include/blockchain.h"
#include "include/cryptography.h"

/**
 * @brief Mines blocks continuously until interrupted.
 * 
 * @param blockchain 
 * @param miner_public_key 
 * @param miner_private_key 
 * @param outfile 
 * @return return_code_t 
 */
return_code_t mine_blocks(
    blockchain_t *blockchain, // TODO synchronized_blockchain_t
    ssh_key_t *miner_public_key,
    ssh_key_t *miner_private_key,
    char *outfile);

#endif  // INCLUDE_MINER_H_
