/**
 * @brief Defines return codes for the project.
 */

#ifndef INCLUDE_RETURN_CODES_H_
#define INCLUDE_RETURN_CODES_H_

typedef enum return_code_t {
    SUCCESS,
    FAILURE_COULD_NOT_MALLOC,
    FAILURE_INVALID_INPUT,
    FAILURE_LINKED_LIST_EMPTY
} return_code_t;

#endif
