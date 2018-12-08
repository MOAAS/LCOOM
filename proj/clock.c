#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "clock.h"

static TextBox* clock_box;
static Layer* clock_layer;
static uint16_t time_left = 0;
static char time_left_string[12];
static Layer* clock_layer = NULL;

void create_clock(Bitmap* bmp, uint16_t time_left_i) {
    clock_layer = create_layer(12, 150, 128, 128);
    clock_box = create_textbox(clock_layer, bmp, 12, 150, 36, 50, 2);
    time_left = time_left_i;
    sprintf(time_left_string, "%d", time_left_i);
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

void update_clock(Event_t event) {
    if (time_left == 0 || !(event.isTimerEvent && event.timerEvent.has_second_passed))
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
