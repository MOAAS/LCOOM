#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "queue.h"

Queue* queue_create() {
	Queue* q = malloc(sizeof(Queue));
	q->front = NULL;
	q->last = NULL;
	q->size = 0;
	return q;
}

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

