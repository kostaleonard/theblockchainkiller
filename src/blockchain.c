#include <stdio.h>
#include <stdlib.h>
#include "include/block.h"
#include "include/blockchain.h"
#include "include/linked_list.h"
#include "include/return_codes.h"

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RESET "\x1b[0m"

return_code_t blockchain_create(
    blockchain_t **blockchain,
    size_t num_leading_zero_bytes_required_in_block_hash
) {
    return_code_t return_code = SUCCESS;
    if (NULL == blockchain) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    blockchain_t *new_blockchain = malloc(sizeof(blockchain_t));
    if (NULL == new_blockchain) {
        return_code = FAILURE_COULD_NOT_MALLOC;
        goto end;
    }
    linked_list_t *block_list = NULL;
    return_code = linked_list_create(
        &block_list,
        (free_function_t *)block_destroy,
        NULL);
    if (SUCCESS != return_code) {
        free(new_blockchain);
        goto end;
    }
    new_blockchain->block_list = block_list;
    new_blockchain->num_leading_zero_bytes_required_in_block_hash =
        num_leading_zero_bytes_required_in_block_hash;
    *blockchain = new_blockchain;
end:
    return return_code;
}

return_code_t blockchain_destroy(blockchain_t *blockchain) {
    return_code_t return_code = SUCCESS;
    if (NULL == blockchain) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    return_code = linked_list_destroy(blockchain->block_list);
    free(blockchain);
end:
    return return_code;
}

return_code_t blockchain_add_block(blockchain_t *blockchain, block_t *block) {
    return_code_t return_code = SUCCESS;
    if (NULL == blockchain || NULL == block) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    return_code = linked_list_append(blockchain->block_list, block);
end:
    return return_code;
}

return_code_t blockchain_is_valid_block_hash(
    blockchain_t *blockchain,
    sha_256_t block_hash,
    bool *is_valid_block_hash
) {
    return_code_t return_code = SUCCESS;
    if (NULL == blockchain || NULL == is_valid_block_hash) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    bool is_valid = true;
    for (size_t idx = 0;
        idx < blockchain->num_leading_zero_bytes_required_in_block_hash;
        idx++) {
        if (block_hash.digest[idx] != 0) {
            is_valid = false;
            break;
        }
    }
    *is_valid_block_hash = is_valid;
end:
    return return_code;
}

return_code_t blockchain_mine_block(
    blockchain_t *blockchain,
    block_t *block,
    bool print_progress
) {
    return_code_t return_code = SUCCESS;
    if (NULL == blockchain || NULL == block) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    size_t best_leading_zeroes = 0;
    size_t print_frequency = 20000;
    sha_256_t hash = {0};
    bool is_valid_block_hash = false;
    for (uint64_t new_proof = 0; new_proof < UINT64_MAX; new_proof++) {
        block->proof_of_work = new_proof;
        return_code = block_hash(block, &hash);
        if (SUCCESS != return_code) {
            goto end;
        }
        if (print_progress) {
            size_t num_zeroes = 0;
            for (size_t idx = 0; idx < sizeof(hash.digest); idx++) {
                unsigned char upper_nybble = hash.digest[idx] >> 4;
                if (upper_nybble != 0) {
                    break;
                }
                num_zeroes++;
                if (hash.digest[idx] != 0) {
                    break;
                }
                num_zeroes++;
            }
            bool print_this_iteration = new_proof % print_frequency == 0;
            if (num_zeroes > best_leading_zeroes) {
                best_leading_zeroes = num_zeroes;
                print_this_iteration = true;
            }
            if (print_this_iteration) {
                // Remove the previous block hash from output.
                printf("\rMining LeoCoin block: ");
                // Print the best number of leading zeroes.
                for (size_t idx = 0; idx < best_leading_zeroes; idx++) {
                    printf("0");
                }
                // Add the part of the new hash following the best number of
                // leading zeroes. It's not the accurate hash, but it does give
                // an idea of progress.
                for (size_t idx = best_leading_zeroes;
                    idx < 2 * sizeof(hash.digest);
                    idx++) {
                    size_t hash_idx = idx / 2;
                    unsigned char nybble = 0;
                    if (idx % 2 == 0) {
                        nybble = hash.digest[hash_idx] >> 4;
                    } else {
                        nybble = hash.digest[hash_idx] & 0x0f;
                    }
                    // To give a better sense of progress, don't allow a
                    // non-leading zero to follow the best number of leading
                    // zeroes.
                    if (idx == best_leading_zeroes && 0 == nybble) {
                        nybble += 1;
                    }
                    printf("%01x", nybble);
                }
                fflush(stdout);
            }
        }
        return_code = blockchain_is_valid_block_hash(
            blockchain, hash, &is_valid_block_hash);
        if (SUCCESS != return_code) {
            goto end;
        }
        if (is_valid_block_hash) {
            if (print_progress) {
                printf("\rMining LeoCoin block: %s", ANSI_COLOR_GREEN);
                for (size_t idx = 0; idx < sizeof(hash.digest); idx++) {
                    printf("%02x", hash.digest[idx]);
                }
                printf("%s\n", ANSI_COLOR_RESET);
            }
            goto end;
        }
    }
    return_code = FAILURE_COULD_NOT_FIND_VALID_PROOF_OF_WORK;
end:
    return return_code;
}

void blockchain_print(blockchain_t *blockchain) {
    if (NULL == blockchain) {
        return;
    }
    for (node_t *node = blockchain->block_list->head;
        NULL != node;
        node = node->next) {
        block_t *block = (block_t *)node->data;
        printf("%"PRIu64"->", block->proof_of_work);
    }
    printf("\n");
}

return_code_t blockchain_serialize(
    blockchain_t *blockchain,
    unsigned char **buffer,
    uint64_t *buffer_size
) {
    return_code_t return_code = SUCCESS;
    if (NULL == blockchain || NULL == buffer || NULL == buffer_size) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    // TODO
end:
    return return_code;
}
