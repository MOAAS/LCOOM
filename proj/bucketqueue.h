#pragma once

typedef struct {
    uint16_t x;
    uint16_t y;
    char* layer_address;
} BucketQueuePoint;

BucketQueuePoint bqueue_front();
bool bqueue_isEmpty();
bool bqueue_isFull();

int bqueue_size();
void bqueue_push(uint16_t x, uint16_t y, char* layer_address);

BucketQueuePoint bqueue_pop();
void bqueue_destroy();
