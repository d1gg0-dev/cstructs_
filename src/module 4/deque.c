/*
 * @file deque.c
 * @brief Implementation of Double-Ended Queue (Deque) Data Structure
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Deque Module
 * Provides deque implementation using doubly linked list
 */

#include "../../include/module 4/deque.h"
#include <stdio.h>

deque_t *deque_create(size_t element_size)
{
    // Validate input parameters
    if (element_size == 0)
    {
        fprintf(stderr, "Error: Cannot create deque with element size 0\n");
        return NULL;
    }

    // Allocate deque structure
    deque_t *deque = (deque_t *)mem_alloc(sizeof(deque_t));
    if (deque == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate deque structure\n");
        return NULL;
    }

    // Create underlying doubly linked list
    deque->list = doubly_list_create(element_size);
    if (deque->list == NULL)
    {
        fprintf(stderr, "Error: Failed to create underlying list for deque\n");
        mem_free((void **)&deque);
        return NULL;
    }

    return deque;
}

void deque_destroy(deque_t *deque)
{
    if (deque != NULL)
    {
        doubly_list_destroy(deque->list);
        mem_free((void **)&deque);
    }
}

status_t deque_push_front(deque_t *deque, const void *element)
{
    // Validate input parameters
    if (deque == NULL || element == NULL)
    {
        fprintf(stderr,
                "Error: Invalid input parameters for deque push_front\n");
        return ERROR_INVALID_INPUT;
    }

    return doubly_list_push_front(deque->list, element);
}

status_t deque_push_back(deque_t *deque, const void *element)
{
    // Validate input parameters
    if (deque == NULL || element == NULL)
    {
        fprintf(stderr,
                "Error: Invalid input parameters for deque push_back\n");
        return ERROR_INVALID_INPUT;
    }

    return doubly_list_push_back(deque->list, element);
}

status_t deque_pop_front(deque_t *deque, void *output)
{
    // Validate input parameters
    if (deque == NULL)
    {
        fprintf(stderr,
                "Error: Invalid deque pointer for pop_front operation\n");
        return ERROR_INVALID_INPUT;
    }

    return doubly_list_pop_front(deque->list, output);
}

status_t deque_pop_back(deque_t *deque, void *output)
{
    // Validate input parameters
    if (deque == NULL)
    {
        fprintf(stderr,
                "Error: Invalid deque pointer for pop_back operation\n");
        return ERROR_INVALID_INPUT;
    }

    return doubly_list_pop_back(deque->list, output);
}

status_t deque_peek_front(const deque_t *deque, void *output)
{
    // Validate input parameters
    if (deque == NULL || output == NULL)
    {
        fprintf(stderr,
                "Error: Invalid input parameters for deque peek_front\n");
        return ERROR_INVALID_INPUT;
    }

    return doubly_list_get(deque->list, 0, output);
}

status_t deque_peek_back(const deque_t *deque, void *output)
{
    // Validate input parameters
    if (deque == NULL || output == NULL)
    {
        fprintf(stderr,
                "Error: Invalid input parameters for deque peek_back\n");
        return ERROR_INVALID_INPUT;
    }

    // Check if deque is empty
    if (doubly_list_empty(deque->list))
    {
        fprintf(stderr, "Error: Cannot peek from empty deque\n");
        return ERROR_EMPTY_CONTAINER;
    }

    // Get the last element (back of deque)
    return doubly_list_get(deque->list, doubly_list_size(deque->list) - 1,
                           output);
}

void *deque_peek_front_ref(const deque_t *deque)
{
    return deque != NULL ? doubly_list_front(deque->list) : NULL;
}

void *deque_peek_back_ref(const deque_t *deque)
{
    return deque != NULL ? doubly_list_back(deque->list) : NULL;
}

size_t deque_size(const deque_t *deque)
{
    return deque != NULL ? doubly_list_size(deque->list) : 0;
}

bool deque_empty(const deque_t *deque)
{
    return deque == NULL || doubly_list_empty(deque->list);
}

status_t deque_insert(deque_t *deque, size_t index, const void *element)
{
    // Validate input parameters
    if (deque == NULL || element == NULL)
    {
        fprintf(stderr, "Error: Invalid input parameters for deque insert\n");
        return ERROR_INVALID_INPUT;
    }

    return doubly_list_insert(deque->list, index, element);
}

status_t deque_remove(deque_t *deque, size_t index)
{
    // Validate input parameters
    if (deque == NULL)
    {
        fprintf(stderr, "Error: Invalid deque pointer for remove operation\n");
        return ERROR_INVALID_INPUT;
    }

    return doubly_list_remove(deque->list, index);
}

status_t deque_get(deque_t *deque, size_t index, void *output)
{
    // Validate input parameters
    if (deque == NULL || output == NULL)
    {
        fprintf(stderr, "Error: Invalid input parameters for deque get\n");
        return ERROR_INVALID_INPUT;
    }

    return doubly_list_get(deque->list, index, output);
}

status_t deque_set(deque_t *deque, size_t index, const void *element)
{
    // Validate input parameters
    if (deque == NULL || element == NULL)
    {
        fprintf(stderr, "Error: Invalid input parameters for deque set\n");
        return ERROR_INVALID_INPUT;
    }

    return doubly_list_set(deque->list, index, element);
}
