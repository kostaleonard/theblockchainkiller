#include <stdlib.h>
#include "include/transaction.h"

return_code_t transaction_create(
    transaction_t **transaction,
    uint32_t sender_id,
    uint32_t recipient_id,
    uint32_t amount
) {
    return_code_t return_code = SUCCESS;
    if (NULL == transaction) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    transaction_t *new_transaction = malloc(sizeof(transaction_t));
    if (NULL == new_transaction) {
        return_code = FAILURE_COULD_NOT_MALLOC;
        goto end;
    }
    new_transaction->created_at = time(NULL);
    new_transaction->sender_id = sender_id;
    new_transaction->recipient_id = recipient_id;
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
