/**
 * @brief Defines a transaction on the blockchain.
 */

#ifndef INCLUDE_TRANSACTION_H_
#define INCLUDE_TRANSACTION_H_

#include <stdint.h>
#include <sys/time.h>
#include "include/return_codes.h"

#define SENDER_ID_FOR_MINTING 0
#define AMOUNT_GENERATED_DURING_MINTING 1

/**
 * @brief Represents a transaction.
 * 
 * @param created_at The datetime at which the user created this block.
 * @param sender_id The sender's ID.
 * @param recipient_id The recipient's ID.
 * @param amount The amount transferred from sender to recipient.
 */
typedef struct transaction_t {
    time_t created_at;
    uint32_t sender_id;
    uint32_t recipient_id;
    uint32_t amount;
} transaction_t;

/**
 * @brief Fills transaction with a newly allocated transaction.
 * 
 * @param transaction The pointer to fill with the new transaction.
 * @param sender_id The sender's ID.
 * @param recipient_id The recipient's ID.
 * @param amount The amount transferred from sender to recipient.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t transaction_create(
    transaction_t **transaction,
    uint32_t sender_id,
    uint32_t recipient_id,
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
