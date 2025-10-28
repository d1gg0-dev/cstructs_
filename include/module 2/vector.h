/*
 * @file vector.h
 * @brief Dynamic Vector Data Structure
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Vector Module
 * Provides a dynamic array that can grow and shrink as needed
 */

#ifndef CSTRUCTS_VECTOR_H
#define CSTRUCTS_VECTOR_H

#include "../module 1/core.h"
#include <stdbool.h>

/* ===== CONSTANTS ===== */

#define VECTOR_INITIAL_CAPACITY 16
#define VECTOR_GROWTH_FACTOR 1.5

/* ===== VECTOR STRUCTURE ===== */

typedef struct
{
        void *data;          // Pointer to the actual data storage
        size_t size;         // Current number of elements in vector
        size_t capacity;     // Total allocated capacity
        size_t element_size; // Size of each element in bytes
} vector_t;

/* ===== VECTOR CREATION AND DESTRUCTION ===== */

/*
 * @brief Creates a new vector with specified element size
 * @param element_size Size of each element in bytes
 * @return Pointer to new vector, NULL on failure
 *
 * @note Time complexity: O(1)
 * @note Uses default initial capacity for memory efficiency
 */
vector_t *vector_create(size_t element_size);

/*
 * @brief Creates a new vector with initial capacity
 * @param element_size Size of each element in bytes
 * @param initial_capacity Initial capacity of the vector
 * @return Pointer to new vector, NULL on failure
 *
 * @note Time complexity: O(1)
 * @note Pre-allocating capacity can improve performance for known size
 */
vector_t *vector_create_with_capacity(size_t element_size,
                                      size_t initial_capacity);

/*
 * @brief Destroys a vector and frees all associated memory
 * @param vector Pointer to vector to destroy
 *
 * @note Time complexity: O(1) for structure, O(n) for elements if cleanup
 * needed
 * @note Frees all allocated memory including internal array
 */
void vector_destroy(vector_t *vector);

/*
 * @brief Creates a deep copy of a vector
 * @param src Vector to copy
 * @return New vector with copied data, NULL on failure
 *
 * @note Time complexity: O(n) where n is source size
 * @note Creates deep copy of all elements
 */
vector_t *vector_copy(const vector_t *src);

/* ===== BASIC OPERATIONS ===== */

/*
 * @brief Appends an element to the end of the vector
 * @param vector Target vector
 * @param element Element to append (must be of correct size)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) amortized (due to occasional resizing)
 * @note Automatically grows when capacity is reached
 */
status_t vector_push_back(vector_t *vector, const void *element);

/*
 * @brief Removes and returns the last element
 * @param vector Target vector
 * @param output Where to store the popped element (can be NULL if not needed)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Does not reduce capacity; use shrink_to_fit if needed
 */
status_t vector_pop_back(vector_t *vector, void *output);

/*
 * @brief Inserts an element at specified index
 * @param vector Target vector
 * @param index Index where to insert
 * @param element Element to insert
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) where n is elements after insertion point
 * @note Shifts elements to make space
 */
status_t vector_insert(vector_t *vector, size_t index, const void *element);

/*
 * @brief Removes element at specified index
 * @param vector Target vector
 * @param index Index of element to remove
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) where n is elements after removal point
 * @note Shifts elements to fill gap
 */
status_t vector_remove(vector_t *vector, size_t index);

/*
 * @brief Removes all elements from vector (does not free memory)
 * @param vector Target vector
 *
 * @note Time complexity: O(1) for size reset, O(n) if element cleanup needed
 * @note Does not free memory; retains capacity for reuse
 */
void vector_clear(vector_t *vector);

/* ===== ELEMENT ACCESS ===== */

/*
 * @brief Gets element at specified index
 * @param vector Target vector
 * @param index Index of element
 * @param output Where to store the element
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Safe access with bounds checking
 */
status_t vector_get(const vector_t *vector, size_t index, void *output);

/*
 * @brief Sets element at specified index
 * @param vector Target vector
 * @param index Index of element to set
 * @param element New element value
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Direct element modification with bounds checking
 */
status_t vector_set(vector_t *vector, size_t index, const void *element);

/*
 * @brief Gets pointer to element at specified index
 * @param vector Target vector
 * @param index Index of element
 * @return Pointer to element, NULL on error
 *
 * @note Time complexity: O(1)
 * @warning Unsafe: returned pointer becomes invalid if vector is resized
 */
const void *vector_at_const(const vector_t *vector, size_t index);

/*
 * @brief Gets pointer to element at specified index (unsafe but fast)
 * @param vector Target vector
 * @param index Index of element
 * @return Pointer to element, NULL on error
 *
 * @note Time complexity: O(1)
 * @warning Unsafe: returned pointer becomes invalid if vector is resized
 */
void *vector_at_mutable(vector_t *vector, size_t index);

/*
 * @brief Gets pointer to first element
 * @param vector Target vector
 * @return Pointer to first element, NULL if empty
 *
 * @note Time complexity: O(1)
 */
void *vector_front(vector_t *vector);

/*
 * @brief Gets pointer to last element
 * @param vector Target vector
 * @return Pointer to last element, NULL if empty
 *
 * @note Time complexity: O(1)
 */
void *vector_back(vector_t *vector);

/* =====CAPACITY MANAGEMENT ===== */

/*
 * @brief Gets current number of elements in vector
 * @param vector Target vector
 * @return Number of elements
 *
 * @note Time complexity: O(1)
 */
size_t vector_size(const vector_t *vector);

/*
 * @brief Gets current capacity of vector
 * @param vector Target vector
 * @return Current capacity
 *
 * @note Time complexity: O(1)
 */
size_t vector_capacity(const vector_t *vector);

/*
 * @brief Checks if vector is empty
 * @param vector Target vector
 * @return true if empty, false otherwise
 * @note Time complexity: O(1)
 */
bool vector_empty(const vector_t *vector);

/*
 * @brief Reserves capacity for at least specified number of elements
 * @param vector Target vector
 * @param new_capacity New capacity
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) where n is current size (for data copy)
 * @note Can prevent reallocations during multiple push operations
 */
status_t vector_reserve(vector_t *vector, size_t new_capacity);

/*
 * @brief Reduces capacity to fit current size
 * @param vector Target vector
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) where n is current size (for data copy)
 * @note Reduces memory usage when growth is complete
 */
status_t vector_shrink_to_fit(vector_t *vector);

/* ===== SEARCH AND UTILITIES ===== */

/*
 * @brief Finds first occurrence of element in vector
 * @param vector Target vector
 * @param element Element to find
 * @param cmp Comparison function
 * @return Index of element if found, -1 if not found
 *
 * @note Time complexity: O(n) where n is vector size
 * @note Linear search; returns first occurrence
 */
int vector_find(const vector_t *vector, const void *element, cmp_fn cmp);

/*
 * @brief Checks if vector contains element
 * @param vector Target vector
 * @param element Element to check
 * @param cmp Comparison function
 * @return true if found, false otherwise
 *
 * @note Time complexity: O(n) where n is vector size
 * @note Uses linear search for existence check
 */
bool vector_contains(const vector_t *vector, const void *element, cmp_fn cmp);

/*
 * @brief Swaps contents of two vectors
 * @param a First vector
 * @param b Second vector
 *
 * @note Time complexity: O(1)
 * @note Efficient pointer swapping without data copying
 */
void vector_swap(vector_t *a, vector_t *b);

/* ===== ITERATION ===== */

/*
 * @brief Applies function to each element in vector
 * @param vector Target vector
 * @param func Function to apply (takes element pointer and returns void)
 *
 * @note Time complexity: O(n) where n is vector size
 * @note Applies function to each element in order
 */
void vector_for_each(vector_t *vector, void (*func)(void *element));

#endif /* CSTRUCTS_VECTOR_H */
