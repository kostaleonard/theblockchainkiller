#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
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
    // TODO sign the transaction--test this
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
    BIO *mem_bio = BIO_new_mem_buf(
        sender_private_key->bytes, MAX_SSH_KEY_LENGTH);
    if (!mem_bio) {
        fprintf(stderr, "Error creating BIO object.\n");
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    EVP_PKEY *private_key = PEM_read_bio_PrivateKey(mem_bio, NULL, NULL, NULL);
    BIO_free(mem_bio);
    if (!private_key) {
        fprintf(stderr, "Error reading private key.\n");
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    if (EVP_PKEY_base_id(private_key) != EVP_PKEY_RSA) {
        fprintf(stderr, "The provided key is not an RSA key.\n");
        EVP_PKEY_free(private_key);
        BIO_free(mem_bio);
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    if (!md_ctx) {
        fprintf(stderr, "Error creating message digest context.\n");
        EVP_PKEY_free(private_key);
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    if (EVP_DigestSignInit(
            md_ctx, NULL, EVP_sha256(), NULL, private_key) <= 0) {
        fprintf(stderr, "Error initializing digest signing.\n");
        EVP_PKEY_free(private_key);
        EVP_MD_CTX_free(md_ctx);
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    size_t size_without_signature = offsetof(transaction_t, sender_signature);
    if (EVP_DigestSignUpdate(
            md_ctx, transaction, size_without_signature) <= 0) {
        fprintf(stderr, "Error updating digest signing.\n");
        EVP_PKEY_free(private_key);
        EVP_MD_CTX_free(md_ctx);
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    size_t sig_len = 0;
    if (EVP_DigestSignFinal(md_ctx, NULL, &sig_len) <= 0) {
        fprintf(stderr, "Error obtaining signature length.\n");
        EVP_PKEY_free(private_key);
        EVP_MD_CTX_free(md_ctx);
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    if (EVP_DigestSignFinal(md_ctx, signature->bytes, &sig_len) <= 0) {
        fprintf(stderr, "Error generating signature.\n");
        EVP_PKEY_free(private_key);
        EVP_MD_CTX_free(md_ctx);
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    EVP_PKEY_free(private_key);
    EVP_MD_CTX_free(md_ctx);
end:
    return return_code;
}

return_code_t transaction_verify_signature(
    bool *is_valid_signature,
    transaction_t *transaction
) {
    return_code_t return_code = SUCCESS;
    if (NULL == is_valid_signature || NULL == transaction) {
        return_code = FAILURE_INVALID_INPUT;
        goto end;
    }
    BIO *bio = BIO_new_mem_buf(
        transaction->sender_public_key.bytes, MAX_SSH_KEY_LENGTH);
    if (bio == NULL) {
        fprintf(stderr, "Error creating BIO object.\n");
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    EVP_PKEY *public_key = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    BIO_free(bio);
    if (public_key == NULL) {
        fprintf(stderr, "Error reading public key.\n");
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    size_t size_without_signature = offsetof(transaction_t, sender_signature);
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    if (!md_ctx) {
        fprintf(stderr, "Error creating message digest context.\n");
        EVP_PKEY_free(public_key);
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    if (EVP_VerifyInit(md_ctx, EVP_sha256()) <= 0) {
        fprintf(stderr, "Error initializing digest verification.\n");
        EVP_PKEY_free(public_key);
        EVP_MD_CTX_free(md_ctx);
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    if (EVP_VerifyUpdate(md_ctx, transaction, size_without_signature) <= 0) {
        fprintf(stderr, "Error updating digest verification.\n");
        EVP_PKEY_free(public_key);
        EVP_MD_CTX_free(md_ctx);
        return_code = FAILURE_OPENSSL_FUNCTION;
        goto end;
    }
    //TODO don't hard code signature length
    if (EVP_VerifyFinal(md_ctx, transaction->sender_signature.bytes, 256, public_key) == 1) {
        *is_valid_signature = true;
    }
    else {
        *is_valid_signature = false;
    }
    EVP_MD_CTX_free(md_ctx);
    EVP_PKEY_free(public_key);
end:
    return return_code;
}
