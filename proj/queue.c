#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "queue.h"

Queue* queue_create(unsigned capacity) { 
    Queue* queue = malloc(sizeof(Queue)); 
    queue->capacity = capacity; 
    queue->front = queue->size = 0;  
    queue->rear = capacity - 1;  // This is important, see the enqueue 
    queue->array = malloc(queue->capacity * sizeof(uint8_t)); 
    return queue; 
} 

void queue_destroy(Queue* q) {
	free(q->array);
	free(q);
}




// Queue is full when size becomes equal to the capacity  
bool isFull(Queue* queue) {  
	return (queue->size == queue->capacity);  
} 
  
// Queue is empty when size is 0 
bool isEmpty(Queue* queue) 
{  return (queue->size == 0); } 

void queue_push(Queue* queue, uint8_t item) { 
    if (isFull(queue)) {
		printf("JEEZ QUEUE IS SO FULL!");
	}
    queue->size++;
    queue->rear++;
	if (queue->rear == queue->capacity) {
		queue->rear = 0; 
	}
    queue->array[queue->rear] = item; 
    //printf("%d enqueued to queue\n", item); 
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
  
  /*
void queue_destroy(Queue* q) {
	while (q->size != 0)
		queue_pop(q);
	free(q);
}
 
uint8_t queue_front(Queue *q) {
	return q->front->data;
}
 
void queue_pop(Queue *q) {
	q->size--; 
	struct Node *tmp = q->front;
	q->front = q->front->next;
	free(tmp);
}
 
void queue_push(Queue *q, uint8_t data) {
    q->size++; 
	if (q->front == NULL) {
		q->front = (struct Node *) malloc(sizeof(struct Node));
		q->front->data = data;
		q->front->next = NULL;
		q->last = q->front;
	} 
	else {
		q->last->next = (struct Node *) malloc(sizeof(struct Node));
		q->last->next->data = data;
		q->last->next->next = NULL;
		q->last = q->last->next;
	}
}
*/

