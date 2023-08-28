#include "include/transaction.h"

int transaction_compare(
    transaction_t *transaction1,
    transaction_t *transaction2
) {
    // TODO test
    if (NULL == transaction1 || NULL == transaction2) {
        return 0;
    }
    if (transaction1->created_at == transaction2->created_at &&
        transaction1->sender_id == transaction2->sender_id &&
        transaction1->recipient_id == transaction2->recipient_id &&
        transaction1->amount == transaction2->amount) {
        return 0;
    }
    return transaction1->created_at - transaction2->created_at;
}

return_code_t transaction_create(
    transaction_t **transaction,
    uint32_t sender_id,
    uint32_t recipient_id,
    uint32_t amount
) {
    return FAILURE_INVALID_INPUT;
}

return_code_t transaction_destroy(transaction_t *transaction) {
    return FAILURE_INVALID_INPUT;
}
