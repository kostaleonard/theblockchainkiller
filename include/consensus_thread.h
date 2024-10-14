/**
 * @brief Contains functions for peerwise consensus over a network.
 */

#ifndef INCLUDE_CONSENSUS_H_
#define INCLUDE_CONSENSUS_H_

#include <stdatomic.h>
#include "include/blockchain.h"
#include "include/return_codes.h"

// TODO docstrings

typedef struct run_consensus_peer_args_t {
    synchronized_blockchain_t *sync;
    struct sockaddr_in6 peer_discovery_node_addr;
    atomic_bool *should_stop;
    bool *exit_ready;
    pthread_cond_t exit_ready_cond;
    pthread_mutex_t exit_ready_mutex;
} run_consensus_peer_args_t;


// TODO what this function does: finds peers, share blockchains, accepts transactions
return_code_t *run_consensus_peer(run_consensus_peer_args_t *args);

#endif  // INCLUDE_CONSENSUS_H_
