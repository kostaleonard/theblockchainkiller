/**
 * @brief Defines a transaction on the blockchain.
 */

#ifndef INCLUDE_TRANSACTION_H_
#define INCLUDE_TRANSACTION_H_

#include <stdint.h>

/**
 * @brief Represents a transaction.
 * 
 * @param sender_id The sender's ID.
 * @param recipient_id The recipient's ID.
 * @param amount The amount transferred from sender to recipient.
 */
typedef struct transaction_t {
    uint32_t sender_id;
    uint32_t recipient_id;
    uint32_t amount;
} transaction_t;

#endif  // INCLUDE_TRANSACTION_H_
