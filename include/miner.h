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
 * @param sync A synchronized blockchain on which to mine. This function will
 * generate new blocks on this blockchain. Periodically, this function will
 * check the version number to see if the networking thread has received a
 * longer blockchain. If so, this function will begin mining on the new
 * blockchain and free all memory associated with the old blockchain.
 * @param miner_public_key The public key with which to mine blocks. This
 * function uses it to create the minting transaction.
 * @param miner_private_key The private key with which to mine blocks. This
 * function uses it to digitally sign the minting transaction.
 * @param outfile If not NULL, this function will save the blockchain to this
 * filename every time it mines a new block. If NULL, this function will only
 * keep the blockchain in memory. Unless you are just testing, you should
 * provide this argument. Otherwise there is no local record of your mining and
 * you may lose all the coin you have mined thus far.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t mine_blocks(
    synchronized_blockchain_t *sync,
    ssh_key_t *miner_public_key,
    ssh_key_t *miner_private_key,
    char *outfile
);

#endif  // INCLUDE_MINER_H_
