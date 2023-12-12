#include <stdio.h> //TODO remove debugging
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/err.h> //TODO remove
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

// TODO explain in markdown file (tests/readme.md) how to generate keys for testing locally and put them in environment variables
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
    // TODO sign data
    // TODO clean up error handling
    BIO *mem_bio = BIO_new_mem_buf(sender_private_key->bytes, MAX_SSH_KEY_LENGTH);
    if (!mem_bio) {
        fprintf(stderr, "Error creating BIO object.\n");
        return FAILURE_OPENSSL_FUNCTION;
    }
    EVP_PKEY *pkey = PEM_read_bio_PrivateKey(mem_bio, NULL, NULL, NULL);
    if (!pkey) {
        fprintf(stderr, "Error reading private key: PEM_read_bio_PrivateKey.\n");
        unsigned long err;
        while ((err = ERR_get_error()) != 0) {
            fprintf(stderr, "OpenSSL Error: %s\n", ERR_error_string(err, NULL));
        }
        //TODO the key doesn't appear to be in PEM format. I think it's PKCS8 or something
        BIO_free(mem_bio);
        return FAILURE_OPENSSL_FUNCTION;
    }

    if (EVP_PKEY_base_id(pkey) != EVP_PKEY_RSA) {
        fprintf(stderr, "The provided key is not an RSA key.\n");
        EVP_PKEY_free(pkey);
        BIO_free(mem_bio);
        return FAILURE_OPENSSL_FUNCTION;
    }
    EVP_MD_CTX *md_ctx = EVP_MD_CTX_new();
    if (!md_ctx) {
        fprintf(stderr, "Error creating message digest context.\n");
        EVP_PKEY_free(pkey);
        //TODO error handling
        return FAILURE_OPENSSL_FUNCTION;
    }

    if (EVP_DigestSignInit(md_ctx, NULL, EVP_sha256(), NULL, pkey) <= 0) {
        fprintf(stderr, "Error initializing digest signing.\n");
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_free(md_ctx);
        //TODO error handling
        return FAILURE_OPENSSL_FUNCTION;
    }

    if (EVP_DigestSignUpdate(md_ctx, transaction, sizeof(transaction_t) - sizeof(transaction->sender_signature)) <= 0) {
        fprintf(stderr, "Error updating digest signing.\n");
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_free(md_ctx);
        //TODO error handling
        return FAILURE_OPENSSL_FUNCTION;
    }
    //TODO only need to call EVP_DigestSignFinal once since we have a max signature length
    size_t sig_len;
    if (EVP_DigestSignFinal(md_ctx, NULL, &sig_len) <= 0) {
        fprintf(stderr, "Error obtaining signature length.\n");
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_free(md_ctx);
        //TODO error handling
        return FAILURE_OPENSSL_FUNCTION;
    }
    printf("Signature length: %lld\n", sig_len);
    if (EVP_DigestSignFinal(md_ctx, (unsigned char *)signature->bytes, &sig_len) <= 0) {
        fprintf(stderr, "Error generating signature.\n");
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_free(md_ctx);
        return 0;
    }
    printf("Signature: ");
    for (size_t idx = 0; idx < sizeof(signature->bytes); idx++) {
        printf("%02x", signature->bytes[idx]);
    }
    printf("\n");
    EVP_PKEY_free(pkey);
    EVP_MD_CTX_free(md_ctx);
end:
    return return_code;
}
