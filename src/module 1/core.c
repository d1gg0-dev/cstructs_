/*
 * @file core.c
 * @brief Implementation of Core Utilities and Memory Management
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Core Module
 * Provides memory management, generic types, and utility functions
 */

#include "../../include/module 1/core.h"
#include <stdio.h>
#include <stdlib.h>

/* ===== MEMORY MANAGEMENT IMPLEMENTATION ===== */

void *mem_alloc(size_t size)
{
    if (size == 0)
    {
        fprintf(stderr, "\n\nERROR: Attempted to allocate 0 bytes\n");
        return NULL;
    }

    void *ptr = malloc(size);
    if (ptr == NULL)
    {
        fprintf(stderr, "\n\nERROR: Memory allocation failed for %zu bytes\n",
                size);
    }

    return ptr;
}

void *mem_calloc(size_t count, size_t size)
{
    if (count == 0 || size == 0)
    {
        fprintf(stderr,
                "\n\nERROR: Attempted to allocate 0 elements or 0 size\n");
        return NULL;
    }

    // Check for potential overflow
    if (size > 0 && count > SIZE_MAX / size)
    {
        fprintf(stderr,
                "\n\nERROR: Memory allocation size overflow (%zu * %zu)\n",
                count, size);
        return NULL;
    }

    void *ptr = calloc(count, size);
    if (ptr == NULL)
    {
        fprintf(stderr,
                "\n\nERROR: Memory allocation failed for %zu elements of %zu "
                "bytes\n",
                count, size);
    }

    return ptr;
}

void *mem_realloc(void *ptr, size_t size)
{
    if (size == 0)
    {
        fprintf(stderr, "\n\nERROR: Attempted to reallocate to 0 bytes\n");
        return NULL;
    }

    if (ptr == NULL)
    {
        return mem_alloc(size);
    }

    void *new_ptr = realloc(ptr, size);
    if (new_ptr == NULL)
    {
        fprintf(stderr, "\n\nERROR: Memory reallocation failed for %zu bytes\n",
                size);
        // Note: Original pointer is still valid, but we can't help much here
    }

    return new_ptr;
}

void mem_free(void **ptr)
{
    if (ptr != NULL && *ptr != NULL)
    {
        free(*ptr);
        *ptr = NULL; // Prevent dangling pointer
    }
}

/* ===== MEMORY OPERATIONS IMPLEMENTATION ===== */

void *mem_copy(void *dest, const void *src, size_t count)
{
    if (dest == NULL || src == NULL || count == 0)
    {
        return dest;
    }

    // Use byte-by-byte copy to avoid alignment issues
    char *dest_bytes = (char *)dest;
    const char *src_bytes = (const char *)src;

    // When the region is small, copy byte-by-byte
    if (count < 16)
    {
        for (size_t i = 0; i < count; i++)
        {
            dest_bytes[i] = src_bytes[i];
        }

        return dest;
    }

    // For larger data, try aligning and copying in Word.
    size_t i = 0;

    // Copies bytes until aligned.
    while ((i < count) &&
           ((uintptr_t)(dest_bytes + i) % sizeof(uintptr_t) != 0))
    {
        dest_bytes[i] = src_bytes[i];
        i++;
    }

    // Copy aligned words.
    uintptr_t *dest_words = (uintptr_t *)(dest_bytes + i);
    const uintptr_t *src_words = (const uintptr_t *)(src_bytes + i);
    size_t word_count = (count - i) / sizeof(uintptr_t);

    for (size_t j = 0; j < word_count; j++)
    {
        dest_words[j] = src_words[j];
    }

    i += word_count * sizeof(uintptr_t);

    for (; i < count; i++)
    {
        dest_bytes[i] = src_bytes[i];
    }

    return dest;
}

void *mem_set(void *dest, int value, size_t count)
{
    if (dest == NULL || count == 0)
    {
        return dest;
    }

    char *dest_bytes = (char *)dest;
    for (size_t i = 0; i < count; i++)
    {
        dest_bytes[i] = (char)value;
    }

    return dest;
}

int mem_cmp(const void *ptr1, const void *ptr2, size_t count)
{
    if (ptr1 == NULL || ptr2 == NULL)
    {
        fprintf(stderr, "ERROR: mem_cmp called with NULL pointer\n");
        return 0;
    }

    if (count == 0)
        return 0;

    const unsigned char *p1 = (const unsigned char *)ptr1;
    const unsigned char *p2 = (const unsigned char *)ptr2;

    for (size_t i = 0; i < count; i++)
    {
        if (p1[i] != p2[i])
        {
            return (int)p1[i] - (int)p2[i];
        }
    }

    return 0;
}

void *mem_move(void *dest, const void *src, size_t count)
{
    if (dest == NULL || src == NULL || count == 0)
    {
        return dest;
    }

    char *dest_bytes = (char *)dest;
    const char *src_bytes = (const char *)src;

    // Handle overlapping regions by copying direction
    if (dest_bytes > src_bytes && dest_bytes < src_bytes + count)
    {
        // Overlap: copy from end to beginning
        for (size_t i = count; i > 0; i--)
        {
            dest_bytes[i - 1] = src_bytes[i - 1];
        }
    }
    else
    {
        // No overlap or dest before src: copy from beginning to end
        for (size_t i = 0; i < count; i++)
        {
            dest_bytes[i] = src_bytes[i];
        }
    }

    return dest;
}

/* ===== UTILITY FUNCTIONS IMPLEMENTATION ===== */

void mem_swap(void *a, void *b, size_t size)
{
    if (a == NULL || b == NULL || size == 0)
    {
        return;
    }

    // Use stack allocation for swapping
    if (size <= 64)
    {
        char temp[64];
        mem_copy(temp, a, size);
        mem_copy(a, b, size);
        mem_copy(b, temp, size);
    }
    // Use heap for swapping
    else
    {
        char *temp = (char *)mem_alloc(size);
        if (temp == NULL)
            return;

        // Performs swap with memory copy functions
        mem_copy(temp, a, size);
        mem_copy(a, b, size);
        mem_copy(b, temp, size);

        mem_free((void **)&temp);
    }
}

int cmp_max(int a, int b) { return (a > b) ? a : b; }

int cmp_min(int a, int b) { return (a < b) ? a : b; }

/* ===== COMPARISON FUNCTIONS IMPLEMENTATION ===== */

int cmp_fn_int(const void *a, const void *b)
{
    const int *int_a = (const int *)a;
    const int *int_b = (const int *)b;

    if (*int_a < *int_b)
        return -1;
    if (*int_a > *int_b)
        return 1;
    return 0;
}

int cmp_fn_float(const void *a, const void *b)
{
    const float *float_a = (const float *)a;
    const float *float_b = (const float *)b;

    if (*float_a < *float_b)
        return -1;
    if (*float_a > *float_b)
        return 1;
    return 0;
}

int cmp_fn_double(const void *a, const void *b)
{
    const double *double_a = (const double *)a;
    const double *double_b = (const double *)b;

    if (*double_a < *double_b)
        return -1;
    if (*double_a > *double_b)
        return 1;
    return 0;
}

int cmp_fn_string(const void *a, const void *b)
{
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;

    if (str_a == NULL && str_b == NULL)
        return 0;
    if (str_a == NULL)
        return -1;
    if (str_b == NULL)
        return 1;

    // Compare strings character by character
    while (*str_a && *str_b && *str_a == *str_b)
    {
        str_a++;
        str_b++;
    }

    return (*str_a > *str_b) - (*str_a < *str_b);
}

/* ===== DEBUGGING AND ERROR HANDLING IMPLEMENTATION ===== */

const char *str_error(status_t status)
{
    switch (status)
    {
    case SUCCESS:
        return "Success.";
    case ERROR_INVALID_INPUT:
        return "ERROR: Invalid input parameters.";
    case ERROR_MEMORY_ALLOCATION:
        return "ERROR: Memory allocation failed.";
    case ERROR_INDEX_OUT_OF_BOUNDS:
        return "ERROR: Index out of bounds.";
    case ERROR_EMPTY_CONTAINER:
        return "ERROR: Container is empty.";
    case ERROR_NOT_FOUND:
        return "ERROR: Element not found.";
    case ERROR_FULL_CONTAINER:
        return "ERROR: Container is full.";
    default:
        return "ERROR: Unknown error.";
    }
}

status_t check_condition(bool condition, status_t error_code)
{
    return condition ? SUCCESS : error_code;
}
