/**
 * @brief Defines a linked list.
 */

#ifndef INCLUDE_LINKED_LIST_H_
#define INCLUDE_LINKED_LIST_H_

#include <stdbool.h>
#include "include/return_codes.h"

/**
 * @brief A linked list node.
 * 
 * @param data A generic pointer to data the user has stored in the node.
 * @param next A pointer to the next link in the node, or NULL if this node is
 * the last in the list.
 */
typedef struct node_t {
    void *data;
    struct node_t *next;
} node_t;

/**
 * @brief A user-defined function that will free the data in a node.
 */
typedef void (free_function_t(void *data));

/**
 * @brief A user-defined function that will compare two elements of data.
 * 
 * The compare function should return a negative number if data1 < data2, a
 * positive number if data1 > data2, and zero if data1 and data2 are equal. If
 * data1 and data2 point to integers, one valid compare function would return
 * *data1 minus *data2.
 */
typedef int (compare_function_t(void *data1, void *data2));

/**
 * @brief A linked list.
 * 
 * @param free_function A user-defined function that will free node data.
 * @param compare_function A user-defined function that will compare node data.
 * @param head The first node in the linked list. If the list is empty, head
 * will be NULL.
 */
typedef struct linked_list_t {
    free_function_t *free_function;
    compare_function_t *compare_function;
    node_t *head;
} linked_list_t;

/**
 * @brief Fills linked_list with a pointer to the newly allocated linked list.
 * 
 * @param linked_list A pointer to fill with the linked list's address.
 * @param free_function A user-defined function that will free node data.
 * @param compare_function A user-defined function that will compare node data.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t linked_list_create(
    linked_list_t **linked_list,
    free_function_t free_function,
    compare_function_t compare_function
);

/**
 * @brief Frees all memory associated with the linked list.
 * 
 * @param linked_list The linked list to destroy.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t linked_list_destroy(linked_list_t *linked_list);

/**
 * @brief Prepends a node containing the given data to the linked list.
 * 
 * @param linked_list The linked list.
 * @param data The data to place in the new node. The user should already have
 * allocated this memory.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t linked_list_prepend(linked_list_t *linked_list, void *data);

/**
 * @brief Fills node with the first node of the linked list.
 * 
 * If the list is empty, this function will return FAILURE_LINKED_LIST_EMPTY.
 * 
 * @param linked_list The linked list.
 * @param node A pointer to fill with the first node's address.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t linked_list_get_first(linked_list_t *linked_list, node_t **node);

/**
 * @brief Removes the first node from the linked list and frees its memory.
 * 
 * @param linked_list The linked list.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t linked_list_remove_first(linked_list_t *linked_list);

/**
 * @brief Fills is_empty with true if the list is empty, false otherwise.
 * 
 * @param linked_list The linked list.
 * @param is_empty A pointer to fill with the result.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t linked_list_is_empty(linked_list_t *linked_list, bool *is_empty);

/**
 * @brief Fills node with the first node whose data matches the given data.
 * 
 * @param linked_list The linked list.
 * @param data The data for which to search.
 * @param node A pointer to fill with the matching node.
 * @return return_code_t A return code indicating success or failure.
 */
return_code_t linked_list_find(
    linked_list_t *linked_list,
    void *data,
    node_t **node
);

#endif  // INCLUDE_LINKED_LIST_H_
