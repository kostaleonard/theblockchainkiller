/**
 * @brief Runs the app.
 */

#include <stdio.h>
#include <stdlib.h>
#include "include/blockchain.h"
#include "include/block.h"
#include "include/transaction.h"

#define NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH 3

// TODO test
return_code_t mine_blocks(blockchain_t *blockchain) {
    return_code_t return_code = SUCCESS;
    while (true) {
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
        // TODO the recipient should be the miner's ID--could take from the command line to start
        uint32_t recipient_id = 1;
        return_code = transaction_create(
            &mint_coin_transaction,
            SENDER_ID_FOR_MINTING,
            recipient_id,
            AMOUNT_GENERATED_DURING_MINTING);
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
        return_code = blockchain_mine_block(blockchain, next_block, true);
        if (SUCCESS != return_code) {
            block_destroy(next_block);
            if (FAILURE_COULD_NOT_FIND_VALID_PROOF_OF_WORK == return_code) {
                printf("\nCouldn't find valid proof of work for block; "
                       "generating new block\n");
            } else {
                goto end;
            }
        } else {
            return_code = blockchain_add_block(blockchain, next_block);
            if (SUCCESS != return_code) {
                block_destroy(next_block);
                goto end;
            }
            blockchain_print(blockchain);
        }
    }
end:
    return return_code;
}

int main(int argc, char **argv) {
    return_code_t return_code = SUCCESS;
    blockchain_t *blockchain = NULL;
    block_t *genesis_block = NULL;
    return_code = blockchain_create(
        &blockchain, NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH);
    if (SUCCESS != return_code) {
        goto end;
    }
    return_code = block_create_genesis_block(&genesis_block);
    if (SUCCESS != return_code) {
        goto end;
    }
    return_code = blockchain_add_block(blockchain, genesis_block);
    if (SUCCESS != return_code) {
        block_destroy(genesis_block);
        goto end;
    }
    blockchain_print(blockchain);
    sha_256_t genesis_block_hash = {0};
    return_code = block_hash(genesis_block, &genesis_block_hash);
    if (SUCCESS != return_code) {
        goto end;
    }
    return_code = mine_blocks(blockchain);
end:
    blockchain_destroy(blockchain);
    return return_code;
}
