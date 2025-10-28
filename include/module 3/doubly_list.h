/*
 * @file doubly_list.h
 * @brief Doubly Linked List Data Structure
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Doubly Linked List Module
 * Provides a doubly linked list with efficient bidirectional traversal
 */

#ifndef CSTRUCTS_DOUBLY_LIST_H
#define CSTRUCTS_DOUBLY_LIST_H

#include "../module 1/core.h"
#include <stdbool.h>

/* ===== NODE STRUCTURE ===== */

typedef struct doubly_node
{
        void *data;               // Pointer to node data
        struct doubly_node *next; // Pointer to next node
        struct doubly_node *prev; // Pointer to previous node
} doubly_node_t;

/* ===== LIST STRUCTURE ===== */

typedef struct
{
        doubly_node_t *head; // First node in list
        doubly_node_t *tail; // Last node in list
        size_t size;         // Number of elements in list
        size_t element_size; // Size of each element in bytes
} doubly_list_t;

/* ===== LIST CREATION AND DESTRUCTION ===== */

/*
 * @brief Creates a new doubly linked list with specified element size
 * @param element_size Size of each element in bytes
 * @return Pointer to new list, NULL on failure
 *
 * @note Time complexity: O(1)
 * @note Uses both head and tail pointers for bidirectional operations
 */
doubly_list_t *doubly_list_create(size_t element_size);

/*
 * @brief Destroys a doubly linked list and frees all associated memory
 * @param list Pointer to list to destroy
 *
 * @note Time complexity: O(n) where n is list size
 * @note Frees all nodes and their data
 */
void doubly_list_destroy(doubly_list_t *list);

/*
 * @brief Creates a deep copy of a doubly linked list
 * @param src List to copy
 * @return New list with copied data, NULL on failure
 *
 * @note Time complexity: O(n) where n is source size
 * @note Creates deep copy of all elements
 */
doubly_list_t *doubly_list_copy(const doubly_list_t *src);

/* ===== BASIC OPERATIONS ===== */

/*
 * @brief Inserts an element at the beginning of the list
 * @param list Target list
 * @param element Element to insert
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 */
status_t doubly_list_push_front(doubly_list_t *list, const void *element);

/*
 * @brief Appends an element to the end of the list
 * @param list Target list
 * @param element Element to append
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 */
status_t doubly_list_push_back(doubly_list_t *list, const void *element);

/*
 * @brief Removes and returns the first element
 * @param list Target list
 * @param output Where to store the popped element (can be NULL if not needed)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 */
status_t doubly_list_pop_front(doubly_list_t *list, void *output);

/*
 * @brief Removes and returns the last element
 * @param list Target list
 * @param output Where to store the popped element (can be NULL if not needed)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 */
status_t doubly_list_pop_back(doubly_list_t *list, void *output);

/*
 * @brief Inserts an element at specified index
 * @param list Target list
 * @param index Index where to insert
 * @param element Element to insert
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at ends
 * @note Bidirectional optimization: starts from head or tail depending on index
 */
status_t doubly_list_insert(doubly_list_t *list, size_t index,
                            const void *element);

/*
 * @brief Removes element at specified index
 * @param list Target list
 * @param index Index of element to remove
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at ends
 * @note Bidirectional optimization for efficient access
 */
status_t doubly_list_remove(doubly_list_t *list, size_t index);

/*
 * @brief Removes all elements from list (does not free list structure)
 * @param list Target list
 */
void doubly_list_clear(doubly_list_t *list);

/* ===== ELEMENT ACCESS ===== */

/*
 * @brief Gets element at specified index
 * @param list Target list
 * @param index Index of element
 * @param output Where to store the element
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at ends
 * @note Bidirectional optimization for efficient access
 */
status_t doubly_list_get(const doubly_list_t *list, size_t index, void *output);

/*
 * @brief Sets element at specified index
 * @param list Target list
 * @param index Index of element to set
 * @param element New element value
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at ends
 * @note Bidirectional optimization for efficient access
 */
status_t doubly_list_set(doubly_list_t *list, size_t index,
                         const void *element);

/*
 * @brief Gets pointer to element at specified index (unsafe but fast)
 * @param list Target list
 * @param index Index of element
 * @return Pointer to element, NULL on error
 *
 * @note Time complexity: O(n) in worst case, O(1) at ends
 * @warning Unsafe: returned pointer becomes invalid if list is modified
 */
void *doubly_list_get_ref(const doubly_list_t *list, size_t index);

/*
 * @brief Gets pointer to first element
 * @param list Target list
 * @return Pointer to first element, NULL if empty
 *
 * @note Time complexity: O(1)
 */
void *doubly_list_front(const doubly_list_t *list);

/*
 * @brief Gets pointer to last element
 * @param list Target list
 * @return Pointer to last element, NULL if empty
 *
 * @note Time complexity: O(1)
 */
void *doubly_list_back(const doubly_list_t *list);

/* ===== LIST PROPERTIES ===== */

/*
 * @brief Gets current number of elements in list
 * @param list Target list
 * @return Number of elements
 */
size_t doubly_list_size(const doubly_list_t *list);

/*
 * @brief Checks if list is empty
 * @param list Target list
 * @return true if empty, false otherwise
 */
bool doubly_list_empty(const doubly_list_t *list);

/* ===== SEARCH OPERATIONS ===== */

/*
 * @brief Finds first occurrence of element in list
 * @param list Target list
 * @param element Element to find
 * @param cmp Comparison function
 * @return Index of element if found, -1 if not found
 *
 * @note Time complexity: O(n) where n is list size
 * @note Linear search from head to tail
 */
int doubly_list_find(const doubly_list_t *list, const void *element,
                     cmp_fn cmp);

/*
 * @brief Checks if list contains element
 * @param list Target list
 * @param element Element to check
 * @param cmp Comparison function
 * @return true if found, false otherwise
 *
 * @note Time complexity: O(n) where n is list size
 * @note Uses linear search for existence check
 */
bool doubly_list_contains(const doubly_list_t *list, const void *element,
                          cmp_fn cmp);

/* ===== BIDIRECTIONAL OPERATIONS ===== */

/*
 * @brief Inserts element before specified node
 * @param list Target list
 * @param node Node to insert before
 * @param element Element to insert
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) when node is known
 * @note Efficient insertion with direct node reference
 */
status_t doubly_list_insert_before(doubly_list_t *list, doubly_node_t *node,
                                   const void *element);

/*
 * @brief Inserts element after specified node
 * @param list Target list
 * @param node Node to insert after
 * @param element Element to insert
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) when node is known
 * @note Efficient insertion with direct node reference
 */
status_t doubly_list_insert_after(doubly_list_t *list, doubly_node_t *node,
                                  const void *element);

/* ===== UTILITY OPERATIONS ===== */

/*
 * @brief Reverses the list in-place
 * @param list Target list
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) where n is list size
 * @note In-place reversal by swapping next/prev pointers
 */
status_t doubly_list_reverse(doubly_list_t *list);

/*
 * @brief Swaps contents of two lists
 * @param a First list
 * @param b Second list
 *
 * @note Time complexity: O(1)
 * @note Efficient pointer swapping without data copying
 */
void doubly_list_swap(doubly_list_t *a, doubly_list_t *b);

/* ===== ITERATION ===== */

/*
 * @brief Iterator structure for doubly linked list
 */
typedef struct
{
        doubly_list_t *list;
        doubly_node_t *current;
        size_t position;
} doubly_list_iter_t;

/*
 * @brief Creates a forward iterator for the list
 * @param list Target list
 * @return Iterator structure
 *
 * @note Time complexity: O(1)
 */
doubly_list_iter_t doubly_list_iter_create(doubly_list_t *list);

/*
 * @brief Creates a reverse iterator for the list
 * @param list Target list
 * @return Iterator structure
 *
 * @note Time complexity: O(1)
 * @note Creates iterator starting from tail for backward traversal
 */
doubly_list_iter_t doubly_list_iter_create_reverse(doubly_list_t *list);

/*
 * @brief Checks if iterator has more elements in forward direction
 * @param iter Iterator
 * @return true if more elements, false otherwise
 *
 * @note Time complexity: O(1)
 */
bool doubly_list_iter_has_next(const doubly_list_iter_t *iter);

/*
 * @brief Checks if iterator has more elements in reverse direction
 * @param iter Iterator
 * @return true if more elements, false otherwise
 *
 * @note Time complexity: O(1)
 */
bool doubly_list_iter_has_prev(const doubly_list_iter_t *iter);

/*
 * @brief Advances iterator and gets next element
 * @param iter Iterator
 * @param output Where to store the element (can be NULL)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) per element, O(n) for full traversal
 */
status_t doubly_list_iter_next(doubly_list_iter_t *iter, void *output);

/*
 * @brief Moves iterator backward and gets previous element
 * @param iter Iterator
 * @param output Where to store the element (can be NULL)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) per element, O(n) for full backward traversal
 */
status_t doubly_list_iter_prev(doubly_list_iter_t *iter, void *output);

/*
 * @brief Removes current element from list (iterator must be valid)
 * @param iter Iterator
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) for current element removal
 * @note Maintains iterator validity after removal
 */
status_t doubly_list_iter_remove(doubly_list_iter_t *iter);

#endif /* CSTRUCTS_DOUBLY_LIST_H */
