/*
 * @file queue.c
 * @brief Implementation of Queue Data Structure
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Queue Module
 * Provides queue implementation using both arrays and linked lists
 */

#include "../../include/module 4/queue.h"
#include <stdio.h>

/* ===== CONSTANTS ===== */

#define QUEUE_INITIAL_CAPACITY 16
#define QUEUE_GROWTH_FACTOR 2

/* ===== ARRAY QUEUE IMPLEMENTATION ===== */

/*
 * @brief Resizes the circular array queue to new capacity
 * @param queue Target queue
 * @param new_capacity New capacity
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) where n is current size
 * @note Maintains element order during resize
 * @note Used internally for growing the queue
 */
static status_t queue_array_resize(queue_array_t *queue, size_t new_capacity)
{
    if (queue == NULL || new_capacity < queue->size)
    {
        fprintf(stderr, "Error: Invalid parameters for queue resize\n");
        return ERROR_INVALID_INPUT;
    }

    // Allocate new data array
    void *new_data = mem_alloc(new_capacity * queue->element_size);
    if (new_data == NULL)
    {
        fprintf(stderr,
                "Error: Failed to allocate new queue data of size %zu\n",
                new_capacity * queue->element_size);
        return ERROR_MEMORY_ALLOCATION;
    }

    // Copy elements to new array maintaining FIFO order
    if (queue->size > 0)
    {
        if (queue->front < queue->rear)
        {
            // Case 1: Contiguous block of elements
            size_t bytes_to_copy = queue->size * queue->element_size;
            void *src =
                (char *)queue->data + (queue->front * queue->element_size);
            mem_copy(new_data, src, bytes_to_copy);
        }
        else
        {
            // Case 2: Wrapped around (elements at end and beginning of array)
            size_t first_part = queue->capacity - queue->front;
            size_t first_bytes = first_part * queue->element_size;
            size_t second_bytes = queue->rear * queue->element_size;

            // Copy from front to end of array
            void *src1 =
                (char *)queue->data + (queue->front * queue->element_size);
            mem_copy(new_data, src1, first_bytes);

            // Copy from beginning to rear
            void *src2 = queue->data;
            void *dest2 = (char *)new_data + first_bytes;
            mem_copy(dest2, src2, second_bytes);
        }
    }

    // Update queue state with new array
    mem_free(&queue->data);
    queue->data = new_data;
    queue->capacity = new_capacity;
    queue->front = 0;
    queue->rear = queue->size; // All elements are now at start of array

    return SUCCESS;
}

/*
 * @brief Checks if queue needs to grow and resizes if necessary
 * @param queue Target queue
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) amortized
 * @note Used internally before enqueue operations
 */
static status_t queue_array_check_grow(queue_array_t *queue)
{
    if (queue->size >= queue->capacity)
    {
        size_t new_capacity = queue->capacity * QUEUE_GROWTH_FACTOR;
        if (new_capacity < QUEUE_INITIAL_CAPACITY)
        {
            new_capacity = QUEUE_INITIAL_CAPACITY;
        }
        return queue_array_resize(queue, new_capacity);
    }
    return SUCCESS;
}

queue_array_t *queue_array_create(size_t element_size)
{
    return queue_array_create_with_capacity(element_size,
                                            QUEUE_INITIAL_CAPACITY);
}

queue_array_t *queue_array_create_with_capacity(size_t element_size,
                                                size_t capacity)
{
    // Validate input parameters
    if (element_size == 0)
    {
        fprintf(stderr, "Error: Cannot create queue with element size 0\n");
        return NULL;
    }

    if (capacity == 0)
    {
        capacity = QUEUE_INITIAL_CAPACITY;
    }

    // Allocate queue structure
    queue_array_t *queue = (queue_array_t *)mem_alloc(sizeof(queue_array_t));
    if (queue == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate queue structure\n");
        return NULL;
    }

    // Allocate data array
    queue->data = mem_alloc(capacity * element_size);
    if (queue->data == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate queue data of size %zu\n",
                capacity * element_size);
        mem_free((void **)&queue);
        return NULL;
    }

    // Initialize queue state
    queue->front = 0;
    queue->rear = 0;
    queue->size = 0;
    queue->capacity = capacity;
    queue->element_size = element_size;

    return queue;
}

void queue_array_destroy(queue_array_t *queue)
{
    if (queue != NULL)
    {
        mem_free(&queue->data);
        mem_free((void **)&queue);
    }
}

status_t queue_array_enqueue(queue_array_t *queue, const void *element)
{
    // Validate input parameters
    if (queue == NULL || element == NULL)
    {
        fprintf(stderr, "Error: Invalid input parameters for queue enqueue\n");
        return ERROR_INVALID_INPUT;
    }

    // Check if we need to grow the queue
    status_t result = queue_array_check_grow(queue);
    if (result != SUCCESS)
    {
        return result;
    }

    // Copy element to rear position
    void *dest = (char *)queue->data + (queue->rear * queue->element_size);
    mem_copy(dest, element, queue->element_size);

    // Update rear position with wrap-around
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->size++;

    return SUCCESS;
}

status_t queue_array_dequeue(queue_array_t *queue, void *output)
{
    // Validate input parameters
    if (queue == NULL)
    {
        fprintf(stderr, "Error: Invalid queue pointer for dequeue operation\n");
        return ERROR_INVALID_INPUT;
    }

    // Check if queue is empty
    if (queue->size == 0)
    {
        fprintf(stderr, "Error: Cannot dequeue from empty queue\n");
        return ERROR_EMPTY_CONTAINER;
    }

    // Copy element if output is provided
    if (output != NULL)
    {
        void *src = (char *)queue->data + (queue->front * queue->element_size);
        mem_copy(output, src, queue->element_size);
    }

    // Update front position with wrap-around
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;

    return SUCCESS;
}

status_t queue_array_peek(const queue_array_t *queue, void *output)
{
    // Validate input parameters
    if (queue == NULL || output == NULL)
    {
        fprintf(stderr, "Error: Invalid input parameters for queue peek\n");
        return ERROR_INVALID_INPUT;
    }

    // Check if queue is empty
    if (queue->size == 0)
    {
        fprintf(stderr, "Error: Cannot peek from empty queue\n");
        return ERROR_EMPTY_CONTAINER;
    }

    // Copy front element to output
    void *src = (char *)queue->data + (queue->front * queue->element_size);
    mem_copy(output, src, queue->element_size);

    return SUCCESS;
}

void *queue_array_peek_ref(const queue_array_t *queue)
{
    if (queue == NULL || queue->size == 0)
    {
        return NULL;
    }

    return (char *)queue->data + (queue->front * queue->element_size);
}

size_t queue_array_size(const queue_array_t *queue)
{
    return queue != NULL ? queue->size : 0;
}

bool queue_array_empty(const queue_array_t *queue)
{
    return queue == NULL || queue->size == 0;
}

size_t queue_array_capacity(const queue_array_t *queue)
{
    return queue != NULL ? queue->capacity : 0;
}

status_t queue_array_reserve(queue_array_t *queue, size_t capacity)
{
    if (queue == NULL)
    {
        fprintf(stderr, "Error: Invalid queue pointer for reserve operation\n");
        return ERROR_INVALID_INPUT;
    }

    if (capacity <= queue->capacity)
    {
        return SUCCESS; // No need to reserve less capacity
    }

    return queue_array_resize(queue, capacity);
}

/* ===== LINKED LIST QUEUE IMPLEMENTATION ===== */

queue_list_t *queue_list_create(size_t element_size)
{
    // Validate input parameters
    if (element_size == 0)
    {
        fprintf(stderr, "Error: Cannot create queue with element size 0\n");
        return NULL;
    }

    // Allocate queue structure
    queue_list_t *queue = (queue_list_t *)mem_alloc(sizeof(queue_list_t));
    if (queue == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate queue structure\n");
        return NULL;
    }

    // Create underlying linked list
    queue->list = singly_list_create(element_size);
    if (queue->list == NULL)
    {
        fprintf(stderr, "Error: Failed to create underlying list for queue\n");
        mem_free((void **)&queue);
        return NULL;
    }

    return queue;
}

void queue_list_destroy(queue_list_t *queue)
{
    if (queue != NULL)
    {
        singly_list_destroy(queue->list);
        mem_free((void **)&queue);
    }
}

status_t queue_list_enqueue(queue_list_t *queue, const void *element)
{
    // Validate input parameters
    if (queue == NULL || element == NULL)
    {
        fprintf(stderr, "Error: Invalid input parameters for queue enqueue\n");
        return ERROR_INVALID_INPUT;
    }

    // Enqueue at back for O(1) with tail pointer
    return singly_list_push_back(queue->list, element);
}

status_t queue_list_dequeue(queue_list_t *queue, void *output)
{
    // Validate input parameters
    if (queue == NULL)
    {
        fprintf(stderr, "Error: Invalid queue pointer for dequeue operation\n");
        return ERROR_INVALID_INPUT;
    }

    return singly_list_pop_front(queue->list, output);
}

status_t queue_list_peek(const queue_list_t *queue, void *output)
{
    // Validate input parameters
    if (queue == NULL || output == NULL)
    {
        fprintf(stderr, "Error: Invalid input parameters for queue peek\n");
        return ERROR_INVALID_INPUT;
    }

    return singly_list_get(queue->list, 0, output);
}

void *queue_list_peek_ref(const queue_list_t *queue)
{
    return queue != NULL ? singly_list_front(queue->list) : NULL;
}

size_t queue_list_size(const queue_list_t *queue)
{
    return queue != NULL ? singly_list_size(queue->list) : 0;
}

bool queue_list_empty(const queue_list_t *queue)
{
    return queue == NULL || singly_list_empty(queue->list);
}
