#include <lcom/lcf.h>

#include "bucketqueue.h"


BucketQueuePoint ptArray[1500000];
int front = 0;
int rear = -1;
int itemCount = 0;

BucketQueuePoint queue_front() {
   return ptArray[front];
}

bool queue_isEmpty() {
   return itemCount == 0;
}

int queue_size() {
   return itemCount;
}  

void queue_push(uint16_t x, uint16_t y, char* layer_address) {
	rear++;
	ptArray[rear].x = x;
	ptArray[rear].y = y;
	ptArray[rear].layer_address = layer_address;
	itemCount++;
}

BucketQueuePoint queue_pop() {
   itemCount--;
   return ptArray[front++];  
}

void queue_destroy() {
	front = 0;
	rear = -1;
	itemCount = 0;
}
