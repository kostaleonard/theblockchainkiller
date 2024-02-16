/**
 * @brief Defines the blockchain.
 * 
 * See https://github.com/dvf/blockchain for a helpful Python blockchain
 * implementation.
 */

#ifndef INCLUDE_BLOCKCHAIN_H_
#define INCLUDE_BLOCKCHAIN_H_

#include <stdint.h>
#include <sys/time.h>
#include "include/block.h"
#include "include/return_codes.h"

/**
 * @brief Represents a blockchain.
 * 
 * @param block_list The list of blocks in the chain.
 */
typedef struct blockchain_t {
    linked_list_t *block_list;
    size_t num_leading_zero_bytes_required_in_block_hash;
} blockchain_t;

/**
 * @brief Fills blockchain with a pointer to the newly allocated blockchain.
 * 
 * @param blockchain A pointer to fill with the blockchain's address.
 * @param num_leading_zero_bytes_required_in_block_hash The number of leading
 * zero bytes to make a block hash a valid proof of work.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_create(
    blockchain_t **blockchain,
    size_t num_leading_zero_bytes_required_in_block_hash
);

/**
 * @brief Frees all memory associated with a blockchain.
 * 
 * @param blockchain The blockchain to destroy.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_destroy(blockchain_t *blockchain);

/**
 * @brief Appends a block to the blockchain.
 * 
 * @param blockchain The blockchain.
 * @param block The block to add.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_add_block(blockchain_t *blockchain, block_t *block);

/**
 * @brief Fills is_valid_block_hash with true or false.
 * 
 * A valid block hash begins with the number of leading zeroes specified in the
 * blockchain struct.
 * 
 * @param blockchain The blockchain.
 * @param block_hash The block hash to check.
 * @param is_valid_block_hash A pointer to fill with the result.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_is_valid_block_hash(
    blockchain_t *blockchain,
    sha_256_t block_hash,
    bool *is_valid_block_hash
);

/**
 * @brief Fills block's proof_of_work with a number that produces a valid hash.
 * 
 * @param blockchain The blockchain.
 * @param block The block for which to calculate a proof of work.
 * @param print_progress If true, display progress on the screen.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_mine_block(
    blockchain_t *blockchain,
    block_t *block,
    bool print_progress
);

/**
 * @brief Prints the blockchain.
 */
void blockchain_print(blockchain_t *blockchain);

/**
 * @brief Serializes the blockchain into a buffer for file or network I/O.
 * 
 * @param blockchain The blockchain.
 * @param buffer A pointer to fill with the bytes representing the blockchain.
 * Callers can write the bytes to a file or send on the network and reconstruct
 * the original blockchain with blockchain_deserialize. Callers are responsible
 * for freeing the buffer.
 * @param buffer_size A pointer to fill with the final size of the buffer.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_serialize(
    blockchain_t *blockchain,
    unsigned char **buffer,
    uint64_t *buffer_size
);

/**
 * @brief Reconstructs the blockchain from a buffer.
 * 
 * @param blockchain A pointer to fill with the reconstructed blockchain.
 * Callers are responsible for calling blockchain_destroy when finished.
 * @param buffer An array containing the serialized blockchain.
 * @param buffer_size The length of the serialized blockchain.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_deserialize(
    blockchain_t **blockchain,
    unsigned char *buffer,
    uint64_t buffer_size
);

/**
 * @brief Saves the blockchain to a file.
 * 
 * @param blockchain The blockchain.
 * @param outfile The path to the file to which to write the blockchain.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_write_to_file(
    blockchain_t *blockchain,
    char *outfile
);

/**
 * @brief Reads the blockchain from a file.
 * 
 * @param blockchain A pointer to fill with the reconstructed blockchain.
 * Callers are responsible for calling blockchain_destroy when finished.
 * @param infile The path to the file from which to read the blockchain.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t blockchain_read_from_file(
    blockchain_t **blockchain,
    char *infile
);

#endif  // INCLUDE_BLOCKCHAIN_H_
