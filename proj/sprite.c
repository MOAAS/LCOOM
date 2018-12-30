#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "canvas.h"

#include "sprite.h"

Sprite *create_sprite(Bitmap* bitmap, int x, int y) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    if( sp == NULL )
    return NULL;
    // read the sprite bitmap
    sp->bitmap = bitmap;
    sp->height = bitmap->bitmapInfoHeader.height;
    sp->width = bitmap->bitmapInfoHeader.width;
    if( sp->bitmap == NULL ) {
        free(sp);
        return NULL;
    }
    sp->x = x;
    sp->y = y;
    sp->color = WHITE;
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
    draw_bitmap(sp->bitmap, sp->x, sp->y);
}

void sprite_set_color(Sprite *sp, uint32_t color) {
    sp->color = color;
}

void draw_sprite_color(Sprite *sp) {
    draw_bitmap_color(sp->bitmap, sp->x, sp->y, sp->color);
}

void erase_sprite(Sprite *sp) {
    uint16_t xCoord = sp->x;
    uint16_t yCoord = sp->y + sp->height - 1;
    char* img = sp->bitmap->bitmapData;
    for (int i = 0; i < sp->height; i++, yCoord--) {
        xCoord = sp->x;
    	for(int j = 0; j < sp->width; j++, xCoord++) {
            uint32_t color = vg_retrieve(img);
            if (!is_transparent(color)) {
                vg_draw_pixel(xCoord, yCoord, layer_get_pixel_under(get_highest_layer(), xCoord, yCoord));
            }
            img = img + sp->bitmap->bytes_per_pixel;
        }
        img = img + sp->bitmap->padding; 
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

void update_cursor(Sprite *cursor, Event_t event) {
    if (!event.isMouseEvent) {
        draw_sprite_color(cursor);
        return;
    }
    erase_sprite(cursor);
    if (event.mouseEvent.type == MOUSE_MOVE) {
        //if (abs(event.mouseEvent.delta_x) > 200 || abs(event.mouseEvent.delta_y) > 200)
        //    printf("UH OH! ");
        //printf("deltax = %d | deltay = %d \n", event.mouseEvent.delta_x, event.mouseEvent.delta_y);
        cursor->x = cursor->x + event.mouseEvent.delta_x;
        cursor->y = cursor->y - event.mouseEvent.delta_y;
        if (cursor->x < 0) cursor->x = 0;
        else if (cursor->x > (int)vg_get_hres()) cursor->x = (int)vg_get_hres();
        if (cursor->y < 0) cursor->y = 0;
        else if (cursor->y > (int)vg_get_vres()) cursor->y = (int)vg_get_vres();
    }
    draw_sprite_color(cursor);
}

void cursor_change_bmp(Sprite *cursor, Bitmap* bm){
    erase_sprite(cursor);
    cursor->bitmap = bm;
    cursor->width = bm->bitmapInfoHeader.width;
    cursor->height = bm->bitmapInfoHeader.height;
    draw_sprite_color(cursor);
}

void make_hitbox(Hitbox* hitbox, int16_t x1, int16_t x2, int16_t y1, int16_t y2) {
    hitbox->x1 = x1;
    hitbox->x2 = x2;
    hitbox->y1 = y1;
    hitbox->y2 = y2;
    hitbox->height = y2 - y1;
    hitbox->width = x2 - x1;
}

void move_hitbox(Hitbox* hitbox, int16_t delx, int16_t dely) {
    hitbox->x1 += delx;
    hitbox->x2 += delx;
    hitbox->y1 += dely;
    hitbox->y2 += dely;
}


bool check_hitbox_collision(Hitbox hitboxA, Hitbox hitboxB) {
    return 
    hitboxA.x1 < hitboxB.x2 && 
    hitboxA.x2 > hitboxB.x1 &&
    hitboxA.y1 < hitboxB.y2 && 
    hitboxA.y2 > hitboxB.y1;
}


// Button

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

void destroy_button(Button* button) {
    free(button);
}

bool is_on_button(Sprite* cursor, Button* button) {
    uint16_t height = button->bitmap->bitmapInfoHeader.height;
    uint16_t width = button->bitmap->bitmapInfoHeader.width;
    return 
    cursor->x >= button->x && 
    cursor->x < button->x + width && 
    cursor->y >= button->y && 
    cursor->y < button->y + height &&
    !is_transparent(get_bitmap_color(button->bitmap, cursor->x - button->x, cursor->y - button->y));
}

void draw_button(Sprite* cursor, Button* button) {
    layer_draw_image(button->layer, button->bitmap, button->x, button->y);
    if (is_on_button(cursor, button))
        highlight_button(button);
    else unhighlight_button(button);
}

void draw_buttons(Sprite* cursor, Button* buttons[], uint8_t num_buttons) {
    for (uint8_t i = 0; i < num_buttons; i++) {
        draw_button(cursor, buttons[i]);
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
    if (!button->isHighlighted || (button->isPressed && !button->singleState))
        return;
    button->isHighlighted = false;
    button->bitmap = button->bitmapIdle;
    layer_draw_image(button->layer, button->bitmap, button->x, button->y);
}

void highlight_button(Button* button) {
    if (button->isHighlighted)
        return;
    button->isHighlighted = true;
    button->bitmap = button->bitmapHighlighted;
    layer_draw_image(button->layer, button->bitmap, button->x, button->y);
}

void press_button(Button* button) {
    if (button->isPressed)
        return;
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
                if (buttons[i]->isPressed || buttons[i]->singleState) 
                    return i; // Ja estava premido ou é botao que nao fica ativo
                press_button(buttons[i]);
                pressedButton = i;
                break;
            }
            else highlight_button(buttons[i]);
        }
        else unhighlight_button(buttons[i]);
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

// NNY

IdleSprite* create_idle_sprite(uint16_t x, uint16_t y, Layer* layer, Bitmap* bitmap, uint32_t color) {
    IdleSprite* idlesprite = malloc(sizeof(IdleSprite));
    idlesprite->x = x;
    idlesprite->y = y;
    idlesprite->bitmap = bitmap;
    idlesprite->layer = layer;
    idlesprite->color = color;
    return idlesprite;
}

void destroy_idle_sprite(IdleSprite* idlesprite) {
    free(idlesprite);
}

void draw_idle_sprite(IdleSprite* idlesprite) {
    layer_draw_image_color(idlesprite->layer, idlesprite->bitmap, idlesprite->x, idlesprite->y, idlesprite->color);
}

void idle_sprite_change_color(IdleSprite* idlesprite, uint32_t color) {
    idlesprite->color = color;
    draw_idle_sprite(idlesprite);
}

// Slider BEC7ED

extern Bitmap* slider_bmp;

Slider* create_slider(uint16_t x, uint16_t y, double perc_filled, Layer* layer, uint32_t color, uint32_t color_active) {
    Slider* slider = malloc(sizeof(Slider));
    slider->x = x;
    slider->y = y;
    slider->width = slider_bmp->bitmapInfoHeader.width;
    slider->height = slider_bmp->bitmapInfoHeader.height;
    slider->handle_pos = x + perc_filled * slider->width;
    slider->layer = layer;
    slider->isPressed = false;
    slider->color = color;
    slider->color_active = color_active;
    return slider;
}

void draw_slider(Slider* slider) {
   //if (slider->bmp == NULL || layer == NULL)
   //    return;
    //uint16_t xCoord = x;
    //uint16_t yCoord = height + y - 1;
    char* img = slider_bmp->bitmapData;    
    for (uint16_t y = slider->y; y < slider->y + slider->height; y++) {
        for (uint16_t x = slider->x; x < slider->x + slider->width; x++) {
            uint32_t color = vg_retrieve(img) & 0xFFFFFF;
            if (color == GREEN) {
                if (x >= slider->handle_pos)
                    draw_on_layer(slider->layer, x, y, slider->color);
                else draw_on_layer(slider->layer, x, y, slider->color_active);
            }
            else if (color != MAGENTA)
                draw_on_layer(slider->layer, x, y, color);
            img += slider_bmp->bytes_per_pixel;
        }
        img += slider_bmp->padding;
    }
}        

bool is_on_slider(Sprite* cursor, Slider* slider) {
    return cursor->x >= slider->x && cursor->x < slider->x + slider->width && cursor->y >= slider->y && cursor->y < slider->y + slider->height;
}


void slider_move_handle(Slider* slider, uint16_t pos) {
    if (pos < slider->x)
        pos = slider->x;
    else if (pos > slider->x + slider->width)
        pos = slider->x + slider->width;
    slider->handle_pos = pos;
    draw_slider(slider);
}

double slider_get_perc(Slider* slider) {
    return (slider->handle_pos - slider->x) / (double)slider->width;
}

bool update_slider(Event_t event, Sprite* cursor, Slider* slider) {
    if (!event.isMouseEvent)
        return false;
    if (event.isLBPressed) {
        if (slider->isPressed || is_on_slider(cursor, slider)) {
            slider->isPressed = true;
            slider_move_handle(slider, cursor->x);
            return true;
        }
        else return false;
    }
    else { 
        slider->isPressed = false;
        return false;
    }
}
