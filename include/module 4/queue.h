/*
 * @file queue.h
 * @brief Queue Data Structure (FIFO)
 * @author Rodrigo Martins
 * @version 0.0
 * @date 2024
 *
 * CStructs+ Library - Queue Module
 * Provides queue implementation using both arrays and linked lists
 */

#ifndef CSTRUCTS_QUEUE_H
#define CSTRUCTS_QUEUE_H

#include "../module 1/core.h"
#include "../module 2/vector.h"
#include "../module 3/singly_list.h"
#include <stdbool.h>

/* ===== QUEUE USING CIRCULAR ARRAY ===== */

/*
 * @brief Queue implemented using circular array
 */
typedef struct
{
        void *data;          // Pointer to data array
        size_t front;        // Front index
        size_t rear;         // Rear index
        size_t size;         // Current number of elements
        size_t capacity;     // Total capacity
        size_t element_size; // Size of each element
} queue_array_t;

/* ===== QUEUE USING LINKED LIST ===== */

/*
 * @brief Queue implemented using singly linked list
 */
typedef struct
{
        singly_list_t *list; // Using singly linked list as underlying storage
} queue_list_t;

/* ===== ARRAY QUEUE OPERATIONS ===== */

/*
 * @brief Creates a new array-based queue with default capacity
 * @param element_size Size of each element in bytes
 * @return Pointer to new queue, NULL on failure
 *
 * @note Uses circular array for efficient FIFO operations
 * @note Default capacity provides good balance for most use cases
 */
queue_array_t *queue_array_create(size_t element_size);

/*
 * @brief Creates a new array-based queue with specified capacity
 * @param element_size Size of each element in bytes
 * @param capacity Initial capacity of the queue
 * @return Pointer to new queue, NULL on failure
 *
 * @note Pre-allocating capacity can improve performance for known queue sizes
 * @note Uses circular buffer pattern for O(1) enqueue/dequeue
 */
queue_array_t *queue_array_create_with_capacity(size_t element_size,
                                                size_t capacity);
/*
 * @brief Destroys an array-based queue and frees all memory
 * @param queue Pointer to queue to destroy
 *
 * @note Safely handles NULL pointers
 * @note Frees both the queue structure and data array
 */
void queue_array_destroy(queue_array_t *queue);

/*
 * @brief Adds an element to the end of the queue
 * @param queue Target queue
 * @param element Element to enqueue
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1) amortized
 * @note Uses circular array with automatic resizing
 */
status_t queue_array_enqueue(queue_array_t *queue, const void *element);

/*
 * @brief Removes and returns the front element from the queue
 * @param queue Target queue
 * @param output Where to store the dequeued element (can be NULL if not needed)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note FIFO behavior - removes oldest element
 */
status_t queue_array_dequeue(queue_array_t *queue, void *output);

/*
 * @brief Peeks at the front element without removing it
 * @param queue Target queue
 * @param output Where to store the front element
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Does not modify the queue
 */
status_t queue_array_peek(const queue_array_t *queue, void *output);

/*
 * @brief Gets direct pointer to front element without copying (unsafe but fast)
 * @param queue Target queue
 * @return Pointer to front element, NULL if empty or error
 *
 * @note Time complexity: O(1)
 * @warning Unsafe: returned pointer becomes invalid if queue is modified
 */
void *queue_array_peek_ref(const queue_array_t *queue);

/*
 * @brief Gets current number of elements in queue
 * @param queue Target queue
 * @return Number of elements, 0 if queue is NULL
 *
 * @note Time complexity: O(1)
 */
size_t queue_array_size(const queue_array_t *queue);

/*
 * @brief Checks if queue is empty
 * @param queue Target queue
 * @return true if empty, false otherwise
 *
 * @note Time complexity: O(1)
 */
bool queue_array_empty(const queue_array_t *queue);

/*
 * @brief Gets current capacity of queue
 * @param queue Target queue
 * @return Current capacity, 0 if queue is NULL
 *
 * @note Time complexity: O(1)
 */
size_t queue_array_capacity(const queue_array_t *queue);

/*
 * @brief Reserves capacity for at least specified number of elements
 * @param queue Target queue
 * @param capacity New capacity
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(n) where n is current size
 * @note Can prevent reallocations during enqueue operations
 */
status_t queue_array_reserve(queue_array_t *queue, size_t capacity);

/* ===== LINKED LIST QUEUE OPERATIONS ===== */

/*
 * @brief Creates a new linked list-based queue
 * @param element_size Size of each element in bytes
 * @return Pointer to new queue, NULL on failure
 *
 * @note Uses singly linked list with tail pointer for O(1) enqueue/dequeue
 * @note No capacity limits, but more memory overhead per element
 */
queue_list_t *queue_list_create(size_t element_size);

/*
 * @brief Destroys a linked list-based queue and frees all memory
 * @param queue Pointer to queue to destroy
 *
 * @note Safely handles NULL pointers
 * @note Frees all nodes and the queue structure
 */
void queue_list_destroy(queue_list_t *queue);

/*
 * @brief Adds an element to the end of the queue
 * @param queue Target queue
 * @param element Element to enqueue
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Uses tail pointer for constant time append
 */
status_t queue_list_enqueue(queue_list_t *queue, const void *element);

/*
 * @brief Removes and returns the front element from the queue
 * @param queue Target queue
 * @param output Where to store the dequeued element (can be NULL if not needed)
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Removes from head (front of queue)
 */
status_t queue_list_dequeue(queue_list_t *queue, void *output);

/*
 * @brief Peeks at the front element without removing it
 * @param queue Target queue
 * @param output Where to store the front element
 * @return SUCCESS on success, error code on failure
 *
 * @note Time complexity: O(1)
 * @note Accesses head element without modification
 */
status_t queue_list_peek(const queue_list_t *queue, void *output);

/*
 * @brief Gets direct pointer to front element without copying (unsafe but fast)
 * @param queue Target queue
 * @return Pointer to front element, NULL if empty or error
 *
 * @note Time complexity: O(1)
 * @warning Unsafe: returned pointer becomes invalid if queue is modified
 */
void *queue_list_peek_ref(const queue_list_t *queue);

/*
 * @brief Gets current number of elements in queue
 * @param queue Target queue
 * @return Number of elements, 0 if queue is NULL
 *
 * @note Time complexity: O(1)
 */
size_t queue_list_size(const queue_list_t *queue);

/*
 * @brief Checks if queue is empty
 * @param queue Target queue
 * @return true if empty, false otherwise
 *
 * @note Time complexity: O(1)
 */
bool queue_list_empty(const queue_list_t *queue);

/* ===== COMMON QUEUE OPERATIONS (MACROS FOR CONVENIENCE) ===== */

/*
 * @brief Generic enqueue operation (works with both array and list queues)
 */
#define QUEUE_ENQUEUE(queue, element)                                          \
    _Generic((queue),                                                          \
        queue_array_t *: queue_array_enqueue,                                  \
        queue_list_t *: queue_list_enqueue)(queue, element)

/*
 * @brief Generic dequeue operation (works with both array and list queues)
 */
#define QUEUE_DEQUEUE(queue, output)                                           \
    _Generic((queue),                                                          \
        queue_array_t *: queue_array_dequeue,                                  \
        queue_list_t *: queue_list_dequeue)(queue, output)

/*
 * @brief Generic peek operation (works with both array and list queues)
 */
#define QUEUE_PEEK(queue, output)                                              \
    _Generic((queue),                                                          \
        queue_array_t *: queue_array_peek,                                     \
        queue_list_t *: queue_list_peek)(queue, output)

/*
 * @brief Generic size operation (works with both array and list queues)
 */
#define QUEUE_SIZE(queue)                                                      \
    _Generic((queue),                                                          \
        queue_array_t *: queue_array_size,                                     \
        queue_list_t *: queue_list_size)(queue)

/*
 * @brief Generic empty check (works with both array and list queues)
 */
#define QUEUE_EMPTY(queue)                                                     \
    _Generic((queue),                                                          \
        queue_array_t *: queue_array_empty,                                    \
        queue_list_t *: queue_list_empty)(queue)

#endif /* CSTRUCTS_QUEUE_H */
