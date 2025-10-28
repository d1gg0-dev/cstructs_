/*
 * @file singly_list.c
 * @brief Singly Linked List Data Structure
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Singly Linked List Module
 * Provides a singly linked list with efficient insertion/deletion at head
 */

#include "../../include/module 3/singly_list.h"
#include <stdio.h>

/* ===== PRIVATE HELPER FUNCTIONS ===== */

/*
 * @brief Creates a new singly linked list node with copied data
 * @param data Pointer to data to copy into the node
 * @param element_size Size of each element in bytes
 * @return Pointer to new node, NULL on memory allocation failure
 *
 * @note Time complexity: O(1) for allocation, plus O(element_size) for data
 * copy
 * @note Performs deep copy of data into newly allocated memory
 * @warning Returns NULL if memory allocation fails for node or data
 */
static singly_node_t *create_node(const void *data, size_t element_size)
{
    singly_node_t *node = (singly_node_t *)mem_alloc(sizeof(singly_node_t));
    if (node == NULL)
        return NULL;

    node->data = mem_alloc(element_size);
    if (node->data == NULL)
    {
        mem_free((void **)&node);
        return NULL;
    }

    mem_copy(node->data, data, element_size);
    node->next = NULL;
    return node;
}

/*
 * @brief Destroys a singly linked list node and its associated data
 * @param node Node to destroy
 *
 * @note Time complexity: O(1)
 * @note Safely frees both the node structure and its data memory
 * @note Handles NULL pointer safely (no operation)
 */
static void destroy_node(singly_node_t *node)
{
    if (node != NULL)
    {
        mem_free(&node->data);
        mem_free((void **)&node);
    }
}

/*
 * @brief Gets node at specified index by traversing from head
 * @param list Target list
 * @param index Index of node to retrieve
 * @return Pointer to node at index, NULL if index out of bounds
 *
 * @note Time complexity: O(n) where n is index (linear search from head)
 * @note Used internally for index-based operations on singly linked list
 * @note No optimization available for singly linked list (must traverse from
 * head)
 */
static singly_node_t *get_node_at(const singly_list_t *list, size_t index)
{
    if (list == NULL || index >= list->size)
    {
        return NULL;
    }

    singly_node_t *current = list->head;
    for (size_t i = 0; i < index && current != NULL; i++)
    {
        current = current->next;
    }
    return current;
}

/* ===== LIST CREATION AND DESTRUCTION ===== */

singly_list_t *singly_list_create(size_t element_size)
{
    if (element_size == 0)
    {
        fprintf(stderr, "Error: Cannot create list with element size 0\n");
        return NULL;
    }

    singly_list_t *list = (singly_list_t *)mem_alloc(sizeof(singly_list_t));
    if (list == NULL)
    {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->element_size = element_size;

    return list;
}

void singly_list_destroy(singly_list_t *list)
{
    if (list == NULL)
        return;

    singly_list_clear(list);
    mem_free((void **)&list);
}

singly_list_t *singly_list_copy(const singly_list_t *src)
{
    if (src == NULL)
        return NULL;

    singly_list_t *dest = singly_list_create(src->element_size);
    if (dest == NULL)
        return NULL;

    // Copy all elements
    singly_node_t *current = src->head;
    while (current != NULL)
    {
        status_t result = singly_list_push_back(dest, current->data);
        if (result != SUCCESS)
        {
            singly_list_destroy(dest);
            return NULL;
        }
        current = current->next;
    }

    return dest;
}

/* ===== BASIC OPERATIONS ===== */

status_t singly_list_push_front(singly_list_t *list, const void *element)
{
    if (list == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    singly_node_t *new_node = create_node(element, list->element_size);
    if (new_node == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    new_node->next = list->head;
    list->head = new_node;

    // If list was empty, update tail
    if (list->tail == NULL)
    {
        list->tail = new_node;
    }

    list->size++;
    return SUCCESS;
}

status_t singly_list_push_back(singly_list_t *list, const void *element)
{
    if (list == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    singly_node_t *new_node = create_node(element, list->element_size);
    if (new_node == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    if (list->tail == NULL)
    {
        // List is empty
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
    return SUCCESS;
}

status_t singly_list_pop_front(singly_list_t *list, void *output)
{
    if (list == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (list->head == NULL)
    {
        return ERROR_EMPTY_CONTAINER;
    }

    singly_node_t *old_head = list->head;

    // Copy data if output is provided
    if (output != NULL)
    {
        mem_copy(output, old_head->data, list->element_size);
    }

    list->head = old_head->next;

    // If list becomes empty, update tail
    if (list->head == NULL)
    {
        list->tail = NULL;
    }

    destroy_node(old_head);
    list->size--;

    return SUCCESS;
}

status_t singly_list_pop_back(singly_list_t *list, void *output)
{
    if (list == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (list->head == NULL)
    {
        return ERROR_EMPTY_CONTAINER;
    }

    // Copy data if output is provided
    if (output != NULL && list->tail != NULL)
    {
        mem_copy(output, list->tail->data, list->element_size);
    }

    if (list->head == list->tail)
    {
        // Only one element
        destroy_node(list->head);
        list->head = NULL;
        list->tail = NULL;
    }
    else
    {
        // Find the node before tail
        singly_node_t *current = list->head;
        while (current->next != list->tail)
        {
            current = current->next;
        }

        destroy_node(list->tail);
        current->next = NULL;
        list->tail = current;
    }

    list->size--;
    return SUCCESS;
}

status_t singly_list_insert(singly_list_t *list, size_t index,
                            const void *element)
{
    if (list == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (index > list->size)
    {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }

    // Optimize for common cases
    if (index == 0)
    {
        return singly_list_push_front(list, element);
    }
    if (index == list->size)
    {
        return singly_list_push_back(list, element);
    }

    // Find the node before insertion point
    singly_node_t *prev = get_node_at(list, index - 1);
    if (prev == NULL)
    {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }

    singly_node_t *new_node = create_node(element, list->element_size);
    if (new_node == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    new_node->next = prev->next;
    prev->next = new_node;
    list->size++;

    return SUCCESS;
}

status_t singly_list_remove(singly_list_t *list, size_t index)
{
    if (list == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (index >= list->size)
    {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }

    // Optimize for common cases
    if (index == 0)
    {
        return singly_list_pop_front(list, NULL);
    }

    // Find the node before the one to remove
    singly_node_t *prev = get_node_at(list, index - 1);
    if (prev == NULL || prev->next == NULL)
    {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }

    singly_node_t *to_remove = prev->next;
    prev->next = to_remove->next;

    // If removing the tail, update tail
    if (to_remove == list->tail)
    {
        list->tail = prev;
    }

    destroy_node(to_remove);
    list->size--;

    return SUCCESS;
}

void singly_list_clear(singly_list_t *list)
{
    if (list == NULL)
        return;

    singly_node_t *current = list->head;
    while (current != NULL)
    {
        singly_node_t *next = current->next;
        destroy_node(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/* ===== ELEMENT ACCESS ===== */

status_t singly_list_get(const singly_list_t *list, size_t index, void *output)
{
    if (list == NULL || output == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    singly_node_t *node = get_node_at(list, index);
    if (node == NULL)
    {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }

    mem_copy(output, node->data, list->element_size);
    return SUCCESS;
}

status_t singly_list_set(singly_list_t *list, size_t index, const void *element)
{
    if (list == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    singly_node_t *node = get_node_at(list, index);
    if (node == NULL)
    {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }

    mem_copy(node->data, element, list->element_size);
    return SUCCESS;
}

void *singly_list_get_ref(const singly_list_t *list, size_t index)
{
    singly_node_t *node = get_node_at(list, index);
    return node != NULL ? node->data : NULL;
}

void *singly_list_front(const singly_list_t *list)
{
    return (list != NULL && list->head != NULL) ? list->head->data : NULL;
}

void *singly_list_back(const singly_list_t *list)
{
    return (list != NULL && list->tail != NULL) ? list->tail->data : NULL;
}

/* ===== LIST PROPERTIES ===== */

size_t singly_list_size(const singly_list_t *list)
{
    return list != NULL ? list->size : 0;
}

bool singly_list_empty(const singly_list_t *list)
{
    return list == NULL || list->size == 0;
}

/* ===== SEARCH OPERATIONS ===== */

int singly_list_find(const singly_list_t *list, const void *element, cmp_fn cmp)
{
    if (list == NULL || element == NULL || cmp == NULL)
    {
        return -1;
    }

    singly_node_t *current = list->head;
    for (size_t i = 0; i < list->size && current != NULL; i++)
    {
        if (cmp(current->data, element) == 0)
        {
            return (int)i;
        }
        current = current->next;
    }

    return -1;
}

bool singly_list_contains(const singly_list_t *list, const void *element,
                          cmp_fn cmp)
{
    return singly_list_find(list, element, cmp) >= 0;
}

/* ===== UTILITY OPERATIONS ===== */

status_t singly_list_reverse(singly_list_t *list)
{
    if (list == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (list->size <= 1)
    {
        return SUCCESS; // Nothing to reverse
    }

    singly_node_t *prev = NULL;
    singly_node_t *current = list->head;
    singly_node_t *next = NULL;

    list->tail = list->head; // Old head becomes new tail

    while (current != NULL)
    {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    list->head = prev; // Last node becomes new head
    return SUCCESS;
}

void singly_list_swap(singly_list_t *a, singly_list_t *b)
{
    if (a == NULL || b == NULL)
        return;

    // Swap all fields
    singly_node_t *temp_head = a->head;
    a->head = b->head;
    b->head = temp_head;

    singly_node_t *temp_tail = a->tail;
    a->tail = b->tail;
    b->tail = temp_tail;

    size_t temp_size = a->size;
    a->size = b->size;
    b->size = temp_size;

    size_t temp_element_size = a->element_size;
    a->element_size = b->element_size;
    b->element_size = temp_element_size;
}

/* ===== ITERATION IMPLEMENTATION ===== */

singly_list_iter_t singly_list_iter_create(singly_list_t *list)
{
    singly_list_iter_t iter;
    iter.list = list;
    iter.current = list != NULL ? list->head : NULL;
    iter.position = 0;
    return iter;
}

bool singly_list_iter_has_next(const singly_list_iter_t *iter)
{
    return iter != NULL && iter->current != NULL;
}

status_t singly_list_iter_next(singly_list_iter_t *iter, void *output)
{
    if (iter == NULL || iter->current == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (output != NULL)
    {
        mem_copy(output, iter->current->data, iter->list->element_size);
    }

    iter->current = iter->current->next;
    iter->position++;
    return SUCCESS;
}

status_t singly_list_iter_remove(singly_list_iter_t *iter)
{
    if (iter == NULL || iter->list == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (iter->position == 0)
    {
        return ERROR_INVALID_INPUT; // Can't remove before first next()
    }

    // This is simplified
    return singly_list_remove(iter->list, iter->position - 1);
}
