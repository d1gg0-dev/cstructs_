/*
 * @file vector.c
 * @brief Dynamic Vector Data Structure
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Vector Module
 * Provides a dynamic array that can grow and shrink as needed
 */

#include "../../include/module 2/vector.h"
#include <stdio.h>

/* ===== PRIVATE HELPER FUNCTIONS ===== */

/*
 * @brief Checks if vector needs to grow and resizes if necessary
 * @param vector Target vector
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) for check, O(n) for resize if needed
 * @note Automatically grows by VECTOR_GROWTH_FACTOR when capacity is reached
 * @note Ensures minimum capacity is at least VECTOR_INITIAL_CAPACITY
 */
static status_t vector_check_grow(vector_t *vector)
{
    if (vector->size >= vector->capacity)
    {
        size_t new_capacity = vector->capacity * VECTOR_GROWTH_FACTOR;
        if (new_capacity < VECTOR_INITIAL_CAPACITY)
        {
            new_capacity = VECTOR_INITIAL_CAPACITY;
        }
        if (new_capacity <= vector->capacity)
        {
            new_capacity = vector->capacity + 1;
        }
        return vector_reserve(vector, new_capacity);
    }
    return SUCCESS;
}

/*
 * @brief Checks if index is valid for the vector
 * @param vector Target vector
 * @param index Index to check
 * @return SUCCESS if valid, ERROR_INDEX_OUT_OF_BOUNDS if not
 *
 * @note Time complexity: O(1)
 * @note Used internally to validate indices before access operations
 * @note Ensures safe element access within vector bounds
 */
static status_t vector_check_index(const vector_t *vector, size_t index)
{
    if (index >= vector->size)
    {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }
    return SUCCESS;
}

/* ===== VECTOR CREATION AND DESTRUCTION ===== */

vector_t *vector_create(size_t element_size)
{
    if (element_size == 0)
    {
        fprintf(stderr, "Error: Cannot create vector with element size 0\n");
        return NULL;
    }

    return vector_create_with_capacity(element_size, VECTOR_INITIAL_CAPACITY);
}

vector_t *vector_create_with_capacity(size_t element_size,
                                      size_t initial_capacity)
{
    if (element_size == 0)
    {
        fprintf(stderr, "Error: Cannot create vector with element size 0\n");
        return NULL;
    }

    if (initial_capacity == 0)
    {
        initial_capacity = VECTOR_INITIAL_CAPACITY;
    }

    vector_t *vector = (vector_t *)mem_alloc(sizeof(vector_t));
    if (vector == NULL)
    {
        return NULL;
    }

    vector->data = mem_alloc(element_size * initial_capacity);
    if (vector->data == NULL)
    {
        mem_free((void **)&vector);
        return NULL;
    }

    vector->size = 0;
    vector->capacity = initial_capacity;
    vector->element_size = element_size;

    return vector;
}

void vector_destroy(vector_t *vector)
{
    if (vector != NULL)
    {
        mem_free((void **)&vector->data);
        mem_free((void **)&vector);
    }
}

vector_t *vector_copy(const vector_t *src)
{
    if (src == NULL)
    {
        return NULL;
    }

    vector_t *dest =
        vector_create_with_capacity(src->element_size, src->capacity);
    if (dest == NULL)
    {
        return NULL;
    }

    // Copy all elements
    for (size_t i = 0; i < src->size; i++)
    {
        const void *element = (const char *)src->data + (i * src->element_size);
        status_t result = vector_push_back(dest, element);
        if (result != SUCCESS)
        {
            vector_destroy(dest);
            return NULL;
        }
    }

    return dest;
}

/* ===== BASIC OPERATIONS ===== */

status_t vector_push_back(vector_t *vector, const void *element)
{
    if (vector == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    status_t result = vector_check_grow(vector);
    if (result != SUCCESS)
    {
        return result;
    }

    // Calculate position for new element
    void *dest = (char *)vector->data + (vector->size * vector->element_size);
    mem_copy(dest, element, vector->element_size);
    vector->size++;

    return SUCCESS;
}

status_t vector_pop_back(vector_t *vector, void *output)
{
    if (vector == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (vector->size == 0)
    {
        return ERROR_EMPTY_CONTAINER;
    }

    // If output is provided, copy the element before removing
    if (output != NULL)
    {
        void *src =
            (char *)vector->data + ((vector->size - 1) * vector->element_size);
        mem_copy(output, src, vector->element_size);
    }

    vector->size--;
    return SUCCESS;
}

status_t vector_insert(vector_t *vector, size_t index, const void *element)
{
    if (vector == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (index > vector->size)
    { // Allow inserting at the end (index == size)
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }

    status_t result = vector_check_grow(vector);
    if (result != SUCCESS)
    {
        return result;
    }

    // If inserting at the end, use push_back for efficiency
    if (index == vector->size)
    {
        return vector_push_back(vector, element);
    }

    // Shift elements to make space
    void *dest_start =
        (char *)vector->data + ((index + 1) * vector->element_size);
    void *src_start = (char *)vector->data + (index * vector->element_size);
    size_t bytes_to_move = (vector->size - index) * vector->element_size;

    mem_move(dest_start, src_start, bytes_to_move);

    // Insert new element
    void *insert_pos = (char *)vector->data + (index * vector->element_size);
    mem_copy(insert_pos, element, vector->element_size);
    vector->size++;

    return SUCCESS;
}

status_t vector_remove(vector_t *vector, size_t index)
{
    if (vector == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    status_t result = vector_check_index(vector, index);
    if (result != SUCCESS)
    {
        return result;
    }

    // If removing the last element, use pop_back
    if (index == vector->size - 1)
    {
        return vector_pop_back(vector, NULL);
    }

    // Shift elements to fill the gap
    void *dest_start = (char *)vector->data + (index * vector->element_size);
    void *src_start =
        (char *)vector->data + ((index + 1) * vector->element_size);
    size_t bytes_to_move = (vector->size - index - 1) * vector->element_size;

    mem_move(dest_start, src_start, bytes_to_move);
    vector->size--;

    return SUCCESS;
}

void vector_clear(vector_t *vector)
{
    if (vector != NULL)
    {
        vector->size = 0;
    }
}

/* ===== ELEMENT ACCESS ===== */

status_t vector_get(const vector_t *vector, size_t index, void *output)
{
    if (vector == NULL || output == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    status_t result = vector_check_index(vector, index);
    if (result != SUCCESS)
    {
        return result;
    }

    void *src = (const char *)vector->data + (index * vector->element_size);
    mem_copy(output, src, vector->element_size);

    return SUCCESS;
}

status_t vector_set(vector_t *vector, size_t index, const void *element)
{
    if (vector == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    status_t result = vector_check_index(vector, index);
    if (result != SUCCESS)
    {
        return result;
    }

    void *dest = (char *)vector->data + (index * vector->element_size);
    mem_copy(dest, element, vector->element_size);

    return SUCCESS;
}

const void *vector_at_const(const vector_t *vector, size_t index)
{
    if (vector == NULL)
    {
        return NULL;
    }

    if (index >= vector->size)
    {
        return NULL;
    }

    return (char *)vector->data + (index * vector->element_size);
}

void *vector_at_mutable(vector_t *vector, size_t index)
{
    if (vector == NULL)
    {
        return NULL;
    }

    if (index >= vector->size)
    {
        return NULL;
    }

    return (char *)vector->data + (index * vector->element_size);
}

void *vector_front(vector_t *vector)
{
    if (vector == NULL || vector->size == 0)
    {
        return NULL;
    }

    return vector->data;
}

void *vector_back(vector_t *vector)
{
    if (vector == NULL || vector->size == 0)
    {
        return NULL;
    }

    return (char *)vector->data + ((vector->size - 1) * vector->element_size);
}

/* ===== CAPACITY MANAGEMENT ===== */

size_t vector_size(const vector_t *vector)
{
    return vector != NULL ? vector->size : 0;
}

size_t vector_capacity(const vector_t *vector)
{
    return vector != NULL ? vector->capacity : 0;
}

bool vector_empty(const vector_t *vector)
{
    return vector == NULL || vector->size == 0;
}

status_t vector_reserve(vector_t *vector, size_t new_capacity)
{
    if (vector == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (new_capacity <= vector->capacity)
    {
        return SUCCESS; // No need to reserve less capacity
    }

    void *new_data =
        mem_realloc(vector->data, new_capacity * vector->element_size);
    if (new_data == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    vector->data = new_data;
    vector->capacity = new_capacity;

    return SUCCESS;
}

status_t vector_shrink_to_fit(vector_t *vector)
{
    if (vector == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (vector->size == vector->capacity)
    {
        return SUCCESS; // Already at minimum capacity
    }

    if (vector->size == 0)
    {
        mem_free((void **)&vector->data);
        vector->capacity = 0;
        return SUCCESS;
    }

    void *new_data =
        mem_realloc(vector->data, vector->size * vector->element_size);
    if (new_data == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    vector->data = new_data;
    vector->capacity = vector->size;

    return SUCCESS;
}

/* ===== SEARCH AND UTILITIES ===== */

int vector_find(const vector_t *vector, const void *element, cmp_fn cmp)
{
    if (vector == NULL || element == NULL || cmp == NULL)
    {
        return -1;
    }

    for (size_t i = 0; i < vector->size; i++)
    {
        void *current = (char *)vector->data + (i * vector->element_size);
        if (cmp(current, element) == 0)
        {
            return (int)i;
        }
    }

    return -1;
}

bool vector_contains(const vector_t *vector, const void *element, cmp_fn cmp)
{
    return vector_find(vector, element, cmp) >= 0;
}

void vector_swap(vector_t *a, vector_t *b)
{
    if (a == NULL || b == NULL)
    {
        return;
    }

    // Swap all fields
    void *temp_data = a->data;
    a->data = b->data;
    b->data = temp_data;

    size_t temp_size = a->size;
    a->size = b->size;
    b->size = temp_size;

    size_t temp_capacity = a->capacity;
    a->capacity = b->capacity;
    b->capacity = temp_capacity;

    size_t temp_element_size = a->element_size;
    a->element_size = b->element_size;
    b->element_size = temp_element_size;
}

/* ===== ITERATION ===== */

void vector_for_each(vector_t *vector, void (*func)(void *element))
{
    if (vector == NULL || func == NULL)
    {
        return;
    }

    for (size_t i = 0; i < vector->size; i++)
    {
        void *element = (char *)vector->data + (i * vector->element_size);
        func(element);
    }
}
