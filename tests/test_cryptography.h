/**
 * @brief Tests cryptography.c
 */

#ifndef TESTS_TEST_CRYPTOGRAPHY_H_
#define TESTS_TEST_CRYPTOGRAPHY_H_

// These environment variables need to contain a base64 encoded RSA key pair.
#define TEST_PRIVATE_KEY_ENVIRONMENT_VARIABLE \
    "THEBLOCKCHAINKILLER_TEST_PRIVATE_KEY"
#define TEST_PUBLIC_KEY_ENVIRONMENT_VARIABLE \
    "THEBLOCKCHAINKILLER_TEST_PUBLIC_KEY"

#endif  // TESTS_TEST_CRYPTOGRAPHY_H_
