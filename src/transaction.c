#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/transaction.h"

return_code_t transaction_create(
    transaction_t **transaction,
    ssh_key_t *sender_public_key,
    ssh_key_t *recipient_public_key,
    uint32_t amount,
    ssh_key_t *sender_private_key
) {
    return_code_t return_code = SUCCESS;
    if (NULL == transaction ||
        NULL == sender_public_key ||
        NULL == recipient_public_key ||
        NULL == sender_private_key) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    transaction_t *new_transaction = calloc(1, sizeof(transaction_t));
    if (NULL == new_transaction) {
        return_code = FAILURE_COULD_NOT_MALLOC;
        goto end;
    }
    new_transaction->created_at = time(NULL);
    memcpy(
        &new_transaction->sender_public_key,
        sender_public_key,
        MAX_SSH_KEY_LENGTH);
    memcpy(
        &new_transaction->recipient_public_key,
        recipient_public_key,
        MAX_SSH_KEY_LENGTH);
    new_transaction->amount = amount;
    *transaction = new_transaction;
end:
    return return_code;
}

return_code_t transaction_destroy(transaction_t *transaction) {
    return_code_t return_code = SUCCESS;
    if (NULL == transaction) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    free(transaction);
end:
    return return_code;
}

return_code_t transaction_generate_signature(
    ssh_signature_t *signature,
    transaction_t *transaction,
    ssh_key_t *sender_private_key
) {
    return_code_t return_code = SUCCESS;
    if (NULL == signature ||
        NULL == transaction ||
        NULL == sender_private_key) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    // TODO
end:
    return return_code;
}
