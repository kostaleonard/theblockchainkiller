/**
 * @brief Defines a linked list.
 */

#ifndef INCLUDE_LINKED_LIST_H_
#define INCLUDE_LINKED_LIST_H_

#include "include/return_codes.h"

//TODO docstring
typedef struct node_t {
    void *data;
    struct node_t *next;
} node_t;

//TODO docstring
typedef void (free_function_t(void *data));

//TODO docstring
typedef int (compare_function_t(void *data1, void *data2));

//TODO docstring
typedef struct linked_list_t {
    free_function_t *free_function;
    compare_function_t *compare_function;
    node_t *head;
} linked_list_t;

//TODO docstring
linked_list_t *linked_list_create(
    free_function_t free_function,
    compare_function_t compare_function
);

//TODO docstring
return_code_t linked_list_destroy(linked_list_t *linked_list);

//TODO docstring
return_code_t linked_list_append(void *data);

#endif  // INCLUDE_LINKED_LIST_H_
