#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "queue.h"

Queue* queue_create(unsigned capacity) { 
    Queue* queue = malloc(sizeof(Queue)); 
    queue->capacity = capacity; 
    queue->front = queue->size = 0;  
    queue->rear = capacity - 1;
    queue->array = malloc(queue->capacity * sizeof(uint8_t)); 
    return queue; 
} 

void queue_destroy(Queue* q) {
	free(q->array);
	free(q);
}


bool queue_full(Queue* queue) {  
	return (queue->size == queue->capacity);  
} 
  
bool queue_empty(Queue* queue) {  
	return (queue->size == 0);
} 

void queue_push(Queue* queue, uint8_t item) { 
    if (queue_full(queue)) {
		printf("JEEZ QUEUE IS SO FULL! size = %d \n", queue->size);
		queue_resize(queue);
	}
    queue->size++;
    queue->rear++;
	if (queue->rear == queue->capacity) {
		queue->rear = 0; 
	}
    queue->array[queue->rear] = item;
} 

void queue_resize(Queue* queue) {
	Queue* new_queue = queue_create(queue->capacity * 2);
	while (queue->size > 0) {
		queue_push(new_queue, queue_pop(queue));
	}
	free(queue->array);
	*queue = *new_queue;
}

uint8_t queue_pop(Queue* queue) { 
    int item = queue->array[queue->front]; 
    queue->size--;
    queue->front++;
	if (queue->front == queue->capacity)
		queue->front = 0; 
    return item; 
} 
  
uint8_t queue_front(Queue* queue) { 
    return queue->array[queue->front]; 
} 
