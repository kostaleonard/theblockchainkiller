#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/block.h"
#include "include/blockchain.h"
#include "include/endian.h"
#include "include/hash.h"
#include "include/linked_list.h"
#include "include/return_codes.h"
#include "include/transaction.h"

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

return_code_t blockchain_verify(
    blockchain_t *blockchain,
    bool *is_valid_blockchain,
    block_t **first_invalid_block
) {
    return_code_t return_code = SUCCESS;
    if (NULL == blockchain || NULL == is_valid_blockchain) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    if (NULL == blockchain->block_list->head) {
        *is_valid_blockchain = true;
        goto end;
    }
    // Check the genesis block, which is unique.
    block_t *genesis_block = (block_t *)blockchain->block_list->head->data;
    bool genesis_block_transaction_list_is_empty = false;
    return_code = linked_list_is_empty(
        genesis_block->transaction_list,
        &genesis_block_transaction_list_is_empty);
    if (SUCCESS != return_code) {
        goto end;
    }
    sha_256_t empty_block_hash = {0};
    if (!genesis_block_transaction_list_is_empty ||
        genesis_block->proof_of_work != GENESIS_BLOCK_PROOF_OF_WORK ||
        0 != memcmp(
            &genesis_block->previous_block_hash,
            &empty_block_hash,
            sizeof(sha_256_t))) {
        *is_valid_blockchain = false;
        if (NULL != first_invalid_block) {
            *first_invalid_block = genesis_block;
        }
        goto end;
    }
    // TODO remove
    printf("Genesis block ok\n");
    sha_256_t genesis_block_hash = {0};
    block_hash(genesis_block, &genesis_block_hash);
    printf("Genesis block hash: ");
    hash_print(&genesis_block_hash);
    // Check the remaining blocks.
    //node_t *previous_node = blockchain->block_list->head; // TODO remove and change to for loop
    uint64_t num_blocks = 0;
    return_code = linked_list_length(blockchain->block_list, &num_blocks);
    if (SUCCESS != return_code) {
        goto end;
    }
    printf("Expected number of blocks: %lld\n", num_blocks);
    node_t *current_node = blockchain->block_list->head->next;
    while (NULL != current_node) {
        //block_t *previous_block = (block_t *)previous_node->data;
        block_t *current_block = (block_t *)current_node->data;
        char *time_string = ctime(&current_block->created_at);
        printf("%s", time_string);
        printf("Previous block hash: ");
        hash_print(&current_block->previous_block_hash);
        sha_256_t current_block_hash = {0};
        return_code = block_hash(current_block, &current_block_hash);
        if (SUCCESS != return_code) {
            printf("Hash failed\n");
            goto end;
        }
        printf("Current block hash: ");
        hash_print(&current_block_hash);
        if (0 != memcmp(
                &current_block_hash,
                &empty_block_hash,
                blockchain->num_leading_zero_bytes_required_in_block_hash)) {
            printf("invalid block\n");
            *is_valid_blockchain = false;
            if (NULL != first_invalid_block) {
                *first_invalid_block = current_block;
            }
            // TODO uncomment
            //goto end;
        }
        // TODO check that previous block hash is accurate

        // TODO check that all transactions in current block have valid signature


        //previous_node = current_node;
        current_node = current_node->next;
        printf("block ok\n");
    }
    *is_valid_blockchain = true;
end:
    return return_code;
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
    uint64_t num_blocks = 0;
    return_code = linked_list_length(blockchain->block_list, &num_blocks);
    if (SUCCESS != return_code) {
        goto end;
    }
    uint64_t size =
        sizeof(blockchain->num_leading_zero_bytes_required_in_block_hash) +
        sizeof(num_blocks);
    unsigned char *serialization_buffer = calloc(1, size);
    if (NULL == serialization_buffer) {
        return_code = FAILURE_COULD_NOT_MALLOC;
        goto end;
    }
    unsigned char *next_spot_in_buffer = serialization_buffer;
    *(uint64_t *)next_spot_in_buffer = htobe64(
        blockchain->num_leading_zero_bytes_required_in_block_hash);
    next_spot_in_buffer += sizeof(
        blockchain->num_leading_zero_bytes_required_in_block_hash);
    *(uint64_t *)next_spot_in_buffer = htobe64(num_blocks);
    next_spot_in_buffer += sizeof(num_blocks);
    for (node_t *block_node = blockchain->block_list->head;
        NULL != block_node;
        block_node = block_node->next) {
        block_t *block = (block_t *)block_node->data;
        uint64_t num_transactions_in_block = 0;
        return_code = linked_list_length(
            block->transaction_list, &num_transactions_in_block);
        if (SUCCESS != return_code) {
            free(serialization_buffer);
            goto end;
        }
        uint64_t next_spot_in_buffer_offset = size;
        size += sizeof(block->created_at);
        size += sizeof(block->previous_block_hash);
        size += sizeof(block->proof_of_work);
        size += sizeof(num_transactions_in_block);
        serialization_buffer = realloc(serialization_buffer, size);
        if (NULL == serialization_buffer) {
            goto end;
        }
        // When we realloc, serialization_buffer may move. We need to use an
        // offset so we can get the next memory location to write.
        next_spot_in_buffer = serialization_buffer + next_spot_in_buffer_offset;
        *(uint64_t *)next_spot_in_buffer = htobe64(block->created_at);
        next_spot_in_buffer += sizeof(block->created_at);
        for (size_t idx = 0; idx < sizeof(block->previous_block_hash); idx++) {
            *next_spot_in_buffer = block->previous_block_hash.digest[idx];
            next_spot_in_buffer++;
        }
        *(uint64_t *)next_spot_in_buffer = htobe64(block->proof_of_work);
        next_spot_in_buffer += sizeof(block->proof_of_work);
        *(uint64_t *)next_spot_in_buffer = htobe64(num_transactions_in_block);
        next_spot_in_buffer += sizeof(num_transactions_in_block);
        for (node_t *transaction_node = block->transaction_list->head;
            NULL != transaction_node;
            transaction_node = transaction_node->next) {
            transaction_t *transaction = (transaction_t *)
                transaction_node->data;
            next_spot_in_buffer_offset = size;
            size += sizeof(transaction->created_at);
            size += sizeof(transaction->sender_public_key);
            size += sizeof(transaction->recipient_public_key);
            size += sizeof(transaction->amount);
            size += sizeof(transaction->sender_signature.length);
            size += sizeof(transaction->sender_signature.bytes);
            serialization_buffer = realloc(serialization_buffer, size);
            if (NULL == serialization_buffer) {
                goto end;
            }
            // When we realloc, serialization_buffer may move. We need to use an
            // offset so we can get the next memory location to write.
            next_spot_in_buffer =
                serialization_buffer + next_spot_in_buffer_offset;
            *(uint64_t *)next_spot_in_buffer = htobe64(transaction->created_at);
            next_spot_in_buffer += sizeof(transaction->created_at);
            for (size_t idx = 0;
                 idx < sizeof(transaction->sender_public_key);
                 idx++) {
                *next_spot_in_buffer =
                    transaction->sender_public_key.bytes[idx];
                next_spot_in_buffer++;
            }
            for (size_t idx = 0;
                 idx < sizeof(transaction->recipient_public_key);
                 idx++) {
                *next_spot_in_buffer =
                    transaction->recipient_public_key.bytes[idx];
                next_spot_in_buffer++;
            }
            *(uint64_t *)next_spot_in_buffer = htobe64(transaction->amount);
            next_spot_in_buffer += sizeof(transaction->amount);
            *(uint64_t *)next_spot_in_buffer = htobe64(
                transaction->sender_signature.length);
            next_spot_in_buffer += sizeof(transaction->sender_signature.length);
            for (size_t idx = 0;
                idx < sizeof(transaction->sender_signature.bytes);
                idx++) {
                *next_spot_in_buffer = transaction->sender_signature.bytes[idx];
                next_spot_in_buffer++;
            }
        }
    }
    *buffer = serialization_buffer;
    *buffer_size = size;
end:
    return return_code;
}

return_code_t blockchain_deserialize(
    blockchain_t **blockchain,
    unsigned char *buffer,
    uint64_t buffer_size
) {
    return_code_t return_code = SUCCESS;
    if (NULL == blockchain || NULL == buffer) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    unsigned char *next_spot_in_buffer = buffer;
    ptrdiff_t total_read_size = next_spot_in_buffer + sizeof(uint64_t) - buffer;
    if (total_read_size > buffer_size) {
        return_code = FAILURE_BUFFER_TOO_SMALL;
        goto end;
    }
    uint64_t num_leading_zero_bytes_required_in_block_hash = betoh64(
        *(uint64_t *)next_spot_in_buffer);
    next_spot_in_buffer += sizeof(
        num_leading_zero_bytes_required_in_block_hash);
    blockchain_t *new_blockchain = NULL;
    return_code = blockchain_create(
        &new_blockchain, num_leading_zero_bytes_required_in_block_hash);
    if (SUCCESS != return_code) {
        goto end;
    }
    total_read_size = next_spot_in_buffer + sizeof(uint64_t) - buffer;
    if (total_read_size > buffer_size) {
        return_code = FAILURE_BUFFER_TOO_SMALL;
        blockchain_destroy(new_blockchain);
        goto end;
    }
    uint64_t num_blocks = betoh64(*(uint64_t *)next_spot_in_buffer);
    next_spot_in_buffer += sizeof(num_blocks);
    for (uint64_t block_idx = 0; block_idx < num_blocks; block_idx++) {
        block_t *block = NULL;
        total_read_size = next_spot_in_buffer + sizeof(uint64_t) - buffer;
        if (total_read_size > buffer_size) {
            return_code = FAILURE_BUFFER_TOO_SMALL;
            blockchain_destroy(new_blockchain);
            goto end;
        }
        time_t block_created_at = betoh64(*(uint64_t *)next_spot_in_buffer);
        next_spot_in_buffer += sizeof(block_created_at);
        total_read_size = next_spot_in_buffer + sizeof(sha_256_t) - buffer;
        if (total_read_size > buffer_size) {
            return_code = FAILURE_BUFFER_TOO_SMALL;
            blockchain_destroy(new_blockchain);
            goto end;
        }
        sha_256_t previous_block_hash = {0};
        for (size_t idx = 0; idx < sizeof(previous_block_hash); idx++) {
            previous_block_hash.digest[idx] = *next_spot_in_buffer;
            next_spot_in_buffer++;
        }
        total_read_size = next_spot_in_buffer + sizeof(uint64_t) - buffer;
        if (total_read_size > buffer_size) {
            return_code = FAILURE_BUFFER_TOO_SMALL;
            blockchain_destroy(new_blockchain);
            goto end;
        }
        uint64_t proof_of_work = betoh64(*(uint64_t *)next_spot_in_buffer);
        next_spot_in_buffer += sizeof(proof_of_work);
        total_read_size = next_spot_in_buffer + sizeof(uint64_t) - buffer;
        if (total_read_size > buffer_size) {
            return_code = FAILURE_BUFFER_TOO_SMALL;
            blockchain_destroy(new_blockchain);
            goto end;
        }
        uint64_t num_transactions = betoh64(*(uint64_t *)next_spot_in_buffer);
        next_spot_in_buffer += sizeof(num_transactions);
        linked_list_t *transaction_list = NULL;
        return_code = linked_list_create(
            &transaction_list, (free_function_t *)transaction_destroy, NULL);
        if (SUCCESS != return_code) {
            blockchain_destroy(new_blockchain);
            goto end;
        }
        for (uint64_t transaction_idx = 0;
            transaction_idx < num_transactions;
            transaction_idx++) {
            transaction_t *transaction = calloc(1, sizeof(transaction_t));
            if (NULL == transaction) {
                return_code = FAILURE_COULD_NOT_MALLOC;
                blockchain_destroy(new_blockchain);
                linked_list_destroy(transaction_list);
                goto end;
            }
            total_read_size = next_spot_in_buffer + sizeof(uint64_t) - buffer;
            if (total_read_size > buffer_size) {
                return_code = FAILURE_BUFFER_TOO_SMALL;
                blockchain_destroy(new_blockchain);
                linked_list_destroy(transaction_list);
                free(transaction);
                goto end;
            }
            transaction->created_at = betoh64(*(uint64_t *)next_spot_in_buffer);
            next_spot_in_buffer += sizeof(transaction->created_at);
            total_read_size = next_spot_in_buffer + sizeof(ssh_key_t) - buffer;
            if (total_read_size > buffer_size) {
                return_code = FAILURE_BUFFER_TOO_SMALL;
                blockchain_destroy(new_blockchain);
                linked_list_destroy(transaction_list);
                free(transaction);
                goto end;
            }
            for (size_t idx = 0;
                idx < sizeof(transaction->sender_public_key);
                idx++) {
                transaction->sender_public_key.bytes[idx] =
                    *next_spot_in_buffer;
                next_spot_in_buffer++;
            }
            total_read_size = next_spot_in_buffer + sizeof(ssh_key_t) - buffer;
            if (total_read_size > buffer_size) {
                return_code = FAILURE_BUFFER_TOO_SMALL;
                blockchain_destroy(new_blockchain);
                linked_list_destroy(transaction_list);
                free(transaction);
                goto end;
            }
            for (size_t idx = 0;
                idx < sizeof(transaction->recipient_public_key);
                idx++) {
                transaction->recipient_public_key.bytes[idx] =
                    *next_spot_in_buffer;
                next_spot_in_buffer++;
            }
            total_read_size = next_spot_in_buffer + sizeof(uint64_t) - buffer;
            if (total_read_size > buffer_size) {
                return_code = FAILURE_BUFFER_TOO_SMALL;
                blockchain_destroy(new_blockchain);
                linked_list_destroy(transaction_list);
                free(transaction);
                goto end;
            }
            transaction->amount = betoh64(*(uint64_t *)next_spot_in_buffer);
            next_spot_in_buffer += sizeof(transaction->amount);
            total_read_size = next_spot_in_buffer + sizeof(uint64_t) - buffer;
            if (total_read_size > buffer_size) {
                return_code = FAILURE_BUFFER_TOO_SMALL;
                blockchain_destroy(new_blockchain);
                linked_list_destroy(transaction_list);
                free(transaction);
                goto end;
            }
            transaction->sender_signature.length = betoh64(
                *(uint64_t *)next_spot_in_buffer);
            next_spot_in_buffer += sizeof(transaction->sender_signature.length);
            if (transaction->sender_signature.length > MAX_SSH_KEY_LENGTH) {
                return_code = FAILURE_SIGNATURE_TOO_LONG;
                blockchain_destroy(new_blockchain);
                linked_list_destroy(transaction_list);
                free(transaction);
                goto end;
            }
            total_read_size =
                next_spot_in_buffer +
                transaction->sender_signature.length -
                buffer;
            if (total_read_size > buffer_size) {
                return_code = FAILURE_BUFFER_TOO_SMALL;
                blockchain_destroy(new_blockchain);
                linked_list_destroy(transaction_list);
                free(transaction);
                goto end;
            }
            for (size_t idx = 0;
                idx < sizeof(transaction->sender_signature.bytes);
                idx++) {
                transaction->sender_signature.bytes[idx] = *next_spot_in_buffer;
                next_spot_in_buffer++;
            }
            return_code = linked_list_append(transaction_list, transaction);
            if (SUCCESS != return_code) {
                blockchain_destroy(new_blockchain);
                linked_list_destroy(transaction_list);
                free(transaction);
                goto end;
            }
        }
        return_code = block_create(
            &block, transaction_list, proof_of_work, previous_block_hash);
        if (SUCCESS != return_code) {
            blockchain_destroy(new_blockchain);
            linked_list_destroy(transaction_list);
            goto end;
        }
        block->created_at = block_created_at;
        return_code = blockchain_add_block(new_blockchain, block);
        if (SUCCESS != return_code) {
            blockchain_destroy(new_blockchain);
            block_destroy(block);
            goto end;
        }
    }
    *blockchain = new_blockchain;
end:
    return return_code;
}

return_code_t blockchain_write_to_file(
    blockchain_t *blockchain,
    char *outfile
) {
    return_code_t return_code = SUCCESS;
    if (NULL == blockchain || NULL == outfile) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    unsigned char *buffer = NULL;
    size_t buffer_size = 0;
    return_code = blockchain_serialize(blockchain, &buffer, &buffer_size);
    if (SUCCESS != return_code) {
        goto end;
    }
    FILE *f = fopen(outfile, "wb");
    if (NULL == f) {
        return_code = FAILURE_FILE_IO;
        goto end;
    }
    size_t bytes_written = fwrite(buffer, 1, buffer_size, f);
    fclose(f);
    free(buffer);
    if (bytes_written != buffer_size) {
        return_code = FAILURE_FILE_IO;
        goto end;
    }
end:
    return return_code;
}

return_code_t blockchain_read_from_file(
    blockchain_t **blockchain,
    char *infile
) {
    return_code_t return_code = SUCCESS;
    if (NULL == blockchain || NULL == infile) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    FILE *f = fopen(infile, "rb");
    if (NULL == f) {
        return_code = FAILURE_FILE_IO;
        goto end;
    }
    fseek(f, 0, SEEK_END);
    uint64_t buffer_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *buffer = malloc(buffer_size);
    if (NULL == buffer) {
        return_code = FAILURE_COULD_NOT_MALLOC;
        goto end;
    }
    size_t read_size = fread(buffer, 1, buffer_size, f);
    if (read_size != (size_t)buffer_size) {
        return_code = FAILURE_FILE_IO;
        goto cleanup;
    }
    return_code = blockchain_deserialize(blockchain, buffer, buffer_size);
cleanup:
    fclose(f);
    free(buffer);
end:
    return return_code;
}
