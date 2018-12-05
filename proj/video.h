#pragma once
#include "vbe.h"

#define BLACK   0x000000
#define RED     0xFF0000
#define GREEN   0x00FF00
#define BLUE    0x0000FF
#define WHITE   0xFFFFFF

struct color {
    uint8_t R;
    uint8_t G;
    uint8_t B;
};


void* video_init(uint16_t mode);

uint32_t vg_get_pixel(uint16_t x, uint16_t y);
void vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

char* calc_address(char* init, uint16_t x, uint16_t y, uint16_t width);
void vg_insert(char* address, uint32_t color);
uint32_t vg_retrieve(char* address);

void vg_draw_image(char* map, uint16_t width, uint16_t height, uint16_t x, uint16_t y);

uint32_t vg_compose_RGB(uint8_t R, uint8_t G, uint8_t B);
struct color vg_decompose_RGB(uint32_t RGB);

unsigned int vg_get_hres();
unsigned int vg_get_vres();
unsigned int vg_get_bytes_pp();
unsigned int vg_get_bits_pp();
void* vg_get_video_mem();

bool vg_mode_is_indexed();
vbe_mode_info_t vg_get_mode();

void video_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
void video_draw_vline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

void video_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

void vg_set_background(uint32_t color);
