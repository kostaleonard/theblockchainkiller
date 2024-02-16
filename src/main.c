/**
 * @brief Runs the app.
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "include/base64.h"
#include "include/blockchain.h"
#include "include/block.h"
#include "include/transaction.h"

#define NUM_LEADING_ZERO_BYTES_IN_BLOCK_HASH 3
#define PRIVATE_KEY_ENVIRONMENT_VARIABLE "THEBLOCKCHAINKILLER_PRIVATE_KEY"
#define PUBLIC_KEY_ENVIRONMENT_VARIABLE "THEBLOCKCHAINKILLER_PUBLIC_KEY"

return_code_t mine_blocks(
    blockchain_t *blockchain,
    ssh_key_t *miner_public_key,
    ssh_key_t *miner_private_key,
    char *outfile) {
    return_code_t return_code = SUCCESS;
    if (NULL == blockchain ||
        NULL == miner_public_key ||
        NULL == miner_private_key) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    while (true) {
        // TODO verify blockchain here just before mining a new block
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
        return_code = transaction_create(
            &mint_coin_transaction,
            &sender_public_key_for_minting,
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
            if (NULL != outfile) {
                blockchain_write_to_file(blockchain, outfile);
            }
        }
    }
end:
    return return_code;
}

void print_usage_statement(char *program_name) {
    if (NULL == program_name) {
        goto end;
    }
    fprintf(
        stderr,
        "Usage: %s "
        "-p <private_key_file_base64_encoded_contents> "
        "-k <public_key_file_base64_encoded_contents>\n",
        program_name);
    fprintf(
        stderr,
        "Or supply keys as environment variables %s and %s\n",
        PRIVATE_KEY_ENVIRONMENT_VARIABLE,
        PUBLIC_KEY_ENVIRONMENT_VARIABLE);
end:
}

int main(int argc, char **argv) {
    return_code_t return_code = SUCCESS;
    blockchain_t *blockchain = NULL;
    block_t *genesis_block = NULL;
    char *ssh_private_key_contents_base64 = NULL;
    char *ssh_public_key_contents_base64 = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "p:k:")) != -1) {
        switch (opt) {
            case 'p':
                printf("Using private key from argv\n");
                ssh_private_key_contents_base64 = optarg;
                break;
            case 'k':
                printf("Using public key from argv\n");
                ssh_public_key_contents_base64 = optarg;
                break;
            default:
                print_usage_statement(argv[0]);
                return_code = FAILRE_INVALID_COMMAND_LINE_ARGS;
                goto end;
        }
    }
    if (NULL == ssh_private_key_contents_base64) {
        printf(
            "No private key found in argv, searching env for %s\n",
            PRIVATE_KEY_ENVIRONMENT_VARIABLE);
        ssh_private_key_contents_base64 = getenv(
            PRIVATE_KEY_ENVIRONMENT_VARIABLE);
    }
    if (NULL == ssh_public_key_contents_base64) {
        printf(
            "No public key found in argv, searching env for %s\n",
            PUBLIC_KEY_ENVIRONMENT_VARIABLE);
        ssh_public_key_contents_base64 = getenv(
            PUBLIC_KEY_ENVIRONMENT_VARIABLE);
    }
    if (NULL == ssh_private_key_contents_base64 ||
        NULL == ssh_public_key_contents_base64) {
        print_usage_statement(argv[0]);
        return_code = FAILRE_INVALID_COMMAND_LINE_ARGS;
        goto end;
    }
    ssh_key_t miner_public_key = {0};
    size_t public_key_decoded_length =
        (size_t)ceil(strlen(ssh_public_key_contents_base64) * 3 / 4) + 1;
    if (public_key_decoded_length > sizeof(miner_public_key.bytes)) {
        printf("Public key is too long\n");
        return_code = FAILRE_INVALID_COMMAND_LINE_ARGS;
        goto end;
    }
    return_code = base64_decode(
        ssh_public_key_contents_base64,
        strlen(ssh_public_key_contents_base64),
        miner_public_key.bytes);
    if (SUCCESS != return_code) {
        goto end;
    }
    ssh_key_t miner_private_key = {0};
    size_t private_key_decoded_length =
        (size_t)ceil(strlen(ssh_private_key_contents_base64) * 3 / 4) + 1;
    if (private_key_decoded_length > sizeof(miner_private_key.bytes)) {
        printf("Private key is too long\n");
        return_code = FAILRE_INVALID_COMMAND_LINE_ARGS;
        goto end;
    }
    return_code = base64_decode(
        ssh_private_key_contents_base64,
        strlen(ssh_private_key_contents_base64),
        miner_private_key.bytes);
    if (SUCCESS != return_code) {
        goto end;
    }
    printf("Using public key: %s\n", miner_public_key.bytes);
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
    return_code = mine_blocks(
        blockchain, &miner_public_key, &miner_private_key, "blockchain");
end:
    blockchain_destroy(blockchain);
    exit(return_code);
}
