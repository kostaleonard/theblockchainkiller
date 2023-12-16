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
    printf("Data to be signed: %p + %lld\n", transaction, sizeof(transaction_t) - sizeof(transaction->sender_signature));

    if (EVP_DigestSignUpdate(md_ctx, transaction, sizeof(transaction_t) - sizeof(transaction->sender_signature)) <= 0) {
        fprintf(stderr, "Error updating digest signing.\n");
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_free(md_ctx);
        //TODO error handling
        return FAILURE_OPENSSL_FUNCTION;
    }
    //TODO only need to call EVP_DigestSignFinal once since we have a max signature length--the first call gets the signature length
    size_t sig_len;
    if (EVP_DigestSignFinal(md_ctx, NULL, &sig_len) <= 0) {
        fprintf(stderr, "Error obtaining signature length.\n");
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_free(md_ctx);
        //TODO error handling
        return FAILURE_OPENSSL_FUNCTION;
    }
    printf("Signature length: %lld\n", sig_len);
    if (EVP_DigestSignFinal(md_ctx, signature->bytes, &sig_len) <= 0) {
        fprintf(stderr, "Error generating signature.\n");
        EVP_PKEY_free(pkey);
        EVP_MD_CTX_free(md_ctx);
        return 0;
    }
    //TODO remove
    // printf("Signature: ");
    // for (size_t idx = 0; idx < sizeof(signature->bytes); idx++) {
    //     printf("%02hhx", signature->bytes[idx]);
    // }
    // printf("\n");
    EVP_PKEY_free(pkey);
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
    // TODO
    BIO *bio = BIO_new_mem_buf(transaction->sender_public_key.bytes, MAX_SSH_KEY_LENGTH);
    if (bio == NULL) {
        fprintf(stderr, "Error creating BIO object.\n");
        return -1;
    }

    EVP_PKEY *pub_key = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    if (pub_key == NULL) {
        fprintf(stderr, "Error reading public key.\n");
        BIO_free(bio);
        return -1;
    }
    BIO_free(bio);

    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pub_key, NULL);
    if (ctx == NULL) {
        fprintf(stderr, "Error creating PKEY context.\n");
        EVP_PKEY_free(pub_key);
        return -1;
    }

    // Set the signature algorithm
    if (EVP_PKEY_verify_init(ctx) <= 0) {
        fprintf(stderr, "Error initializing verification context.\n");
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pub_key);
        return -1;
    }

    // Set the hashing algorithm (SHA256 in this case)
    if (EVP_PKEY_CTX_set_signature_md(ctx, EVP_sha256()) <= 0) {
        fprintf(stderr, "Error setting hashing algorithm.\n");
        EVP_PKEY_CTX_free(ctx);
        EVP_PKEY_free(pub_key);
        return -1;
    }
    //TODO remove
    // printf("Signature for verification: ");
    // for (size_t idx = 0; idx < 512; idx++) {
    //     printf("%02x", transaction->sender_signature.bytes[idx]);
    // }
    // printf("\n");
    printf("Data to be verified: %p + %lld\n", transaction, sizeof(transaction_t) - sizeof(transaction->sender_signature));

    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char *)transaction, sizeof(transaction_t) - sizeof(transaction->sender_signature), digest);

    // TODO don't hard code signature length. It might have to go in transaction_t
    // Verify the signature
    if (EVP_PKEY_verify(ctx, transaction->sender_signature.bytes, 256, digest, SHA256_DIGEST_LENGTH) <= 0) {
        fprintf(stderr, "Signature verification failed.\n");
        unsigned long err;
        while ((err = ERR_get_error()) != 0) {
            fprintf(stderr, "OpenSSL Error: %s\n", ERR_error_string(err, NULL));
        }
        *is_valid_signature = false;
    } else {
        printf("Signature verified successfully.\n");
        *is_valid_signature = true;
    }

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(pub_key);

end:
    return return_code;
}
