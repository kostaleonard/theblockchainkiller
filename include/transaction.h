/**
 * @brief Defines a transaction on the blockchain.
 */

#ifndef INCLUDE_TRANSACTION_H_
#define INCLUDE_TRANSACTION_H_

#include <stdbool.h>
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
 * @param sender_signature The digital signature the sender creates to provide
 * authentication, integrity, and non-repudiation for the transaction. The
 * signature covers all fields in this data structure except for the signature
 * itself.
 */
typedef struct transaction_t {
    time_t created_at;
    ssh_key_t sender_public_key;
    ssh_key_t recipient_public_key;
    uint32_t amount;
    ssh_signature_t sender_signature;
} transaction_t;

/**
 * @brief Fills transaction with a newly allocated transaction.
 * 
 * @param transaction The pointer to fill with the new transaction.
 * @param sender_public_key The sender's public key. Set to zero to represent
 * coins generated during the mining process.
 * @param recipient_public_key The recipient's public key.
 * @param amount The amount transferred from sender to recipient.
 * @param sender_private_key The sender's private key. This function will fill
 * the transaction_t with a signature that covers all fields in the data
 * structure except for the signature itself.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t transaction_create(
    transaction_t **transaction,
    ssh_key_t *sender_public_key,
    ssh_key_t *recipient_public_key,
    uint32_t amount,
    ssh_key_t *sender_private_key
);

/**
 * @brief Frees all memory associated with the transaction.
 * 
 * @param transaction The transaction.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t transaction_destroy(transaction_t *transaction);

/**
 * @brief Fills signature with the sender's signature for the transaction.
 * 
 * @param signature The pointer to fill with the signature.
 * @param transaction The transaction for which to generate a signature. The
 * transaction should have all fields filled out except sender_signature. This
 * function generates a value to fill that field.
 * @param sender_private_key The sender's private key.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t transaction_generate_signature(
    ssh_signature_t *signature,
    transaction_t *transaction,
    ssh_key_t *sender_private_key
);

/**
 * @brief Fills is_valid_signature with the signature's correctness.
 * 
 * @param is_valid_signature The pointer to fill with the result. A signature is
 * valid if decrypting the signature with the public key produces the original
 * message (or message hash).
 * @param transaction The transaction whose signature to verify. The signature
 * must cover all fields in the transaction except the signature itself.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t transaction_verify_signature(
    bool *is_valid_signature,
    transaction_t *transaction
);

#endif  // INCLUDE_TRANSACTION_H_
