#pragma once
#include "layer.h"
#include "video.h"
#include "sprite.h"

typedef struct {
    uint16_t xMin, xMax;
    uint16_t yMin, yMax;
    uint16_t width;
    uint16_t height;
    Layer* layer;
} Canvas;

void create_canvas(Layer* layer, uint16_t xMin, uint16_t yMin, uint16_t xMax, uint16_t yMax, uint32_t color);

void canvas_draw_line1(uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf, uint32_t color, uint16_t thickness);
void canvas_draw_line2(uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf, uint32_t color, uint16_t thickness);

void canvas_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);

void canvas_draw_circumference(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);

void canvas_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

void canvas_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

void canvas_draw_vline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

void canvas_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

void canvas_draw_rectangle_outline(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t thickness, uint32_t color);

void canvas_draw_square(uint16_t x, uint16_t y, uint16_t side_len, uint32_t color);

void canvas_set_color(uint32_t color);
void canvas_set_outline(uint32_t color);

uint32_t rainbow();

void bucket_tool(Sprite* cursor, uint32_t cor_inicial, uint32_t cor_balde);

bool is_inside_canvas(uint16_t x, uint16_t y);
