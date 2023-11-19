/**
 * @brief Runs the app.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "include/blockchain.h"
#include "include/block.h"
#include "include/transaction.h"

#define NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH 3
#define PRIVATE_KEY_ENVIRONMENT_VARIABLE "THEBLOCKCHAINKILLER_PRIVATE_KEY"
#define PUBLIC_KEY_ENVIRONMENT_VARIABLE "THEBLOCKCHAINKILLER_PUBLIC_KEY"

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
        ssh_key_t sender_public_key_for_minting = {0};
        // TODO use the SSH key supplied at startup
        ssh_key_t miner_public_key = {0};
        return_code = transaction_create(
            &mint_coin_transaction,
            &sender_public_key_for_minting,
            &miner_public_key,
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

void print_usage_statement(char *program_name) {
    fprintf(
        stderr,
        "Usage: %s "
        "-p <private_key_file_contents> "
        "-k <public_key_file_contents>\n",
        program_name);
    fprintf(
        stderr,
        "Or supply keys as environment variables %s and %s\n",
        PRIVATE_KEY_ENVIRONMENT_VARIABLE,
        PUBLIC_KEY_ENVIRONMENT_VARIABLE);
}

int main(int argc, char **argv) {
    // TODO redo gif on README to show passing environment variables for keys
    // TODO update README to show running instructions
    // TODO detect when the user accidentally reversed their keys and gave a private key instead of a public key
    return_code_t return_code = SUCCESS;
    blockchain_t *blockchain = NULL;
    block_t *genesis_block = NULL;
    char *ssh_private_key_contents = NULL;
    char *ssh_public_key_contents = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "p:k:")) != -1) {
        switch (opt) {
            case 'p':
                printf("Using private key from argv\n");
                ssh_private_key_contents = optarg;
                break;
            case 'k':
                printf("Using public key from argv\n");
                ssh_public_key_contents = optarg;
                break;
            default:
                print_usage_statement(argv[0]);
                return_code = FAILRE_INVALID_COMMAND_LINE_ARGS;
                goto end;
        }
    }
    if (NULL == ssh_private_key_contents) {
        printf(
            "No private key found in argv, searching env for %s\n",
            PRIVATE_KEY_ENVIRONMENT_VARIABLE);
        ssh_private_key_contents = getenv(PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    }
    if (NULL == ssh_public_key_contents) {
        printf(
            "No public key found in argv, searching env for %s\n",
            PUBLIC_KEY_ENVIRONMENT_VARIABLE);
        ssh_public_key_contents = getenv(PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    }
    if (NULL == ssh_private_key_contents || NULL == ssh_public_key_contents) {
        print_usage_statement(argv[0]);
        return_code = FAILRE_INVALID_COMMAND_LINE_ARGS;
        goto end;
    }
    // TODO check if keys exceed maximum key length
    printf("Using public key: %s\n", ssh_public_key_contents);
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
    exit(return_code);
}
