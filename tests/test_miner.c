#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/base64.h"
#include "include/blockchain.h"
#include "include/miner.h"
#include "tests/test_cryptography.h"
#include "tests/test_miner.h"

#define NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH 2

void test_mine_blocks_exits_when_should_stop_is_set() {
    // TODO timeout of 1 second

    blockchain_t *blockchain = NULL;
    return_code_t return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    assert_true(SUCCESS == return_code);
    block_t *genesis_block;
    return_code = block_create_genesis_block(&genesis_block);
    assert_true(SUCCESS == return_code);
    // Manually set created_at for consistent hashes.
    genesis_block->created_at = 0;
    return_code = blockchain_add_block(blockchain, genesis_block);
    assert_true(SUCCESS == return_code);
    synchronized_blockchain_t *sync = NULL;
    return_code = synchronized_blockchain_create(&sync, blockchain);
    assert_true(SUCCESS == return_code);
    atomic_bool should_stop = false;
    ssh_key_t miner_public_key = {0};
    ssh_key_t miner_private_key = {0};
    mine_blocks_args_t args = {0};
    args.sync = sync;
    char *ssh_public_key_contents_base64 = getenv(
        TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        miner_public_key.bytes);
    char *ssh_private_key_contents_base64 = getenv(
        TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    return_code = base64_decode(
        ssh_private_key_contents_base64,
        strlen(ssh_private_key_contents_base64),
        miner_private_key.bytes);
    bool exit_ready = false;
    pthread_cond_init(&args.exit_ready_cond, NULL);
    pthread_mutex_init(&args.exit_ready_mutex, NULL);
    args.miner_public_key = &miner_public_key;
    args.miner_private_key = &miner_private_key;
    args.print_progress = false;
    args.outfile = NULL;
    args.should_stop = &should_stop;
    args.exit_ready = &exit_ready;
    pthread_t thread;
    pthread_create(&thread, NULL, mine_blocks_pthread_wrapper, &args);
    *args.should_stop = true;
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    // One second timeout.
    // TODO this timeout code should probably be in a helper function
    ts.tv_sec += 1;
    // TODO the pthread functions can all fail--need error handling
    pthread_mutex_lock(&args.exit_ready_mutex);
    while (!*args.exit_ready) {
        int result = pthread_cond_timedwait(
            &args.exit_ready_cond, &args.exit_ready_mutex, &ts);
        if (ETIMEDOUT == result) {
            assert_true(false);
        }
    }
    pthread_mutex_unlock(&args.exit_ready_mutex);
    void *retval = NULL;
    pthread_join(thread, &retval);
    return_code_t *return_code_ptr = (return_code_t *)retval;
    assert_true(NULL != return_code_ptr);
    return_code = *return_code_ptr;
    assert_true(SUCCESS == return_code);
    free(return_code_ptr);
    pthread_cond_destroy(&args.exit_ready_cond);
    pthread_mutex_destroy(&args.exit_ready_mutex);
    synchronized_blockchain_destroy(sync);
}
