/*
 * @file singly_list.h
 * @brief Singly Linked List Data Structure
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Singly Linked List Module
 * Provides a singly linked list with efficient insertion/deletion at head
 */

#ifndef CSTRUCTS_SINGLY_LIST_H
#define CSTRUCTS_SINGLY_LIST_H

#include "../module 1/core.h"
#include <stdbool.h>

/* ===== NODE STRUCTURE ===== */

typedef struct singly_node
{
        void *data;               // Pointer to node data
        struct singly_node *next; // Pointer to next node
} singly_node_t;

/* ===== LIST STRUCTURE ===== */

typedef struct
{
        singly_node_t *head; // First node in list
        singly_node_t *tail; // Last node in list (for O(1) append)
        size_t size;         // Number of elements in list
        size_t element_size; // Size of each element in bytes
} singly_list_t;

/* ===== LIST CREATION AND DESTRUCTION ===== */

/*
 * @brief Creates a new singly linked list with specified element size
 * @param element_size Size of each element in bytes
 * @return Pointer to new list, NULL on failure
 *
 * @note Time complexity: O(1)
 * @note Uses tail pointer for O(1) append operations
 */
singly_list_t *singly_list_create(size_t element_size);

/*
 * @brief Destroys a singly linked list and frees all associated memory
 * @param list Pointer to list to destroy
 *
 * @note Time complexity: O(n) where n is list size
 * @note Frees all nodes and their data
 */
void singly_list_destroy(singly_list_t *list);

/*
 * @brief Creates a deep copy of a singly linked list
 * @param src List to copy
 * @return New list with copied data, NULL on failure
 *
 * @note Time complexity: O(n) where n is source size
 * @note Creates deep copy of all elements
 */
singly_list_t *singly_list_copy(const singly_list_t *src);

/* ===== BASIC OPERATIONS ===== */

/*
 * @brief Inserts an element at the beginning of the list
 * @param list Target list
 * @param element Element to insert
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Efficient head insertion
 */
status_t singly_list_push_front(singly_list_t *list, const void *element);

/*
 * @brief Appends an element to the end of the list
 * @param list Target list
 * @param element Element to append
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Uses tail pointer for efficient append
 */
status_t singly_list_push_back(singly_list_t *list, const void *element);

/*
 * @brief Removes and returns the first element
 * @param list Target list
 * @param output Where to store the popped element (can be NULL if not needed)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Efficient head removal
 */
status_t singly_list_pop_front(singly_list_t *list, void *output);

/*
 * @brief Removes and returns the last element
 * @param list Target list
 * @param output Where to store the popped element (can be NULL if not needed)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) where n is list size
 * @note Requires traversal to find previous node
 */
status_t singly_list_pop_back(singly_list_t *list, void *output);

/*
 * @brief Inserts an element at specified index
 * @param list Target list
 * @param index Index where to insert
 * @param element Element to insert
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at head
 * @note Requires traversal to insertion point
 */
status_t singly_list_insert(singly_list_t *list, size_t index,
                            const void *element);

/*
 * @brief Removes element at specified index
 * @param list Target list
 * @param index Index of element to remove
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at head
 * @note Requires traversal to find previous node
 */
status_t singly_list_remove(singly_list_t *list, size_t index);

/*
 * @brief Removes all elements from list (does not free list structure)
 * @param list Target list
 *
 * @note Time complexity: O(n) where n is list size
 * @note Frees all nodes but retains list structure
 */
void singly_list_clear(singly_list_t *list);

/* ===== ELEMENT ACCESS ===== */

/*
 * @brief Gets element at specified index
 * @param list Target list
 * @param index Index of element
 * @param output Where to store the element
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at head
 * @note Requires traversal to index
 */
status_t singly_list_get(const singly_list_t *list, size_t index, void *output);

/*
 * @brief Sets element at specified index
 * @param list Target list
 * @param index Index of element to set
 * @param element New element value
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at head
 * @note Requires traversal to index
 */
status_t singly_list_set(singly_list_t *list, size_t index,
                         const void *element);

/*
 * @brief Gets pointer to element at specified index (unsafe but fast)
 * @param list Target list
 * @param index Index of element
 * @return Pointer to element, NULL on error
 *
 * @note Time complexity: O(n) in worst case, O(1) at head
 * @warning Unsafe: returned pointer becomes invalid if list is modified
 */
void *singly_list_get_ref(const singly_list_t *list, size_t index);

/*
 * @brief Gets pointer to first element
 * @param list Target list
 * @return Pointer to first element, NULL if empty
 *
 * @note Time complexity: O(1)
 */
void *singly_list_front(const singly_list_t *list);

/*
 * @brief Gets pointer to last element
 * @param list Target list
 * @return Pointer to last element, NULL if empty
 *
 * @note Time complexity: O(1)
 */
void *singly_list_back(const singly_list_t *list);

/* ===== LIST PROPERTIES ===== */

/*
 * @brief Gets current number of elements in list
 * @param list Target list
 * @return Number of elements
 *
 * @note Time complexity: O(1)
 */
size_t singly_list_size(const singly_list_t *list);

/*
 * @brief Checks if list is empty
 * @param list Target list
 * @return true if empty, false otherwise
 *
 * @note Time complexity: O(1)
 */
bool singly_list_empty(const singly_list_t *list);

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
int singly_list_find(const singly_list_t *list, const void *element,
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
bool singly_list_contains(const singly_list_t *list, const void *element,
                          cmp_fn cmp);

/* ===== UTILITY OPERATIONS ===== */

/*
 * @brief Reverses the list in-place
 * @param list Target list
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) where n is list size
 * @note In-place reversal using iterative method
 */
status_t singly_list_reverse(singly_list_t *list);

/*
 * @brief Swaps contents of two lists
 * @param a First list
 * @param b Second list
 *
 * @note Time complexity: O(1)
 * @note Efficient pointer swapping without data copying
 */
void singly_list_swap(singly_list_t *a, singly_list_t *b);

/* ===== ITERATION ===== */

/*
 * @brief Iterator structure for singly linked list
 */
typedef struct
{
        singly_list_t *list;
        singly_node_t *current;
        size_t position;
} singly_list_iter_t;

/*
 * @brief Creates an iterator for the list
 * @param list Target list
 * @return Iterator structure
 *
 * @note Time complexity: O(1)
 */
singly_list_iter_t singly_list_iter_create(singly_list_t *list);

/*
 * @brief Checks if iterator has more elements
 * @param iter Iterator
 * @return true if more elements, false otherwise
 *
 * @note Time complexity: O(1)
 */
bool singly_list_iter_has_next(const singly_list_iter_t *iter);

/*
 * @brief Advances iterator and gets next element
 * @param iter Iterator
 * @param output Where to store the element (can be NULL)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) per element, O(n) for full traversal
 * @note Safe iteration with current state tracking
 */
status_t singly_list_iter_next(singly_list_iter_t *iter, void *output);

/*
 * @brief Removes current element from list (iterator must be valid)
 * @param iter Iterator
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) for current element removal
 * @warning Iterator becomes invalid after removal; must call iter_next next
 */
status_t singly_list_iter_remove(singly_list_iter_t *iter);

#endif /* CSTRUCTS_SINGLY_LIST_H */
