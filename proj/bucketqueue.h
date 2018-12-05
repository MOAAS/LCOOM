#pragma once

typedef struct {
    uint16_t x;
    uint16_t y;
    char* layer_address;
} BucketQueuePoint;

BucketQueuePoint queue_front();
bool queue_isEmpty();
bool queue_isFull();

int queue_size();
void queue_push(uint16_t x, uint16_t y, char* layer_address);

BucketQueuePoint queue_pop();
void queue_destroy();
