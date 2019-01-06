#pragma once

/** @defgroup queue queue
 * @{
 *
 * Functions used to create and manipulate simple array-based queues of 8 bit unsigned integers.
 */

/**
 * @brief Represents a queue.
 * 
 */
typedef struct { 
    int front;      ///< Front of the queue
    int rear;       ///< Rear of the queue
    int capacity;   ///< Queue capacity
    int size;       ///< Queue current size
    uint8_t* array; ///< Queue's elements
} Queue;

/**
 * @brief Creates a queue
 * @param capacity Queue initial capacity
 * @return Queue* Returns the created queue
 */
Queue* queue_create(unsigned capacity);

/**
 * @brief Destroys a queue, freeing all used memory.
 * 
 * @param q Queue to be destroyed
 */
void queue_destroy(Queue* q);

/**
 * @brief Gets the front of the queue
 * 
 * @param q Queue to read
 * @return uint8_t Returns the front of the queue
 */
uint8_t queue_front(Queue *q);

/**
 * @brief Removes the front of the queue
 * 
 * @param q Queue to pop
 * @return uint8_t Returns the removed item
 */
uint8_t queue_pop(Queue* q);

/**
 * @brief Pushes an item to the end of the queue
 * 
 * @param q Queue to add an element to
 * @param data Byte to add to the queue
 */
void queue_push(Queue *q, uint8_t data);

/**
 * @brief Checks if a queue is full
 * 
 * @param queue Queue to inspect
 * @return true The queue is full
 * @return false The queue is not full
 */
bool queue_full(Queue* queue);

/**
 * @brief Checks if a queue is empty
 * 
 * @param queue Queue to inspect
 * @return true The queue is empty
 * @return false The queue is not empty
 */
bool queue_empty(Queue* queue);

/**
 * @brief Doubles a queue's capacity. Called by queue_push when needed.
 * 
 * @param queue Queue to increase capacity.
 */
void queue_resize(Queue* queue);

  

