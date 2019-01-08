#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "textbox.h"

static Bitmap* letters_small[256];
static Bitmap* letters_medium[256];
static Bitmap* letters_big[256];

extern Bitmap* letters_bmp;

void loadLetterMap() {
    int i = 0;
    for (int y = 256 - 16; y >= 0; y -= 16) {
        for (int x = 0; x < 256; x += 16, i++) {
            letters_small[i] = loadBitmapSection(letters_bmp, x, y, 16, 16);
        }
    }
    for (int i = 0; i < 256; i++) {
        letters_medium[i] = resizeBitmap(letters_small[i], 2);
        letters_big[i] = resizeBitmap(letters_small[i], 3);
    }
}

TextBox* create_textbox(Layer* layer, Bitmap* bitmap, uint16_t x_disp, uint16_t y_disp, uint8_t font_scale, TextAlignment alignment) {
    TextBox* textbox = malloc(sizeof(TextBox)); 
    textbox->bitmap = bitmap;
    textbox->layer = layer;
    textbox->x = layer->x;
    textbox->y = layer->y;
    textbox->cursorX = textbox->x + x_disp;
    textbox->cursorY = textbox->y + y_disp;
    textbox->cursorX_init = textbox->cursorX;
    textbox->cursorY_init = textbox->cursorY;
    
    textbox->font_size = 16 * font_scale;
    textbox->text_size = 0;

    textbox->cursorX_limit = textbox->x + textbox->layer->width;
    textbox->cursorY_limit = textbox->y + textbox->layer->height;
   
    textbox->isEmpty = true;
    textbox->alignment = alignment;
    strcpy(textbox->text, "");
    layer_draw_image(layer, bitmap, layer->x, layer->y);
    return textbox;
}

void destroy_textbox(TextBox* textbox) {
    destroy_layer(textbox->layer);
    free(textbox);
}

void textbox_write(TextBox* textbox, char* string) {
    for (int i = 0; string[i] != '\0'; i++) {
        textbox_put(textbox, string[i]);
    }
}

void textbox_write_int(TextBox* textbox, int number) {
    // Calcula numero de digitos
    int num_digits;
    if (abs(number) < 10)
        num_digits = 1;
    else num_digits = floor(log10(abs(number))) + 1;
    if (number < 0)
        num_digits++;
    // Copia numero para uma string
    char* number_str = malloc(num_digits + 5); 
    sprintf(number_str, "%d", number);
    textbox_write(textbox, number_str);
    free(number_str);
}

void textbox_clear(TextBox* textbox) {
    while (textbox->text_size != 0)
        textbox_backspace(textbox);
}


void textbox_put(TextBox* textbox, char character) {
    if (textbox->text_size >= 90)
        return;
    if (textbox->cursorX >= textbox->cursorX_limit) // ultrapassou
        textbox_linefeed(textbox);
    char string[2] = {character}; // precisa de null terminator
    strcat(textbox->text, string);
    textbox->text_size++;
    draw_char(textbox->layer, character, textbox->cursorX, textbox->cursorY, textbox->font_size / 16, textbox->alignment);
    textbox->cursorX += textbox->font_size;
}

void textbox_linefeed(TextBox* textbox) {
    textbox->cursorX = textbox->cursorX_init;
    textbox->cursorY += textbox->font_size;
}


void string_pop_char(char string[]) {
    string[strlen(string) - 1] = '\0';
}

void textbox_backspace(TextBox* textbox) {
    if (textbox->text_size == 0)
        return;
    string_pop_char(textbox->text);
    textbox->cursorX -= textbox->font_size;
    textbox->text_size--;
    int16_t relative_x = textbox->cursorX - textbox->x;
    int16_t relative_y = textbox->cursorY - textbox->y;
    int16_t xCoord = textbox->cursorX;
    int16_t yCoord = textbox->cursorY;
    if (textbox->alignment == CenterAlign) {
        relative_x -= textbox->font_size / 2;
        relative_y -= textbox->font_size / 2;
        xCoord -= textbox->font_size / 2;
        yCoord -= textbox->font_size / 2;
    }

    for (int8_t i = 0; i < textbox->font_size; i++) {
        for (int8_t j = 0; j < textbox->font_size; j++) {
            uint32_t color = get_bitmap_color(textbox->bitmap, relative_x + j, relative_y + i);
            draw_on_layer(textbox->layer, xCoord + j, yCoord + i, color);
        }
    }
    if (textbox->cursorX <= textbox->cursorX_init && textbox->cursorY > textbox->cursorY_init) {
        textbox->cursorX = textbox->cursorX_limit;
        textbox->cursorY -= textbox->font_size;
    }
}

void draw_char(Layer* layer, char character, int16_t x, int16_t y, uint16_t font_scale, TextAlignment alignment){
    Bitmap* bitmap; 
    switch (font_scale) {
        case 0: return;
        case 1: bitmap = letters_small[(uint8_t)character]; break;
        case 2: bitmap = letters_medium[(uint8_t)character]; break;
        case 3: bitmap = letters_big[(uint8_t)character]; break;
        default: bitmap = resizeBitmap(letters_small[(uint8_t)character], font_scale); break;
    }
    if (alignment == CenterAlign) {
        x -= 8 * font_scale;
        y -= 8 * font_scale;
    }
    layer_draw_image(layer, bitmap, x, y);
    if (font_scale > 3)
        deleteBitmap(bitmap);
}

void draw_word(Layer* layer, char* word, int16_t x, int16_t y, uint16_t font_scale, uint16_t space_scale, TextAlignment alignment) {
    int16_t xDiff = 16 * (font_scale + space_scale);
    if (alignment == CenterAlign) {
        x -= xDiff * (strlen(word) - 1) / 2.0;
    }
    for (int i = 0; word[i] != '\0'; i++) {     
        draw_char(layer, word[i], x, y, font_scale, alignment);
        x += xDiff;
    }
}
