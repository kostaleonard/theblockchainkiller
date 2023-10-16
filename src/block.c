#include <stdlib.h>
#include <time.h>
#include <openssl/evp.h>
#include "include/block.h"
#include "include/linked_list.h"
#include "include/transaction.h"

return_code_t block_create(
    block_t **block,
    linked_list_t *transaction_list,
    uint32_t proof_of_work,
    sha_256_t previous_block_hash
) {
    return_code_t return_code = SUCCESS;
    if (NULL == block || NULL == transaction_list) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    block_t *new_block = malloc(sizeof(block_t));
    if (NULL == new_block) {
        return_code = FAILURE_COULD_NOT_MALLOC;
        goto end;
    }
    time_t created_at = time(NULL);
    new_block->created_at = created_at;
    new_block->transaction_list = transaction_list;
    new_block->proof_of_work = proof_of_work;
    new_block->previous_block_hash = previous_block_hash;
    *block = new_block;
end:
    return return_code;
}

return_code_t block_create_genesis_block(block_t **block) {
    return_code_t return_code = SUCCESS;
    if (NULL == block) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    linked_list_t *transaction_list = NULL;
    return_code = linked_list_create(
        &transaction_list,
        (free_function_t *)transaction_destroy,
        NULL);
    if (SUCCESS != return_code) {
        goto end;
    }
    sha_256_t previous_block_hash = {0};
    return_code = block_create(
        block,
        transaction_list,
        GENESIS_BLOCK_PROOF_OF_WORK,
        previous_block_hash);
end:
    return return_code;
}

return_code_t block_destroy(block_t *block) {
    return_code_t return_code = SUCCESS;
    if (NULL == block) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    return_code = linked_list_destroy(block->transaction_list);
    free(block);
end:
    return return_code;
}

return_code_t block_hash(block_t *block, sha_256_t *hash) {
    return_code_t return_code = SUCCESS;
    if (NULL == block || NULL == hash) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    EVP_MD_CTX *mdctx;
    const EVP_MD *md = EVP_sha256();
    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, md, NULL);
    
    //TODO put this in helper function with same interface as transaction?
    EVP_DigestUpdate(
        mdctx, (void *)&block->created_at, sizeof(block->created_at));
    EVP_DigestUpdate(
        mdctx, (void *)&block->proof_of_work, sizeof(block->proof_of_work));
    EVP_DigestUpdate(
        mdctx,
        (void *)&block->previous_block_hash,
        sizeof(block->previous_block_hash));
    // TODO hash transaction list.
    
    EVP_DigestFinal_ex(mdctx, hash->digest, NULL);
    EVP_MD_CTX_free(mdctx);
end:
    return return_code;
}
