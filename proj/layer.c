#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "canvas.h"
#include "layer.h"

Layer* layers[60];
static uint8_t num_layers = 0;

Layer* create_layer(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    Layer* layer = malloc(sizeof(Layer));
    layer->x = x;
    layer->y = y;
    layer->xMax = x + width;
    layer->yMax = y + height;
    layer->width = width;
    layer->height = height;
    layer->map = malloc(vg_get_bytes_pp() * width * height);
    layer->layer_no = num_layers;
    layers[num_layers] = layer;
    num_layers++;
    for (uint16_t yCoord = y; yCoord < height + y; yCoord++) {
        for (uint16_t xCoord = x; xCoord < width + x; xCoord++) {
            layer_erase_pixel(layer, xCoord, yCoord);
        }
    }
    return layer;
}

void destroy_layer(Layer* layer) {
    for (uint16_t y = layer->y; y < layer->height + layer->y; y++) {
        for (uint16_t x = layer->x; x < layer->width + layer->x; x++) {
            layer_erase_pixel(layer, x, y);
        }
    }
    for (int i = layer->layer_no + 1; i < num_layers; i++) {
        layers[i]->layer_no--;
        layers[i-1] = layers[i];
    }
    num_layers--;
    free(layer);
    free(layer->map);
}

void layer_set_pixel(Layer* layer, uint16_t x, uint16_t y, uint32_t color) {
    if (!is_within_bounds(layer, x, y))
        return;
    char * pixel_ptr = layer->map + ((x - layer->x) + layer->width * (y - layer->y)) * vg_get_bytes_pp();
    vg_insert(pixel_ptr, color);
}

void layer_erase_pixel(Layer* layer, uint16_t x, uint16_t y) {
    if (layer->layer_no == 0) {
        draw_on_layer(layer, x, y, 0);
    }
    else {
        uint32_t colorUnder = layer_get_pixel_under(layers[layer->layer_no - 1], x, y);
        draw_on_layer(layer, x, y, colorUnder);
    }
}

uint32_t layer_get_pixel(Layer* layer, uint16_t x, uint16_t y) {
    if (!is_within_bounds(layer, x, y))
        return 0;
    char * pixel_ptr = layer->map + ((x - layer->x) + layer->width * (y - layer->y)) * vg_get_bytes_pp();
    return vg_retrieve(pixel_ptr);
}

uint32_t layer_get_pixel_under(Layer* layer, uint16_t x, uint16_t y) {
    for (int i = layer->layer_no; i >= 0; i--) {
        layer = layers[i];
        if (is_within_bounds(layer, x, y))
            break;
        else if (i == 0)
            return 0;
    }
    return layer_get_pixel(layer, x, y);
}


Layer* get_highest_layer() {
    if (num_layers == 0)
        return NULL;
    return layers[num_layers-1];
}

uint8_t get_num_layers() {
    return num_layers;
}

void draw_on_layer(Layer* layer, uint16_t x, uint16_t y, uint32_t color) {
    layer_set_pixel(layer, x, y, color);
    if (is_top_layer(layer, x, y)) {
        vg_draw_pixel(x, y, color);
    }
}

bool is_top_layer(Layer* layer, uint16_t x, uint16_t y) {
    if (!is_within_bounds(layer, x, y))
        return false;
    for (size_t i = layer->layer_no + 1; i < num_layers; i++) {
        layer = layers[i];
        if (is_within_bounds(layer, x, y))
            return false;
    }
    return true;
}
 

bool is_within_bounds(Layer* layer, uint16_t x, uint16_t y) {
    return x >= layer->x && y >= layer->y && x < layer->xMax && y < layer->yMax;
}

void layer_draw_image(Layer* layer, Bitmap* bmp, int x, int y) {
    if (bmp == NULL || layer == NULL)
        return;
    int width =  bmp->bitmapInfoHeader.width;
    int height = bmp->bitmapInfoHeader.height;
    uint16_t xCoord = x;
    uint16_t yCoord = height + y - 1;
    char* img = bmp->bitmapData;    
    for (int i = 0; i < height; i++, yCoord--) {
        xCoord = x;
    	for(int j = 0; j < width; j++, xCoord++) {
            uint32_t color = vg_retrieve(img);
            if (!is_transparent(color)) {                
                draw_on_layer(layer, xCoord, yCoord, color);
            }
            img += bmp->bytes_per_pixel;
        }
        img += bmp->padding;
    }
}


void layer_draw_image_color(Layer* layer, Bitmap* bmp, int x, int y, uint32_t new_color) {
    if (bmp == NULL || layer == NULL)
        return;
    int width =  bmp->bitmapInfoHeader.width;
    int height = bmp->bitmapInfoHeader.height;
    uint16_t xCoord = x;
    uint16_t yCoord = height + y - 1;
    char* img = bmp->bitmapData;
    for (int i = 0; i < height; i++, yCoord--) {
        xCoord = x;
    	for(int j = 0; j < width; j++, xCoord++) {
            uint32_t color = vg_retrieve(img);
            if ((color & 0xFFFFFF) == GREEN)
                draw_on_layer(layer, xCoord, yCoord, new_color);
            else if (!is_transparent(color))
                draw_on_layer(layer, xCoord, yCoord, color);
            img += bmp->bytes_per_pixel;
        }
        img += bmp->padding;
    }
}


