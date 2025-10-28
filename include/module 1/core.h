/*
 * @file core.h
 * @brief Core Utilities and Fundamental Data Dtructures
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Core Module
 * Provides memory management, generic types, and utility functions
 */

#ifndef CSTRUCTS_CORE_H
#define CSTRUCTS_CORE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ===== ERROR CODES ===== */

typedef enum
{
    SUCCESS = 0,
    ERROR_INVALID_INPUT = -1,
    ERROR_MEMORY_ALLOCATION = -2,
    ERROR_INDEX_OUT_OF_BOUNDS = -3,
    ERROR_EMPTY_CONTAINER = -4,
    ERROR_NOT_FOUND = -5,
    ERROR_FULL_CONTAINER = -6
} status_t;

/* ===== GENERIC DATA TYPE ===== */

typedef union
{
        int32_t int_value;
        int64_t long_value;
        float float_value;
        double double_value;
        char char_value;
        bool bool_value;
        void *ptr_value;
} data_t;

/* ===== MEMORY MANAGEMENT ===== */

/*
 * @brief Allocates memory with error checking
 * @param size Number of bytes to allocate
 * @return Pointer to allocated memory, NULL on failure
 *
 * @note Logs error message on allocation failure
 * @note Time complexity: O(1)
 * @warning Returns NULL if allocation fails or size is 0
 */
void *mem_alloc(size_t size);

/*
 * @brief Allocates and zero-initializes memory with error checking
 * @param count Number of elements to allocate
 * @param size Size of each element
 * @return Pointer to allocated memory, NULL on failure
 *
 * @note Time complexity: O(n) where n is count * size
 * @warning Returns NULL if allocation fails, count/size is 0, or overflow
 * occurs
 */
void *mem_calloc(size_t count, size_t size);

/*
 * @brief Reallocates memory with error checking
 * @param ptr Pointer to existing memory block
 * @param size New size in bytes
 * @return Pointer to reallocated memory, NULL on failure
 *
 * @note Time complexity: O(n) where n is min(old_size, new_size) for data copy
 * @warning Returns NULL if reallocation fails; original pointer remains valid
 */
void *mem_realloc(void *ptr, size_t size);

/*
 * @brief Safely frees memory and sets pointer to NULL
 * @param ptr Pointer to pointer of memory to free
 *
 * @note Time complexity: O(1)
 * @warning Safely sets pointer to NULL after freeing to prevent dangling
 * pointers
 */
void mem_free(void **ptr);

/* ===== MEMORY OPERATIONS (OUR OWN IMPLEMENTATIONS) ===== */

/*
 * @brief Copies memory from source to destination (our own memcpy)
 * @param dest Destination memory address
 * @param src Source memory address
 * @param count Number of bytes to copy
 * @return Pointer to destination
 *
 * @note Time complexity: O(n) where n is count
 * @warning Does not handle overlapping regions; use mem_move for that
 */
void *mem_copy(void *dest, const void *src, size_t count);

/*
 * @brief Sets memory to specified value (our own memset)
 * @param dest Destination memory address
 * @param value Value to set
 * @param count Number of bytes to set
 * @return Pointer to destination
 *
 * @note Time complexity: O(n) where n is count
 * @note Efficient byte-by-byte setting without alignment assumptions
 */
void *mem_set(void *dest, int value, size_t count);

/*
 * @brief Compares two memory regions (our own memcmp)
 * @param ptr1 First memory region
 * @param ptr2 Second memory region
 * @param count Number of bytes to compare
 * @return 0 if equal, negative if ptr1 < ptr2, positive if ptr1 > ptr2
 *
 * @note Time complexity: O(n) where n is count
 * @note Returns difference at first mismatch for early termination
 */
int mem_cmp(const void *ptr1, const void *ptr2, size_t count);

/*
 * @brief Copies memory handling overlapping regions (our own memmove)
 * @param dest Destination memory address
 * @param src Source memory address
 * @param count Number of bytes to copy
 * @return Pointer to destination
 *
 * @note Time complexity: O(n) where n is count
 * @note Handles overlapping regions correctly by copying direction
 */
void *mem_move(void *dest, const void *src, size_t count);

/* ===== UTILITY FUNCTIONS ===== */

/*
 * @brief Swaps two memory regions of specified size
 * @param a First element
 * @param b Second element
 * @param size Size of each element in bytes
 *
 * @note Time complexity: O(n) where n is size
 * @note Uses temporary buffer; inefficient for large sizes but safe
 */
void mem_swap(void *a, void *b, size_t size);

/*
 * @brief Returns maximum of two integers
 * @param a First integer
 * @param b Second integer
 * @return Maximum value
 *
 * @note Time complexity: O(1)
 */
int cmp_max(int a, int b);

/*
 * @brief Returns minimum of two integers
 * @param a First integer
 * @param b Second integer
 * @return Minimum value
 *
 * @note Time complexity: O(1)
 */
int cmp_min(int a, int b);

/*
 * @brief Gets the size of a statically allocated array
 * @param array The array
 * @return Number of elements in the array
 */
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))

/* ===== TYPE-SAFE COMPARISON FUNCTIONS ===== */

/*
 * @brief Compare function type for generic data comparison
 * @param a First element
 * @param b Second element
 * @return Negative if a < b, 0 if a == b, positive if a > b
 */
typedef int (*cmp_fn)(const void *a, const void *b);

/*
 * @brief Compare function for integers
 *
 * @note Time complexity: O(1)
 */
int cmp_fn_int(const void *a, const void *b);

/*
 * @brief Compare function for floats
 *
 * @note Time complexity: O(1)
 */
int cmp_fn_float(const void *a, const void *b);

/*
 * @brief Compare function for doubles
 *
 * @note Time complexity: O(1)
 */
int cmp_fn_double(const void *a, const void *b);

/*
 * @brief Compare function for strings
 *
 * @note Time complexity: O(n) where n is string length
 * @note Handles NULL strings safely
 */
int cmp_fn_string(const void *a, const void *b);

/* ===== DEBUGGING AND ERROR HANDLING ===== */

/*
 * @brief Gets string representation of error code
 * @param status Error code
 * @return String describing the error
 *
 * @note Time complexity: O(1)
 */
const char *str_error(status_t status);

/*
 * @brief Asserts a condition and returns error code if false
 * @param condition Condition to check
 * @param error_code Error code to return if condition is false
 * @return DS_SUCCESS if condition true, error_code otherwise
 *
 * @note Time complexity: O(1)
 */
status_t check_condition(bool condition, status_t error_code);

#endif /* CSTRUCTS_CORE_H */
