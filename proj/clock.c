#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "clock.h"

static uint16_t initial_time_left = 90;

static TextBox* clock_box;
static Layer* clock_layer;
static uint16_t time_left = 0;
static char time_left_string[12];
static Layer* clock_layer = NULL;

void create_clock(Bitmap* bmp) {
    clock_layer = create_layer(12, 150, 128, 128);
    clock_box = create_textbox(clock_layer, bmp, 36, 50, 2, LeftAlign);
    time_left = initial_time_left;
    sprintf(time_left_string, "%d", initial_time_left);
    textbox_write(clock_box, time_left_string); 
}

void destroy_clock() {
    destroy_layer(clock_layer);
    clock_layer = NULL;
    time_left = 0;
}

uint16_t clock_get_time_left() {
    return time_left;
}

bool clock_time_up() {
    return time_left == 0;
}

void tick_clock() {
    if (time_left == 0)
        return;
    time_left--;
    textbox_clear(clock_box);
    sprintf(time_left_string, "%d", time_left);
    if (time_left == 9) {
        clock_box->cursorX_init += 16;
        clock_box->cursorX += 16;
    }
    textbox_write(clock_box, time_left_string);        
}
