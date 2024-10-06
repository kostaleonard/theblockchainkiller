#include <stdio.h>
#include <stdlib.h>
#include "include/transaction.h"
#include "include/miner.h"

return_code_t *mine_blocks(mine_blocks_args_t *args) {
    return_code_t return_code = SUCCESS;
    if (NULL == args) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    synchronized_blockchain_t *sync = args->sync;
    ssh_key_t *miner_public_key = args->miner_public_key;
    ssh_key_t *miner_private_key = args->miner_private_key;
    bool print_progress = args->print_progress;
    char *outfile = args->outfile;
    atomic_bool *should_stop = args->should_stop;
    bool *exit_ready = args->exit_ready;
    pthread_cond_t *exit_ready_cond = &args->exit_ready_cond;
    pthread_mutex_t *exit_ready_mutex = &args->exit_ready_mutex;
    // TODO add logic for replacing the blockchain when it's been changed
    if (0 != pthread_mutex_lock(&sync->mutex)) {
        return_code = FAILURE_PTHREAD_FUNCTION;
        goto end;
    }
    blockchain_t *blockchain = sync->blockchain;
    if (0 != pthread_mutex_unlock(&sync->mutex)) {
        return_code = FAILURE_PTHREAD_FUNCTION;
        goto end;
    }
    while (!*should_stop) {
        bool is_valid_blockchain = false;
        block_t *first_invalid_block = NULL;
        return_code = blockchain_verify(
            blockchain, &is_valid_blockchain, &first_invalid_block);
        if (SUCCESS != return_code) {
            goto end;
        }
        if (!is_valid_blockchain) {
            printf(
                "Invalid blockchain detected. First invalid block follows.\n");
            printf("Created at: %"PRIu64"\n", first_invalid_block->created_at);
            printf(
                "Proof of work: %"PRIu64"\n",
                first_invalid_block->proof_of_work);
            printf("Block hash: ");
            hash_print(&first_invalid_block->previous_block_hash);
            return_code = FAILURE_INVALID_BLOCKCHAIN;
            goto end;
        }
        node_t *node = NULL;
        return_code = linked_list_get_last(blockchain->block_list, &node);
        if (SUCCESS != return_code) {
            goto end;
        }
        block_t *previous_block = (block_t *)node->data;
        sha_256_t previous_block_hash = {0};
        return_code = block_hash(previous_block, &previous_block_hash);
        if (SUCCESS != return_code) {
            goto end;
        }
        linked_list_t *transaction_list = NULL;
        return_code = linked_list_create(&transaction_list, free, NULL);
        if (SUCCESS != return_code) {
            goto end;
        }
        transaction_t *mint_coin_transaction = NULL;
        return_code = transaction_create(
            &mint_coin_transaction,
            miner_public_key,
            miner_public_key,
            AMOUNT_GENERATED_DURING_MINTING,
            miner_private_key);
        if (SUCCESS != return_code) {
            linked_list_destroy(transaction_list);
            goto end;
        }
        return_code = linked_list_prepend(
            transaction_list, mint_coin_transaction);
        if (SUCCESS != return_code) {
            transaction_destroy(mint_coin_transaction);
            linked_list_destroy(transaction_list);
            goto end;
        }
        block_t *next_block = NULL;
        return_code = block_create(
            &next_block, transaction_list, 0, previous_block_hash);
        if (SUCCESS != return_code) {
            linked_list_destroy(transaction_list);
            goto end;
        }
        // TODO synchronized_blockchain_mine_block?
        return_code = blockchain_mine_block(
            blockchain, next_block, print_progress, should_stop);
        if (SUCCESS != return_code) {
            block_destroy(next_block);
            if (FAILURE_COULD_NOT_FIND_VALID_PROOF_OF_WORK == return_code) {
                printf("\nCouldn't find valid proof of work for block; "
                       "generating new block\n");
            } else if (FAILURE_STOPPED_EARLY != return_code) {
                goto end;
            }
        } else {
            return_code = blockchain_add_block(blockchain, next_block);
            if (SUCCESS != return_code) {
                block_destroy(next_block);
                goto end;
            }
            if (print_progress) {
                blockchain_print(blockchain);
            }
            if (NULL != outfile) {
                blockchain_write_to_file(blockchain, outfile);
            }
        }
    }
    pthread_mutex_lock(exit_ready_mutex);
    *exit_ready = true;
    pthread_cond_signal(exit_ready_cond);
    pthread_mutex_unlock(exit_ready_mutex);
end:
    return_code_t *return_code_ptr = malloc(sizeof(return_code_t));
    *return_code_ptr = return_code;
    return return_code_ptr;
}

void *mine_blocks_pthread_wrapper(void *args) {
    mine_blocks_args_t *mine_blocks_args = (mine_blocks_args_t *)args;
    return_code_t *return_code_ptr = mine_blocks(mine_blocks_args);
    return (void *)return_code_ptr;
}
