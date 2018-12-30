#include <lcom/lcf.h>

#include "emote.h"

static Layer* emote_layer = NULL;
static Layer* emote_wheel_layer = NULL;

static int num_emote_buttons = 5;
static Button* emote_buttons[10];

static bool emote_wheel_on = false;

extern Bitmap* emote0_bmp;
extern Bitmap* emote1_bmp;
extern Bitmap* emote2_bmp;
extern Bitmap* emote3_bmp;
extern Bitmap* emote4_bmp;

extern Bitmap* emote0_hl_bmp;
extern Bitmap* emote1_hl_bmp;
extern Bitmap* emote2_hl_bmp;
extern Bitmap* emote3_hl_bmp;
extern Bitmap* emote4_hl_bmp;
extern Bitmap* emote_wheel_bmp;


void draw_emote(Emote emote) {
    if (emote_layer != NULL) {
        destroy_layer(emote_layer);
    }
    emote_layer = create_layer(900, 30, 115, 90);
    switch (emote) {
        case Hello: layer_draw_image(emote_layer, emote0_bmp, 900, 30); break;
        case GoodGame: layer_draw_image(emote_layer, emote1_bmp, 900, 30); break;
        case ThankYou: layer_draw_image(emote_layer, emote2_bmp, 900, 30); break;
        case YouWelcome: layer_draw_image(emote_layer, emote3_bmp, 900, 30); break;
        case GoodLuck: layer_draw_image(emote_layer, emote4_bmp, 900, 30); break;
        default: printf("Unknown emote: %d \n", emote); break;
    }
}

void destroy_emote() {
    if (emote_layer != NULL) 
        destroy_layer(emote_layer);
    emote_layer = NULL;
}

bool is_emote_wheel_on() {
    return emote_wheel_on;
}

void toggle_emote_wheel(Button* emote_wheel, Sprite* cursor) {
    if (emote_wheel_on) {
        unpress_button(emote_wheel);
        destroy_emote_wheel();
    }
    else {
        press_button(emote_wheel);
        draw_emote_wheel(cursor);
    }
}

void draw_emote_wheel(Sprite* cursor) {
    if (emote_wheel_layer == NULL)
        emote_wheel_layer = create_layer(100, 10, 380, 200);
    layer_draw_image(emote_wheel_layer, emote_wheel_bmp, 100, 10);
    emote_buttons[0] = create_button(110, 13, emote_wheel_layer, emote0_bmp, emote0_hl_bmp); 
    emote_buttons[1] = create_button(230, 13, emote_wheel_layer, emote1_bmp, emote1_hl_bmp); 
    emote_buttons[2] = create_button(350, 13, emote_wheel_layer, emote2_bmp, emote2_hl_bmp); 
    emote_buttons[3] = create_button(160, 110, emote_wheel_layer, emote3_bmp, emote3_hl_bmp);
    emote_buttons[4] = create_button(280, 110, emote_wheel_layer, emote4_bmp, emote4_hl_bmp);
    draw_buttons(cursor, emote_buttons, num_emote_buttons);
    emote_wheel_on = true;
}

void destroy_emote_wheel() {
    if (emote_wheel_layer == NULL)
        return;
    destroy_layer(emote_wheel_layer);
    emote_wheel_layer = NULL;
    destroy_button(emote_buttons[0]);
    destroy_button(emote_buttons[1]);
    destroy_button(emote_buttons[2]);
    destroy_button(emote_buttons[3]);
    destroy_button(emote_buttons[4]);
    emote_wheel_on = false;
}

int check_emote_press(Event_t event, Sprite* cursor) {
    if (!emote_wheel_on)
        return -1;
    return checkButtonPress(event, cursor, emote_buttons, num_emote_buttons);
}
