/*
 * @file stack.c
 * @brief Implementation of Stack Data Structure
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Stack Module
 * Provides stack implementation using both arrays and linked lists
 */

#include "../../include/module 4/stack.h"
#include <stdio.h>

/* ===== ARRAY STACK IMPLEMENTATION ===== */

stack_array_t *stack_array_create(size_t element_size)
{
    return stack_array_create_with_capacity(element_size,
                                            VECTOR_INITIAL_CAPACITY);
}

stack_array_t *stack_array_create_with_capacity(size_t element_size,
                                                size_t capacity)
{
    // Validate input parameters
    if (element_size == 0)
    {
        fprintf(stderr, "Error: Cannot create stack with element size 0\n");
        return NULL;
    }

    // Allocate stack structure
    stack_array_t *stack = (stack_array_t *)mem_alloc(sizeof(stack_array_t));
    if (stack == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate stack structure\n");
        return NULL;
    }

    // Create underlying vector with specified capacity
    stack->vector = vector_create_with_capacity(element_size, capacity);
    if (stack->vector == NULL)
    {
        fprintf(stderr,
                "Error: Failed to create underlying vector for stack\n");
        mem_free((void **)&stack);
        return NULL;
    }

    return stack;
}

void stack_array_destroy(stack_array_t *stack)
{
    if (stack != NULL)
    {
        vector_destroy(stack->vector);
        mem_free((void **)&stack);
    }
}

status_t stack_array_push(stack_array_t *stack, const void *element)
{
    // Validate input parameters
    if (stack == NULL || element == NULL)
    {
        fprintf(stderr, "Error: Invalid input parameters for stack push\n");
        return ERROR_INVALID_INPUT;
    }

    // Use vector's push_back for O(1) amortized insertion
    return vector_push_back(stack->vector, element);
}

status_t stack_array_pop(stack_array_t *stack, void *output)
{
    // Validate input parameters
    if (stack == NULL)
    {
        fprintf(stderr, "Error: Invalid stack pointer for pop operation\n");
        return ERROR_INVALID_INPUT;
    }

    // Use vector's pop_back for O(1) removal
    return vector_pop_back(stack->vector, output);
}

status_t stack_array_peek(const stack_array_t *stack, void *output)
{
    // Validate input parameters
    if (stack == NULL || output == NULL)
    {
        fprintf(stderr, "Error: Invalid input parameters for stack peek\n");
        return ERROR_INVALID_INPUT;
    }

    // Check if stack is empty
    if (vector_empty(stack->vector))
    {
        fprintf(stderr, "Error: Cannot peek from empty stack\n");
        return ERROR_EMPTY_CONTAINER;
    }

    // Get the last element (top of stack)
    return vector_get(stack->vector, vector_size(stack->vector) - 1, output);
}

void *stack_array_peek_ref(const stack_array_t *stack)
{
    if (stack == NULL || vector_empty(stack->vector))
    {
        return NULL;
    }

    return vector_back(stack->vector);
}

size_t stack_array_size(const stack_array_t *stack)
{
    return stack != NULL ? vector_size(stack->vector) : 0;
}

bool stack_array_empty(const stack_array_t *stack)
{
    return stack == NULL || vector_empty(stack->vector);
}

size_t stack_array_capacity(const stack_array_t *stack)
{
    return stack != NULL ? vector_capacity(stack->vector) : 0;
}

status_t stack_array_reserve(stack_array_t *stack, size_t capacity)
{
    if (stack == NULL)
    {
        fprintf(stderr, "Error: Invalid stack pointer for reserve operation\n");
        return ERROR_INVALID_INPUT;
    }

    return vector_reserve(stack->vector, capacity);
}

/* ===== LINKED LIST STACK IMPLEMENTATION ===== */

stack_list_t *stack_list_create(size_t element_size)
{
    // Validate input parameters
    if (element_size == 0)
    {
        fprintf(stderr, "Error: Cannot create stack with element size 0\n");
        return NULL;
    }

    // Allocate stack structure
    stack_list_t *stack = (stack_list_t *)mem_alloc(sizeof(stack_list_t));
    if (stack == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate stack structure\n");
        return NULL;
    }

    // Create underlying linked list
    stack->list = singly_list_create(element_size);
    if (stack->list == NULL)
    {
        fprintf(stderr, "Error: Failed to create underlying list for stack\n");
        mem_free((void **)&stack);
        return NULL;
    }

    return stack;
}

void stack_list_destroy(stack_list_t *stack)
{
    if (stack != NULL)
    {
        singly_list_destroy(stack->list);
        mem_free((void **)&stack);
    }
}

status_t stack_list_push(stack_list_t *stack, const void *element)
{
    // Validate input parameters
    if (stack == NULL || element == NULL)
    {
        fprintf(stderr, "Error: Invalid input parameters for stack push\n");
        return ERROR_INVALID_INPUT;
    }

    // Push to front for O(1) operation (head becomes top of stack)
    return singly_list_push_front(stack->list, element);
}

status_t stack_list_pop(stack_list_t *stack, void *output)
{
    // Validate input parameters
    if (stack == NULL)
    {
        fprintf(stderr, "Error: Invalid stack pointer for pop operation\n");
        return ERROR_INVALID_INPUT;
    }

    return singly_list_pop_front(stack->list, output);
}

status_t stack_list_peek(const stack_list_t *stack, void *output)
{
    // Validate input parameters
    if (stack == NULL || output == NULL)
    {
        fprintf(stderr, "Error: Invalid input parameters for stack peek\n");
        return ERROR_INVALID_INPUT;
    }

    return singly_list_get(stack->list, 0, output);
}

void *stack_list_peek_ref(const stack_list_t *stack)
{
    return stack != NULL ? singly_list_front(stack->list) : NULL;
}

size_t stack_list_size(const stack_list_t *stack)
{
    return stack != NULL ? singly_list_size(stack->list) : 0;
}

bool stack_list_empty(const stack_list_t *stack)
{
    return stack == NULL || singly_list_empty(stack->list);
}
