#pragma once

#include "layer.h"
#include "bitmap.h"

typedef enum {
    Left,
    Center
} TextAlignment;

typedef struct {
    Layer* layer;
    uint16_t x;
    uint16_t y;
    int16_t cursorX;
    int16_t cursorY;
    int16_t cursorX_init;
    int16_t cursorY_init;
    int16_t cursorX_limit;
    int16_t cursorY_limit;
    uint16_t text_size;    
    Bitmap* bitmap;
    char text[200];
    uint8_t font_size;
    bool isEmpty;
} TextBox;

void loadLetterMap();

TextBox* create_textbox(Layer* layer, Bitmap* bitmap, uint16_t x, uint16_t y, uint8_t x_disp, uint8_t y_disp, uint8_t font_scale);
void destroy_textbox(TextBox* textbox);

void textbox_write(TextBox* textbox, char* string);
void textbox_clear(TextBox* textbox);

void textbox_linefeed(TextBox* textbox);

void textbox_put(TextBox* textbox, char character);
void textbox_backspace(TextBox* textbox);

void draw_char(Layer* layer, char character, int16_t x, int16_t y, uint16_t font_scale, TextAlignment alignment);
void draw_word(Layer* layer, char* word, int16_t x, int16_t y, uint16_t font_scale, uint16_t space_scale, TextAlignment alignment);

