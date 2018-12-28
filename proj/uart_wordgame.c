#include <lcom/lcf.h>

#include "uart_wordgame.h"

void uart_send_draw_line(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness) {
    uint8_t bytes[MSG_DRAW_LINE_SIZE]; // size = 12;
    bytes[0] = (uint8_t)xi;
    bytes[1] = (uint8_t)(xi >> 8);

    bytes[2] = (uint8_t)yi;
    bytes[3] = (uint8_t)(yi >> 8);

    bytes[4] = (uint8_t)xf;
    bytes[5] = (uint8_t)(xf >> 8);

    bytes[6] = (uint8_t)yf;
    bytes[7] = (uint8_t)(yf >> 8);

    bytes[8] = (uint8_t)color;
    bytes[9] = (uint8_t)(color >> 8);
    bytes[10] = (uint8_t)(color >> 16);

    bytes[11] = thickness;
    UARTMessage* message = create_message(MSG_DRAW_LINE_SIZE, MSG_DRAW_LINE, bytes);
    uart_send_message(message);   
}

void uart_send_draw_line2(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness) {
    uint8_t bytes[MSG_DRAW_LINE2_SIZE]; // size = 12;
    bytes[0] = (uint8_t)xi;
    bytes[1] = (uint8_t)(xi >> 8);

    bytes[2] = (uint8_t)yi;
    bytes[3] = (uint8_t)(yi >> 8);

    bytes[4] = (uint8_t)xf;
    bytes[5] = (uint8_t)(xf >> 8);

    bytes[6] = (uint8_t)yf;
    bytes[7] = (uint8_t)(yf >> 8);

    bytes[8] = (uint8_t)color;
    bytes[9] = (uint8_t)(color >> 8);
    bytes[10] = (uint8_t)(color >> 16);

    bytes[11] = thickness;
    UARTMessage* message = create_message(MSG_DRAW_LINE2_SIZE, MSG_DRAW_LINE2, bytes);
    uart_send_message(message);   
}

void uart_send_trash() {
    uint8_t bytes[1]; // size = 0 mas n funciona em c sei la pq
    UARTMessage* message = create_message(MSG_TRASH_SIZE, MSG_TRASH, bytes);
    uart_send_message(message);   
}

void uart_send_bucket(uint16_t xi, uint16_t yi, uint32_t color) {
    uint8_t bytes[MSG_DRAW_LINE2_SIZE]; // size = 7;
    bytes[0] = (uint8_t)xi;
    bytes[1] = (uint8_t)(xi >> 8);

    bytes[2] = (uint8_t)yi;
    bytes[3] = (uint8_t)(yi >> 8);

    bytes[4] = (uint8_t)color;
    bytes[5] = (uint8_t)(color >> 8);
    bytes[6] = (uint8_t)(color >> 16);
    UARTMessage* message = create_message(MSG_BUCKET_SIZE, MSG_BUCKET, bytes);
    uart_send_message(message);   
}

void uart_send_guesser_ready() {
    uint8_t bytes[1]; // size = 0 mas n funciona em c sei la pq
    UARTMessage* message = create_message(0, MSG_GUESSER_READY, bytes);
    uart_send_message(message);
}

void uart_send_drawer_ready(char* solution) {
    UARTMessage* message = create_message(strlen(solution) + 1, MSG_DRAWER_READY, (uint8_t*)solution);
    uart_send_message(message);
}

void uart_send_game_win() {
    uint8_t bytes[1]; // size = 0 mas n funciona em c sei la pq
    UARTMessage* message = create_message(MSG_GAME_WON_SIZE, MSG_GAME_WON, bytes);
    uart_send_message(message);
}

void uart_send_game_loss() {
    uint8_t bytes[1]; // size = 0 mas n funciona em c sei la pq
    UARTMessage* message = create_message(MSG_GAME_LOST_SIZE, MSG_GAME_LOST, bytes);
    uart_send_message(message);
}

void uart_send_tick_clock() {
    uint8_t bytes[1]; // size = 0 mas n funciona em c sei la pq
    UARTMessage* message = create_message(MSG_TICK_CLOCK_SIZE, MSG_TICK_CLOCK, bytes);
    uart_send_message(message);
}

void uart_send_emote(Emote emote) {
    uint8_t bytes[1];
    bytes[0] = emote;
    UARTMessage* message = create_message(MSG_EMOTE_SIZE, MSG_EMOTE, bytes);
    uart_send_message(message);
}

void uart_process_msg(UARTMessage* message) {
    switch (message->type) {
        case MSG_DRAW_LINE: {
            uint16_t xi = (uint16_t)message->bytes[0] | (uint16_t)message->bytes[1] << 8;
            uint16_t yi = (uint16_t)message->bytes[2] | (uint16_t)message->bytes[3] << 8;
            uint16_t xf = (uint16_t)message->bytes[4] | (uint16_t)message->bytes[5] << 8;
            uint16_t yf = (uint16_t)message->bytes[6] | (uint16_t)message->bytes[7] << 8;
            uint32_t color = (uint32_t)message->bytes[8] | (uint32_t)message->bytes[9] << 8 | (uint32_t)message->bytes[10] << 16; 
            uint8_t thickness = message->bytes[11];
            if (thickness > 20) {
                thickness = 4;
                printf("Wait whaat? \n");
                for (int i = 0; i < message->size; i++) {
                    printf("Byte no. %d - %d \n", i, message->bytes[i]);
                }
            }
            canvas_draw_line(xi, yi, xf, yf, color, thickness);
            break;
        }
        case MSG_DRAW_LINE2: {
            uint16_t xi = (uint16_t)message->bytes[0] | (uint16_t)message->bytes[1] << 8;
            uint16_t yi = (uint16_t)message->bytes[2] | (uint16_t)message->bytes[3] << 8;
            uint16_t xf = (uint16_t)message->bytes[4] | (uint16_t)message->bytes[5] << 8;
            uint16_t yf = (uint16_t)message->bytes[6] | (uint16_t)message->bytes[7] << 8;
            uint32_t color = (uint32_t)message->bytes[8] | (uint32_t)message->bytes[9] << 8 | (uint32_t)message->bytes[10] << 16; 
            uint8_t thickness = message->bytes[11];
            canvas_draw_line2(xi, yi, xf, yf, color, thickness);
            break;
        }
        case MSG_TRASH:
            canvas_set_color(WHITE);
            break;
        case MSG_BUCKET: {
            uint16_t xi = (uint16_t)message->bytes[0] | (uint16_t)message->bytes[1] << 8;
            uint16_t yi = (uint16_t)message->bytes[2] | (uint16_t)message->bytes[3] << 8;
            uint32_t color = (uint32_t)message->bytes[4] | (uint32_t)message->bytes[5] << 8 | (uint32_t)message->bytes[6] << 16; 
            bucket_tool(xi, yi, color);
            break;
        }
        case MSG_TICK_CLOCK:
            wordgame_tick_clock();
            break;
        case MSG_EMOTE: {
            Emote emote = message->bytes[0];
            draw_emote(emote);
            break;
        }
        default: break;
    }
}

void uart_process_msgs(UARTMessage messages[], uint16_t num_messages) {
    for (uint16_t i = 0; i < num_messages; i++) {
        uart_process_msg(&messages[i]);
    }
}

// Emotes

extern Bitmap* emote0_bmp;
extern Bitmap* emote1_bmp;
extern Bitmap* emote2_bmp;
extern Bitmap* emote3_bmp;
extern Bitmap* emote4_bmp;
extern Bitmap* emote_wheel_bmp;

static Layer* emote_layer = NULL;

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

static int num_emote_buttons = 5;
static Button* emote_buttons[10];
static Layer* emote_wheel_layer = NULL;
static bool emote_wheel_on = false;

void toggle_emote_wheel(Sprite* cursor) {
    if (emote_wheel_on) {
        destroy_emote_wheel();
        emote_wheel_on = false;
    }
    else {
        draw_emote_wheel(cursor);
        emote_wheel_on = true;
    }
}

void draw_emote_wheel(Sprite* cursor) {
    if (emote_wheel_layer == NULL)
        emote_wheel_layer = create_layer(100, 10, 400, 200);
    layer_draw_image(emote_wheel_layer, emote_wheel_bmp, 100, 10);
    emote_buttons[0] = create_button(110, 10, emote_wheel_layer, emote0_bmp, emote0_bmp); // normal
    emote_buttons[1] = create_button(230, 10, emote_wheel_layer, emote1_bmp, emote1_bmp); // normal
    emote_buttons[2] = create_button(350, 10, emote_wheel_layer, emote2_bmp, emote2_bmp); // normal
    emote_buttons[3] = create_button(160, 110, emote_wheel_layer, emote3_bmp, emote3_bmp); // normal
    emote_buttons[4] = create_button(280, 110, emote_wheel_layer, emote4_bmp, emote4_bmp); // normal
    draw_buttons(cursor, emote_buttons, num_emote_buttons);
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
}
