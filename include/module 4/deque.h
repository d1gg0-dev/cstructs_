/*
 * @file deque.h
 * @brief Double-Ended Queue (Deque) Data Structure
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Deque Module
 * Provides deque implementation using doubly linked list
 */

#ifndef CSTRUCTS_DEQUE_H
#define CSTRUCTS_DEQUE_H

#include "../module 1/core.h"
#include "../module 3/doubly_list.h"
#include <stdbool.h>

/* ===== DEQUE USING DOUBLY LINKED LIST ===== */

/*
 * @brief Deque implemented using doubly linked list
 */
typedef struct
{
        doubly_list_t *list; // Using doubly linked list as underlying storage
} deque_t;

/* ===== DEQUE OPERATIONS ===== */

/*
 * @brief Creates a new deque
 * @param element_size Size of each element in bytes
 * @return Pointer to new deque, NULL on failure
 *
 * @note Uses doubly linked list for O(1) operations at both ends
 * @note No capacity limits, flexible insertion/removal anywhere
 */
deque_t *deque_create(size_t element_size);

/*
 * @brief Destroys a deque and frees all memory
 * @param deque Pointer to deque to destroy
 *
 * @note Safely handles NULL pointers
 * @note Frees all nodes and the deque structure
 */
void deque_destroy(deque_t *deque);

/*
 * @brief Adds an element to the front of the deque
 * @param deque Target deque
 * @param element Element to push
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Efficient front insertion using doubly linked list
 */
status_t deque_push_front(deque_t *deque, const void *element);

/*
 * @brief Adds an element to the back of the deque
 * @param deque Target deque
 * @param element Element to push
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Efficient back insertion using doubly linked list
 */
status_t deque_push_back(deque_t *deque, const void *element);

/*
 * @brief Removes and returns the front element from the deque
 * @param deque Target deque
 * @param output Where to store the popped element (can be NULL if not needed)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Efficient front removal using doubly linked list
 */
status_t deque_pop_front(deque_t *deque, void *output);

/*
 * @brief Removes and returns the back element from the deque
 * @param deque Target deque
 * @param output Where to store the popped element (can be NULL if not needed)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Efficient back removal using doubly linked list
 */
status_t deque_pop_back(deque_t *deque, void *output);

/*
 * @brief Peeks at the front element without removing it
 * @param deque Target deque
 * @param output Where to store the front element
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Does not modify the deque
 */
status_t deque_peek_front(const deque_t *deque, void *output);

/*
 * @brief Peeks at the back element without removing it
 * @param deque Target deque
 * @param output Where to store the back element
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Does not modify the deque
 */
status_t deque_peek_back(const deque_t *deque, void *output);

/*
 * @brief Gets direct pointer to front element without copying (unsafe but fast)
 * @param deque Target deque
 * @return Pointer to front element, NULL if empty or error
 *
 * @note Time complexity: O(1)
 * @warning Unsafe: returned pointer becomes invalid if deque is modified
 */
void *deque_peek_front_ref(const deque_t *deque);

/*
 * @brief Gets direct pointer to back element without copying (unsafe but fast)
 * @param deque Target deque
 * @return Pointer to back element, NULL if empty or error
 *
 * @note Time complexity: O(1)
 * @warning Unsafe: returned pointer becomes invalid if deque is modified
 */
void *deque_peek_back_ref(const deque_t *deque);

/*
 * @brief Gets current number of elements in deque
 * @param deque Target deque
 * @return Number of elements, 0 if deque is NULL
 *
 * @note Time complexity: O(1)
 */
size_t deque_size(const deque_t *deque);

/*
 * @brief Checks if deque is empty
 * @param deque Target deque
 * @return true if empty, false otherwise
 *
 * @note Time complexity: O(1)
 */
bool deque_empty(const deque_t *deque);

/*
 * @brief Inserts an element at specified index
 * @param deque Target deque
 * @param index Index where to insert
 * @param element Element to insert
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at ends
 * @note Supports insertion at any position in the deque
 */
status_t deque_insert(deque_t *deque, size_t index, const void *element);

/*
 * @brief Removes element at specified index
 * @param deque Target deque
 * @param index Index of element to remove
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at ends
 * @note Supports removal from any position in the deque
 */
status_t deque_remove(deque_t *deque, size_t index);

/*
 * @brief Gets element at specified index
 * @param deque Target deque
 * @param index Index of element
 * @param output Where to store the element
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at ends
 * @note Supports random access to any element
 */
status_t deque_get(deque_t *deque, size_t index, void *output);

/*
 * @brief Sets element at specified index
 * @param deque Target deque
 * @param index Index of element to set
 * @param element New element value
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) in worst case, O(1) at ends
 * @note Supports random modification of any element
 */
status_t deque_set(deque_t *deque, size_t index, const void *element);

#endif /* CSTRUCTS_DEQUE_H */
