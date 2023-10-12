/**
 * @brief Runs the app.
 */

#include <stdio.h>
#include "include/blockchain.h"
#include "include/block.h"

int main(int argc, char **argv) {
    return_code_t return_code = SUCCESS;
    blockchain_t *blockchain = NULL;
    block_t *genesis_block = NULL;
    return_code = blockchain_create(&blockchain);
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
    // return_code = block_hash(genesis_block, &genesis_block_hash);
    // if (SUCCESS != return_code) {
    //     goto end;
    // }
    hash_print(&genesis_block_hash);
end:
    blockchain_destroy(blockchain);
    return return_code;
}
