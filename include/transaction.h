/**
 * @brief Defines a transaction on the blockchain.
 */

#ifndef INCLUDE_TRANSACTION_H_
#define INCLUDE_TRANSACTION_H_

#include <stdint.h>

typedef struct transaction_t {
    uint32_t sender_id; //TODO public key?
    uint32_t recipient_id;
    uint32_t amount;
} transaction_t;

#endif  // INCLUDE_TRANSACTION_H_
