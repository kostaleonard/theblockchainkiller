/**
 * @brief Defines a transaction on the blockchain.
 */

#ifndef INCLUDE_TRANSACTION_H_
#define INCLUDE_TRANSACTION_H_

#include <stdint.h>
#include <sys/time.h>
#include "include/return_codes.h"
#include "include/cryptography.h"

#define AMOUNT_GENERATED_DURING_MINTING 1

/**
 * @brief Represents a transaction.
 * 
 * @param created_at The datetime at which the user created this block.
 * @param sender_public_key The sender's public key. Set to zero to represent
 * coins generated during the mining process.
 * @param recipient_public_key The recipient's public key.
 * @param amount The amount transferred from sender to recipient.
 */
typedef struct transaction_t {
    time_t created_at;
    ssh_key_t sender_public_key;
    ssh_key_t recipient_public_key;
    uint32_t amount;
    // TODO need sender's signature here to provide authentication, integrity, and non-repudiation
} transaction_t;

/**
 * @brief Fills transaction with a newly allocated transaction.
 * 
 * @param transaction The pointer to fill with the new transaction.
 * @param sender_public_key The sender's public key. Set to zero to represent
 * coins generated during the mining process.
 * @param recipient_public_key The recipient's public key.
 * @param amount The amount transferred from sender to recipient.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t transaction_create(
    transaction_t **transaction,
    ssh_key_t *sender_public_key,
    ssh_key_t *recipient_public_key,
    uint32_t amount
);

/**
 * @brief Frees all memory associated with the transaction.
 * 
 * @param transaction The transaction.
 * @return return_code_t 
 */
return_code_t transaction_destroy(transaction_t *transaction);

#endif  // INCLUDE_TRANSACTION_H_
