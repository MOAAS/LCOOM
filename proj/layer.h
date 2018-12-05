#pragma once

#include "video.h"
#include "bitmap.h"

typedef struct {
    char* map;
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;    
    uint8_t layer_no;    
} Layer;

Layer* create_layer(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void destroy_layer(Layer* layer);

Layer* get_highest_layer();

uint8_t get_num_layers();

void draw_on_layer(Layer* layer, uint16_t x, uint16_t y, uint32_t color);
bool is_top_layer(Layer* layer, uint16_t x, uint16_t y);

void layer_set_pixel(Layer* layer, uint16_t x, uint16_t y, uint32_t color);
void layer_erase_pixel(Layer* layer, uint16_t x, uint16_t y);

uint32_t layer_get_pixel(Layer* layer, uint16_t x, uint16_t y);
uint32_t layer_get_pixel_under(Layer* layer, uint16_t x, uint16_t y);

bool is_within_bounds(Layer* layer, uint16_t x, uint16_t y);

void layer_draw_image(Layer* layer, Bitmap* bmp, int x, int y);




