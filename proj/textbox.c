#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "textbox.h"

static Bitmap* letters[256];

void loadLetterMap() {
    Bitmap* letters_bmp = loadBitmap("home/lcom/labs/proj/bitmaps/letraas.bmp");
    int i = 0;
    for (int y = 256 - 16; y >= 0; y -= 16) {
        for (int x = 0; x < 256; x += 16, i++) {
            letters[i]= loadBitmapSection(letters_bmp, x, y, 16, 16);
        }
    }
}

TextBox* create_textbox(Layer* layer, Bitmap* bitmap, uint16_t x, uint16_t y, uint8_t x_disp, uint8_t y_disp, uint8_t font_scale) {
    TextBox* textbox = malloc(sizeof(TextBox));
    textbox->bitmap = bitmap;
    textbox->layer = layer;
    textbox->x = x;
    textbox->y = y;
    textbox->cursorX = x + x_disp;
    textbox->cursorY = y + y_disp;
    textbox->cursorX_init = textbox->cursorX;
    textbox->cursorY_init = textbox->cursorY;
    textbox->cursorX_limit = textbox->cursorX_init + textbox->layer->width;
    textbox->cursorY_limit = textbox->cursorY_init + textbox->layer->height;
    textbox->isEmpty = true;
    textbox->text_size = 0;
    textbox->font_size = 16 * font_scale;
    textbox->letter_width = textbox->font_size;
    strcpy(textbox->text, "");
    layer_draw_image(layer, bitmap, x, y);
    return textbox;

}

void destroy_textbox(TextBox* textbox) {
    free(textbox);
}

void textbox_write(TextBox* textbox, char* string) {
    for (int i = 0; string[i] != 0; i++) {
        textbox_put(textbox, string[i]);
    }
}

void textbox_clear(TextBox* textbox) {
    while (textbox->text_size != 0)
        textbox_backspace(textbox);
}


void textbox_put(TextBox* textbox, char character) {
    if (textbox->text_size >= 199)
        return;
    // talvez guardar no inicio
    Bitmap* bitmap = resizeBitmap(letters[(uint8_t)character], textbox->font_size / 16);
    // atualizar char arrays
    char string[2] = {character};
    strcat(textbox->text, string);
    textbox->text_size++;
    layer_draw_image(textbox->layer, bitmap, textbox->cursorX, textbox->cursorY);
    textbox->cursorX += textbox->letter_width;
    if (textbox->cursorX >= textbox->cursorX_limit - textbox->letter_width) // ultrapassou
        textbox_linefeed(textbox);
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
    textbox->cursorX -= textbox->letter_width;
    textbox->text_size--;
    int16_t relative_x = textbox->cursorX - textbox->x;
    int16_t relative_y = textbox->cursorY - textbox->y;
    for (int8_t i = 0; i < textbox->font_size; i++) {
        for (int8_t j = 0; j < textbox->letter_width; j++) {
            uint32_t color = get_bitmap_color(textbox->bitmap, relative_x + j, relative_y + i);
            draw_on_layer(textbox->layer, textbox->cursorX + j, textbox->cursorY + i, color);
        }
    }
    if (textbox->cursorX < textbox->cursorX_init && textbox->cursorY > textbox->cursorY_init) {
        textbox->cursorX = textbox->cursorX_limit - textbox->letter_width;
        textbox->cursorY -= textbox->font_size;
    }
}

void draw_char(Layer* layer, char character, int16_t x, int16_t y, uint16_t font_scale, TextAlignment alignment){
    Bitmap* bitmap = resizeBitmap(letters[(uint8_t)character], font_scale);
    if (alignment == Center)
        x -= 8 * font_scale;
    layer_draw_image(layer, bitmap, x, y);
}

void draw_word(Layer* layer, char* word, int16_t x, int16_t y, uint16_t font_scale, uint16_t space_scale, TextAlignment alignment) {
    int16_t xDiff = 16 * (font_scale + space_scale);
    if (alignment == Center) {
        x -= xDiff * (strlen(word) - 1) / 2.0;
    }
    for (int i = 0; word[i] != '\0'; i++) {
        draw_char(layer, word[i], x, y, font_scale, alignment);
        x += xDiff;
    }
}
