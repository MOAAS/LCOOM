#pragma once

struct Node {
	uint8_t data;
	struct Node *next;
};
 
typedef struct {
	struct Node *front;
	struct Node *last;
	unsigned int size;
} Queue; 
 
Queue* queue_create();
void queue_destroy(Queue* q);
uint8_t queue_front(Queue *q);
void queue_pop(Queue *q); 
void queue_push(Queue *q, uint8_t data);
