/*
 * @file doubly_list.c
 * @brief Doubly Linked List Data Structure
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Doubly Linked List Module
 * Provides a doubly linked list with efficient bidirectional traversal
 */

#include "../../include/module 3/doubly_list.h"
#include <stdio.h>

/* ===== PRIVATE HELPER FUNCTIONS ===== */

/*
 * @brief Creates a new doubly linked list node with copied data
 * @param data Pointer to data to copy into the node
 * @param element_size Size of each element in bytes
 * @return Pointer to new node, NULL on memory allocation failure
 *
 * @note Time complexity: O(1) for allocation, plus O(element_size) for data
 * copy
 * @note Performs deep copy of data into newly allocated memory
 * @warning Returns NULL if memory allocation fails for node or data
 */
static doubly_node_t *create_doubly_node(const void *data, size_t element_size)
{
    doubly_node_t *node = (doubly_node_t *)mem_alloc(sizeof(doubly_node_t));
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
    node->prev = NULL;
    return node;
}

/*
 * @brief Destroys a doubly linked list node and its associated data
 * @param node Node to destroy
 *
 * @note Time complexity: O(1)
 * @note Safely frees both the node structure and its data memory
 * @note Handles NULL pointer safely (no operation)
 */
static void destroy_doubly_node(doubly_node_t *node)
{
    if (node != NULL)
    {
        mem_free(&node->data);
        mem_free((void **)&node);
    }
}

/*
 * @brief Gets node at specified index with bidirectional optimization
 * @param list Target list
 * @param index Index of node to retrieve
 * @return Pointer to node at index, NULL if index out of bounds
 *
 * @note Time complexity: O(n) in worst case, but optimized to O(n/2) on average
 * @note Uses bidirectional search: starts from head for first half, tail for
 * second half
 * @note Improves performance for accessing elements near ends of large lists
 */
static doubly_node_t *get_doubly_node_at(const doubly_list_t *list,
                                         size_t index)
{
    if (list == NULL || index >= list->size)
    {
        return NULL;
    }

    // Optimize: start from head or tail depending on index position
    if (index < list->size / 2)
    {
        // Start from head
        doubly_node_t *current = list->head;
        for (size_t i = 0; i < index && current != NULL; i++)
        {
            current = current->next;
        }
        return current;
    }
    else
    {
        // Start from tail
        doubly_node_t *current = list->tail;
        for (size_t i = list->size - 1; i > index && current != NULL; i--)
        {
            current = current->prev;
        }
        return current;
    }
}

/* ===== LIST CREATION AND DESTRUCTION ===== */

doubly_list_t *doubly_list_create(size_t element_size)
{
    if (element_size == 0)
    {
        fprintf(stderr, "Error: Cannot create list with element size 0\n");
        return NULL;
    }

    doubly_list_t *list = (doubly_list_t *)mem_alloc(sizeof(doubly_list_t));
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

void doubly_list_destroy(doubly_list_t *list)
{
    if (list == NULL)
        return;

    doubly_list_clear(list);
    mem_free((void **)&list);
}

doubly_list_t *doubly_list_copy(const doubly_list_t *src)
{
    if (src == NULL)
        return NULL;

    doubly_list_t *dest = doubly_list_create(src->element_size);
    if (dest == NULL)
        return NULL;

    // Copy all elements
    doubly_node_t *current = src->head;
    while (current != NULL)
    {
        status_t result = doubly_list_push_back(dest, current->data);
        if (result != SUCCESS)
        {
            doubly_list_destroy(dest);
            return NULL;
        }
        current = current->next;
    }

    return dest;
}

/* ===== BASIC OPERATIONS ===== */

status_t doubly_list_push_front(doubly_list_t *list, const void *element)
{
    if (list == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    doubly_node_t *new_node = create_doubly_node(element, list->element_size);
    if (new_node == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    new_node->next = list->head;

    if (list->head != NULL)
    {
        list->head->prev = new_node;
    }

    list->head = new_node;

    // If list was empty, update tail
    if (list->tail == NULL)
    {
        list->tail = new_node;
    }

    list->size++;
    return SUCCESS;
}

status_t doubly_list_push_back(doubly_list_t *list, const void *element)
{
    if (list == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    doubly_node_t *new_node = create_doubly_node(element, list->element_size);
    if (new_node == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    new_node->prev = list->tail;

    if (list->tail != NULL)
    {
        list->tail->next = new_node;
    }

    list->tail = new_node;

    // If list was empty, update head
    if (list->head == NULL)
    {
        list->head = new_node;
    }

    list->size++;
    return SUCCESS;
}

status_t doubly_list_pop_front(doubly_list_t *list, void *output)
{
    if (list == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (list->head == NULL)
    {
        return ERROR_EMPTY_CONTAINER;
    }

    doubly_node_t *old_head = list->head;

    // Copy data if output is provided
    if (output != NULL)
    {
        mem_copy(output, old_head->data, list->element_size);
    }

    list->head = old_head->next;

    if (list->head != NULL)
    {
        list->head->prev = NULL;
    }
    else
    {
        // List became empty
        list->tail = NULL;
    }

    destroy_doubly_node(old_head);
    list->size--;

    return SUCCESS;
}

status_t doubly_list_pop_back(doubly_list_t *list, void *output)
{
    if (list == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (list->tail == NULL)
    {
        return ERROR_EMPTY_CONTAINER;
    }

    doubly_node_t *old_tail = list->tail;

    // Copy data if output is provided
    if (output != NULL)
    {
        mem_copy(output, old_tail->data, list->element_size);
    }

    list->tail = old_tail->prev;

    if (list->tail != NULL)
    {
        list->tail->next = NULL;
    }
    else
    {
        // List became empty
        list->head = NULL;
    }

    destroy_doubly_node(old_tail);
    list->size--;

    return SUCCESS;
}

status_t doubly_list_insert(doubly_list_t *list, size_t index,
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
        return doubly_list_push_front(list, element);
    }
    if (index == list->size)
    {
        return doubly_list_push_back(list, element);
    }

    // Find the node at insertion point
    doubly_node_t *current = get_doubly_node_at(list, index);
    if (current == NULL)
    {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }

    // Insert before current node
    doubly_node_t *new_node = create_doubly_node(element, list->element_size);
    if (new_node == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    new_node->prev = current->prev;
    new_node->next = current;

    if (current->prev != NULL)
    {
        current->prev->next = new_node;
    }

    current->prev = new_node;
    list->size++;

    return SUCCESS;
}

status_t doubly_list_remove(doubly_list_t *list, size_t index)
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
        return doubly_list_pop_front(list, NULL);
    }
    if (index == list->size - 1)
    {
        return doubly_list_pop_back(list, NULL);
    }

    // Find the node to remove
    doubly_node_t *to_remove = get_doubly_node_at(list, index);
    if (to_remove == NULL)
    {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }

    // Update neighbors
    if (to_remove->prev != NULL)
    {
        to_remove->prev->next = to_remove->next;
    }
    if (to_remove->next != NULL)
    {
        to_remove->next->prev = to_remove->prev;
    }

    destroy_doubly_node(to_remove);
    list->size--;

    return SUCCESS;
}

void doubly_list_clear(doubly_list_t *list)
{
    if (list == NULL)
        return;

    doubly_node_t *current = list->head;
    while (current != NULL)
    {
        doubly_node_t *next = current->next;
        destroy_doubly_node(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/* ===== ELEMENT ACCESS ===== */

status_t doubly_list_get(const doubly_list_t *list, size_t index, void *output)
{
    if (list == NULL || output == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    doubly_node_t *node = get_doubly_node_at(list, index);
    if (node == NULL)
    {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }

    mem_copy(output, node->data, list->element_size);
    return SUCCESS;
}

status_t doubly_list_set(doubly_list_t *list, size_t index, const void *element)
{
    if (list == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    doubly_node_t *node = get_doubly_node_at(list, index);
    if (node == NULL)
    {
        return ERROR_INDEX_OUT_OF_BOUNDS;
    }

    mem_copy(node->data, element, list->element_size);
    return SUCCESS;
}

void *doubly_list_get_ref(const doubly_list_t *list, size_t index)
{
    doubly_node_t *node = get_doubly_node_at(list, index);
    return node != NULL ? node->data : NULL;
}

void *doubly_list_front(const doubly_list_t *list)
{
    return (list != NULL && list->head != NULL) ? list->head->data : NULL;
}

void *doubly_list_back(const doubly_list_t *list)
{
    return (list != NULL && list->tail != NULL) ? list->tail->data : NULL;
}

/* ===== LIST PROPERTIES ===== */

size_t doubly_list_size(const doubly_list_t *list)
{
    return list != NULL ? list->size : 0;
}

bool doubly_list_empty(const doubly_list_t *list)
{
    return list == NULL || list->size == 0;
}

/* ===== SEARCH OPERATIONS ===== */

int doubly_list_find(const doubly_list_t *list, const void *element, cmp_fn cmp)
{
    if (list == NULL || element == NULL || cmp == NULL)
    {
        return -1;
    }

    doubly_node_t *current = list->head;
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

bool doubly_list_contains(const doubly_list_t *list, const void *element,
                          cmp_fn cmp)
{
    return doubly_list_find(list, element, cmp) >= 0;
}

/* ===== BIDIRECTIONAL OPERATIONS ===== */

status_t doubly_list_insert_before(doubly_list_t *list, doubly_node_t *node,
                                   const void *element)
{
    if (list == NULL || node == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    doubly_node_t *new_node = create_doubly_node(element, list->element_size);
    if (new_node == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    new_node->prev = node->prev;
    new_node->next = node;

    if (node->prev != NULL)
    {
        node->prev->next = new_node;
    }
    else
    {
        // Inserting before head
        list->head = new_node;
    }

    node->prev = new_node;
    list->size++;

    return SUCCESS;
}

status_t doubly_list_insert_after(doubly_list_t *list, doubly_node_t *node,
                                  const void *element)
{
    if (list == NULL || node == NULL || element == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    doubly_node_t *new_node = create_doubly_node(element, list->element_size);
    if (new_node == NULL)
    {
        return ERROR_MEMORY_ALLOCATION;
    }

    new_node->prev = node;
    new_node->next = node->next;

    if (node->next != NULL)
    {
        node->next->prev = new_node;
    }
    else
    {
        // Inserting after tail
        list->tail = new_node;
    }

    node->next = new_node;
    list->size++;

    return SUCCESS;
}

/* ===== UTILITY OPERATIONS ===== */

status_t doubly_list_reverse(doubly_list_t *list)
{
    if (list == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (list->size <= 1)
    {
        return SUCCESS; // Nothing to reverse
    }

    doubly_node_t *current = list->head;
    doubly_node_t *temp = NULL;

    // Swap head and tail
    list->head = list->tail;
    list->tail = current;

    // Reverse all links
    while (current != NULL)
    {
        temp = current->prev;
        current->prev = current->next;
        current->next = temp;
        current = current->prev; // Move to next (which was previous)
    }

    return SUCCESS;
}

void doubly_list_swap(doubly_list_t *a, doubly_list_t *b)
{
    if (a == NULL || b == NULL)
        return;

    // Swap all fields
    doubly_node_t *temp_head = a->head;
    a->head = b->head;
    b->head = temp_head;

    doubly_node_t *temp_tail = a->tail;
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

doubly_list_iter_t doubly_list_iter_create(doubly_list_t *list)
{
    doubly_list_iter_t iter;
    iter.list = list;
    iter.current = list != NULL ? list->head : NULL;
    iter.position = 0;
    return iter;
}

doubly_list_iter_t doubly_list_iter_create_reverse(doubly_list_t *list)
{
    doubly_list_iter_t iter;
    iter.list = list;
    iter.current = list != NULL ? list->tail : NULL;
    iter.position = list != NULL ? list->size - 1 : 0;
    return iter;
}

bool doubly_list_iter_has_next(const doubly_list_iter_t *iter)
{
    return iter != NULL && iter->current != NULL;
}

bool doubly_list_iter_has_prev(const doubly_list_iter_t *iter)
{
    return iter != NULL && iter->current != NULL;
}

status_t doubly_list_iter_next(doubly_list_iter_t *iter, void *output)
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

status_t doubly_list_iter_prev(doubly_list_iter_t *iter, void *output)
{
    if (iter == NULL || iter->current == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    if (output != NULL)
    {
        mem_copy(output, iter->current->data, iter->list->element_size);
    }

    iter->current = iter->current->prev;
    iter->position--;
    return SUCCESS;
}

status_t doubly_list_iter_remove(doubly_list_iter_t *iter)
{
    if (iter == NULL || iter->list == NULL || iter->current == NULL)
    {
        return ERROR_INVALID_INPUT;
    }

    doubly_node_t *to_remove = iter->current;
    doubly_node_t *next = to_remove->next;

    // Update neighbors
    if (to_remove->prev != NULL)
    {
        to_remove->prev->next = to_remove->next;
    }
    else
    {
        iter->list->head = to_remove->next;
    }

    if (to_remove->next != NULL)
    {
        to_remove->next->prev = to_remove->prev;
    }
    else
    {
        iter->list->tail = to_remove->prev;
    }

    // Update iterator
    iter->current = next;
    iter->list->size--;

    destroy_doubly_node(to_remove);
    return SUCCESS;
}
