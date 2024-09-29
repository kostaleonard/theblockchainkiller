/**
 * @brief Defines return codes for the project.
 */

#ifndef INCLUDE_RETURN_CODES_H_
#define INCLUDE_RETURN_CODES_H_

typedef enum return_code_t {
    SUCCESS,
    FAILURE_COULD_NOT_MALLOC,
    FAILURE_INVALID_INPUT,
    FAILURE_LINKED_LIST_EMPTY,
    FAILURE_LINKED_LIST_NO_COMPARE_FUNCTION,
    FAILURE_COULD_NOT_FIND_VALID_PROOF_OF_WORK,
    FAILURE_INVALID_COMMAND_LINE_ARGS,
    FAILURE_OPENSSL_FUNCTION,
    FAILURE_FILE_IO,
    FAILURE_SIGNATURE_TOO_LONG,
    FAILURE_BUFFER_TOO_SMALL,
    FAILURE_INVALID_BLOCKCHAIN,
    FAILURE_PTHREAD_FUNCTION,
} return_code_t;

#endif  // INCLUDE_RETURN_CODES_H_
