/**
 * @brief Contains functions for mining and consensus.
 */

#ifndef INCLUDE_MINER_H_
#define INCLUDE_MINER_H_

#include "include/return_codes.h"
#include "include/blockchain.h"
#include "include/cryptography.h"

/**
 * @brief Contains the arguments to the mine_blocks function.
 * 
 * This struct packages arguments so that users can call mine_blocks in
 * pthread_create.
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
 * @param should_stop Setting this flag while the function is running requests
 * that the function terminate gracefully. Users should expect the function to
 * terminate in a timely manner (on the order of seconds), but not necessarily
 * immediately.
 * @param print_progress If true, display progress on the screen.
 * @param outfile If not NULL, this function will save the blockchain to this
 * filename every time it mines a new block. If NULL, this function will only
 * keep the blockchain in memory. Unless you are just testing, you should
 * provide this argument. Otherwise there is no local record of your mining and
 * you may lose all the coin you have mined thus far.
 * 
 */
typedef struct mine_blocks_args_t {
    synchronized_blockchain_t *sync;
    ssh_key_t *miner_public_key;
    ssh_key_t *miner_private_key;
    atomic_bool *should_stop;
    bool print_progress;
    char *outfile;
} mine_blocks_args_t;

/**
 * @brief Mines blocks continuously until interrupted.
 * 
 * @param args Contains the function arguments. See mine_blocks_args for
 * details.
 * @return return_code_t A pointer to a return code indicating success or
 * failure. Callers must free.
 */
return_code_t *mine_blocks(mine_blocks_args_t *args);

#endif  // INCLUDE_MINER_H_
