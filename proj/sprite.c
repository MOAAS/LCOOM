#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "canvas.h"


#include "sprite.h"

Sprite *create_sprite(const char *file, int x, int y) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    if( sp == NULL )
    return NULL;
    // read the sprite bitmao
    Bitmap* bm = loadBitmap(file);
    sp->bitmap = bm;
    sp->height = bm->bitmapInfoHeader.height;
    sp->width = bm->bitmapInfoHeader.width;
    if( sp->bitmap == NULL ) {
        free(sp);
        return NULL;
    }
    sp->x = x;
    sp->y = y;
    return sp;
}

void destroy_sprite(Sprite *sp) {
    if(sp == NULL)
        return;
    if(sp ->bitmap)
        free(sp->bitmap);
    free(sp);
    sp = NULL; // XXX: pointer is passed by value
}

void draw_sprite(Sprite *sp) {
    draw_bitmap(sp->bitmap, sp->x, sp->y, ALIGN_LEFT);
}

void draw_sprite_color(Sprite *sp,uint32_t new_color) {
    draw_bitmap_color(sp->bitmap, sp->x, sp->y, ALIGN_LEFT,new_color);
}

void erase_sprite(Sprite *sp) {
    uint16_t xCoord = sp->x;
    uint16_t yCoord = sp->y + sp->height - 1;
    unsigned char* img = sp->bitmap->bitmapData;
    for (int i = 0; i < sp->height; i++, yCoord--) {
        xCoord = sp->x;
    	for(int j = 0; j < sp->width; j++, xCoord++) {
            uint32_t color = *(uint32_t*)img;
            if (!is_transparent(color)) {
                vg_draw_pixel(xCoord, yCoord, layer_get_pixel_under(get_highest_layer(), xCoord, yCoord));
            }
            img = img + sp->bitmap->bytes_per_pixel;
        }
        img = img + sp->bitmap->padding * sp->bitmap->bytes_per_pixel;
    }
    return;
}

void move_sprite(Sprite *sp, int16_t delX, int16_t delY) {
    erase_sprite(sp); 
    sp->x = sp->x + delX;
    sp->y = sp->y + delY;
    draw_sprite(sp);
}

uint16_t cursor_get_xf(Sprite* cursor, int16_t delX) {
    int xf = cursor->x + delX;
    if (xf < 0) xf = 0;
    else if (xf > (int)vg_get_hres()) xf = (int)vg_get_hres();
    return xf;
}

uint16_t cursor_get_yf(Sprite* cursor, int16_t delY) {
    int yf = cursor->y + delY;
    if (yf < 0) yf = 0;
    else if (yf > (int)vg_get_vres()) yf = (int)vg_get_vres();
    return yf;
}

void update_cursor(Sprite *cursor, Event_t event, uint32_t cor) {
    if (!event.isMouseEvent) {
        draw_sprite_color(cursor,cor);
        return;
    }
    erase_sprite(cursor);
    if (event.mouseEvent.type == MOUSE_MOVE) {
        cursor->x = cursor->x + event.mouseEvent.delta_x;
        cursor->y = cursor->y - event.mouseEvent.delta_y;
        if (cursor->x < 0) cursor->x = 0;
        else if (cursor->x > (int)vg_get_hres()) cursor->x = (int)vg_get_hres();
        if (cursor->y < 0) cursor->y = 0;
        else if (cursor->y > (int)vg_get_vres()) cursor->y = (int)vg_get_vres();
    }
    draw_sprite_color(cursor,cor);
}

void cursor_change_bmp(Sprite *cursor,Bitmap* bm){
    erase_sprite(cursor);
    cursor->bitmap = bm;
    draw_sprite_color(cursor,WHITE);
}

Button* create_button(uint16_t x, uint16_t y, Layer* layer, Bitmap* bitmapIdle, Bitmap* bitmapHighlighted) {
    Button* button = malloc(sizeof(Button));
    button->x = x;
    button->y = y;
    button->isHighlighted = false;
    button->isPressed = false;
    button->singleState = false;
    button->bitmap = bitmapIdle;
    button->bitmapIdle = bitmapIdle;
    button->bitmapHighlighted = bitmapHighlighted;
    button->layer = layer;
    return button;
}

bool is_on_button(Sprite* cursor, Button* button) {
    uint16_t height = button->bitmap->bitmapInfoHeader.height;
    uint16_t width = button->bitmap->bitmapInfoHeader.width;
    return 
    cursor->x >= button->x && 
    cursor->x < button->x + width && 
    cursor->y >= button->y && 
    cursor->y < button->y + height &&
    layer_get_pixel_under(get_highest_layer(), cursor->x, cursor->y) == get_bitmap_color(button->bitmap, cursor->x - button->x, cursor->y - button->y);
}

void draw_button(Button* button) {
    layer_draw_image(button->layer, button->bitmap, button->x, button->y);
}

void draw_buttons(Button* buttons[], uint8_t num_buttons) {
    for (uint8_t i = 0; i < num_buttons; i++) {
        draw_button(buttons[i]);
    }
}

void disable_buttons(Button* buttons[], uint8_t num_buttons) {
    for (uint8_t i = 0; i < num_buttons; i++) {
        buttons[i]->bitmap = buttons[i]->bitmapIdle;
        buttons[i]->isHighlighted = false;
        buttons[i]->isPressed = false;
    }
}

void unhighlight_button(Button* button) {
    button->isHighlighted = false;
    button->bitmap = button->bitmapIdle;
    draw_button(button);
}

void highlight_button(Button* button) {
    button->isHighlighted = true;
    button->bitmap = button->bitmapHighlighted;
    draw_button(button);
}

void press_button(Button* button) {
    button->isPressed = true;
    highlight_button(button);
}

void unpress_button(Button* button) {
    button->isPressed = false;
    unhighlight_button(button);
}

int checkButtonPress(Event_t event, Sprite* cursor, Button* buttons[], uint8_t num_buttons) {
    if (!event.isMouseEvent)
        return -1;
    int pressedButton = -1;
    for (uint8_t i = 0; i < num_buttons; i++) {
        if (is_on_button(cursor, buttons[i])) {
            if (event.mouseEvent.type == LB_PRESSED) {
                if (buttons[i]->isPressed || buttons[i]->singleState) // Ja estava premido ou é botao que nao fica ativo
                    return i;
                press_button(buttons[i]);
                pressedButton = i;
                break;
            }
            else highlight_button(buttons[i]);
        }
        else if (buttons[i]->isHighlighted && (!buttons[i]->isPressed || buttons[i]->singleState))
            unhighlight_button(buttons[i]);
    }
    // Nenhum botao premido
    if (pressedButton == -1)
        return -1;
    // Larga os outros botoes
    for (uint8_t i = 0; i < pressedButton; i++)
        unpress_button(buttons[i]);
    for (uint8_t i = pressedButton + 1; i < num_buttons; i++)
        unpress_button(buttons[i]);
    return pressedButton;
}

NNY* create_NNY(uint16_t x, uint16_t y, Layer* layer, Bitmap* bitmap) {
    NNY* nny = malloc(sizeof(NNY));
    nny->x = x;
    nny->y = y;
    nny->bitmap = bitmap;
    nny->layer = layer;
    return nny;
}

void draw_nny(NNY* nny) {
    layer_draw_image(nny->layer, nny->bitmap, nny->x, nny->y);
}

void nny_change_color(NNY* nny,uint32_t color) {
    layer_draw_image_color(nny->layer, nny->bitmap, nny->x, nny->y,color);
}
