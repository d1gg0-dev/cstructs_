/*
 * @file stack.h
 * @brief Stack Data Structure (LIFO)
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Stack Module
 * Provides stack implementation using both arrays and linked lists
 */

#ifndef CSTRUCTS_STACK_H
#define CSTRUCTS_STACK_H

#include "../module 1/core.h"
#include "../module 2/vector.h"
#include "../module 3/singly_list.h"
#include <stdbool.h>

/* ===== STACK USING ARRAY (VECTOR) ===== */

/*
 * @brief Stack implemented using dynamic array
 */
typedef struct
{
        vector_t *vector; // Using vector as underlying storage
} stack_array_t;

/* ===== STACK USING LINKED LIST ===== */

/*
 * @brief Stack implemented using singly linked list
 */
typedef struct
{
        singly_list_t *list; // Using singly linked list as underlying storage
} stack_list_t;

/* ===== ARRAY STACK OPERATIONS ===== */

/*
 * @brief Creates a new array-based stack with default capacity
 * @param element_size Size of each element in bytes
 * @return Pointer to new stack, NULL on failure
 *
 * @note Uses vector as underlying storage for efficient memory management
 * @note Default capacity is defined in vector implementation
 */
stack_array_t *stack_array_create(size_t element_size);

/*
 * @brief Creates a new array-based stack with specified capacity
 * @param element_size Size of each element in bytes
 * @param capacity Initial capacity of the stack
 * @return Pointer to new stack, NULL on failure
 *
 * @note Pre-allocating capacity can improve performance for known stack sizes
 * @note Element size must be greater than 0
 */
stack_array_t *stack_array_create_with_capacity(size_t element_size,
                                                size_t capacity);
/*
 * @brief Destroys an array-based stack and frees all associated memory
 * @param stack Pointer to stack to destroy
 *
 * @note Safely handles NULL pointers
 * @note Frees both the stack structure and underlying vector
 */
void stack_array_destroy(stack_array_t *stack);

/*
 * @brief Pushes an element onto the top of the stack
 * @param stack Target stack
 * @param element Element to push
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) amortized (due to vector resizing)
 * @note Element is copied into stack storage
 */
status_t stack_array_push(stack_array_t *stack, const void *element);

/*
 * @brief Pops the top element from the stack
 * @param stack Target stack
 * @param output Where to store the popped element (can be NULL if not needed)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note If output is NULL, element is still removed but not returned
 */
status_t stack_array_pop(stack_array_t *stack, void *output);

/*
 * @brief Peeks at the top element without removing it
 * @param stack Target stack
 * @param output Where to store the top element
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Does not modify the stack
 */
status_t stack_array_peek(const stack_array_t *stack, void *output);

/*
 * @brief Gets direct pointer to top element without copying (unsafe but fast)
 * @param stack Target stack
 * @return Pointer to top element, NULL if empty or error
 *
 * @note Time complexity: O(1)
 * @warning Unsafe: returned pointer becomes invalid if stack is modified
 * @note Useful for performance-critical code that doesn't need safety
 */
void *stack_array_peek_ref(const stack_array_t *stack);

/*
 * @brief Gets current number of elements in stack
 * @param stack Target stack
 * @return Number of elements, 0 if stack is NULL
 *
 * @note Time complexity: O(1)
 */
size_t stack_array_size(const stack_array_t *stack);

/*
 * @brief Checks if stack is empty
 * @param stack Target stack
 * @return true if empty, false otherwise
 *
 * @note Time complexity: O(1)
 */
bool stack_array_empty(const stack_array_t *stack);

/*
 * @brief Gets current capacity of stack
 * @param stack Target stack
 * @return Current capacity, 0 if stack is NULL
 *
 * @note Time complexity: O(1)
 * @note Capacity is the maximum number of elements before resizing
 */
size_t stack_array_capacity(const stack_array_t *stack);

/*
 * @brief Reserves capacity for at least specified number of elements
 * @param stack Target stack
 * @param capacity New capacity
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) where n is current size
 * @note Can improve performance by reducing reallocations
 */
status_t stack_array_reserve(stack_array_t *stack, size_t capacity);

/* ===== LINKED LIST STACK OPERATIONS ===== */

/*
 * @brief Creates a new linked list-based stack
 * @param element_size Size of each element in bytes
 * @return Pointer to new stack, NULL on failure
 *
 * @note Uses singly linked list with head insertion for O(1) operations
 * @note No capacity limits, but more memory overhead per element
 */
stack_list_t *stack_list_create(size_t element_size);

/*
 * @brief Destroys a linked list-based stack and frees all memory
 * @param stack Pointer to stack to destroy
 *
 * @note Safely handles NULL pointers
 * @note Frees all nodes and the stack structure
 */
void stack_list_destroy(stack_list_t *stack);

/*
 * @brief Pushes an element onto the top of the stack
 * @param stack Target stack
 * @param element Element to push
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Uses head insertion for constant time operation
 */
status_t stack_list_push(stack_list_t *stack, const void *element);

/*
 * @brief Pops the top element from the stack
 * @param stack Target stack
 * @param output Where to store the popped element (can be NULL if not needed)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Removes from head (top of stack)
 */
status_t stack_list_pop(stack_list_t *stack, void *output);

/*
 * @brief Peeks at the top element without removing it
 * @param stack Target stack
 * @param output Where to store the top element
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Accesses head element without modification
 */
status_t stack_list_peek(const stack_list_t *stack, void *output);

/*
 * @brief Gets direct pointer to top element without copying (unsafe but fast)
 * @param stack Target stack
 * @return Pointer to top element, NULL if empty or error
 *
 * @note Time complexity: O(1)
 * @warning Unsafe: returned pointer becomes invalid if stack is modified
 */
void *stack_list_peek_ref(const stack_list_t *stack);

/*
 * @brief Gets current number of elements in stack
 * @param stack Target stack
 * @return Number of elements, 0 if stack is NULL
 *
 * @note Time complexity: O(1)
 */
size_t stack_list_size(const stack_list_t *stack);

/*
 * @brief Checks if stack is empty
 * @param stack Target stack
 * @return true if empty, false otherwise
 *
 * @note Time complexity: O(1)
 */
bool stack_list_empty(const stack_list_t *stack);

/* ===== COMMON STACK OPERATIONS (MACROS FOR CONVENIENCE) ===== */

/*
 * @brief Generic push operation (works with both array and list stacks)
 */
#define STACK_PUSH(stack, element)                                             \
    _Generic((stack),                                                          \
        stack_array_t *: stack_array_push,                                     \
        stack_list_t *: stack_list_push)(stack, element)

/*
 * @brief Generic pop operation (works with both array and list stacks)
 */
#define STACK_POP(stack, output)                                               \
    _Generic((stack),                                                          \
        stack_array_t *: stack_array_pop,                                      \
        stack_list_t *: stack_list_pop)(stack, output)

/*
 * @brief Generic peek operation (works with both array and list stacks)
 */
#define STACK_PEEK(stack, output)                                              \
    _Generic((stack),                                                          \
        stack_array_t *: stack_array_peek,                                     \
        stack_list_t *: stack_list_peek)(stack, output)

/*
 * @brief Generic size operation (works with both array and list stacks)
 */
#define STACK_SIZE(stack)                                                      \
    _Generic((stack),                                                          \
        stack_array_t *: stack_array_size,                                     \
        stack_list_t *: stack_list_size)(stack)

/*
 * @brief Generic empty check (works with both array and list stacks)
 */
#define STACK_EMPTY(stack)                                                     \
    _Generic((stack),                                                          \
        stack_array_t *: stack_array_empty,                                    \
        stack_list_t *: stack_list_empty)(stack)

#endif /* CSTRUCTS_STACK_H */
