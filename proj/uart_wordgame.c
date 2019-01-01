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
    uart_send_message(MSG_DRAW_LINE, MSG_DRAW_LINE_SIZE, bytes);
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
    uart_send_message(MSG_DRAW_LINE2, MSG_DRAW_LINE2_SIZE, bytes);
}

void uart_send_draw_shape(Shape shape, uint16_t click1_x, uint16_t click1_y, uint16_t click2_x, uint16_t click2_y, uint32_t color, uint16_t thickness) {
    uint8_t bytes[MSG_DRAW_SHAPE_SIZE]; // size = 13;
    bytes[0] = (uint8_t)shape;

    bytes[1] = (uint8_t)click1_x;
    bytes[2] = (uint8_t)(click1_x >> 8);

    bytes[3] = (uint8_t)click1_y;
    bytes[4] = (uint8_t)(click1_y >> 8);

    bytes[5] = (uint8_t)click2_x;
    bytes[6] = (uint8_t)(click2_x >> 8);

    bytes[7] = (uint8_t)click2_y;
    bytes[8] = (uint8_t)(click2_y >> 8);

    bytes[9] = (uint8_t)color;
    bytes[10] = (uint8_t)(color >> 8);
    bytes[11] = (uint8_t)(color >> 16);

    bytes[12] = thickness;
    uart_send_message(MSG_DRAW_SHAPE, MSG_DRAW_SHAPE_SIZE, bytes);
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
    uart_send_message(MSG_BUCKET, MSG_BUCKET_SIZE, bytes);
}

void uart_send_drawer_ready(char* solution) {
    uart_send_message(MSG_DRAWER_READY, strlen(solution) + 1, (uint8_t*)solution);
}

void uart_send_emote(Emote emote) {
    uint8_t bytes[MSG_EMOTE_SIZE];
    bytes[0] = emote;
    uart_send_message(MSG_EMOTE, MSG_EMOTE_SIZE, bytes);
}

void uart_send_number(int number) {
    uint8_t bytes[MSG_NUMBER_SIZE];
    bytes[0] = (uint8_t)number;
    bytes[1] = (uint8_t)(number >> 8);
    bytes[2] = (uint8_t)(number >> 16);
    bytes[3] = (uint8_t)(number >> 24);
    uart_send_message(MSG_NUMBER, MSG_NUMBER_SIZE, bytes);
}

void uart_send_tick_clock(uint8_t time_left) {
    uint8_t bytes[MSG_TICK_CLOCK_SIZE];
    bytes[0] = time_left;
    uart_send_message(MSG_TICK_CLOCK, MSG_TICK_CLOCK_SIZE, bytes);
}

UARTMessageContents uart_process_msg(UARTMessage* message) {
    UARTMessageContents contents;
    contents.type = message->type;
    switch (message->type) {
        case MSG_DRAW_LINE:             
            contents.xi = (uint16_t)message->bytes[0] | (uint16_t)message->bytes[1] << 8;
            contents.yi = (uint16_t)message->bytes[2] | (uint16_t)message->bytes[3] << 8;
            contents.xf = (uint16_t)message->bytes[4] | (uint16_t)message->bytes[5] << 8;
            contents.yf = (uint16_t)message->bytes[6] | (uint16_t)message->bytes[7] << 8;
            contents.color = (uint32_t)message->bytes[8] | (uint32_t)message->bytes[9] << 8 | (uint32_t)message->bytes[10] << 16; 
            contents.thickness = message->bytes[11];
            break;
        case MSG_DRAW_LINE2:
            contents.xi = (uint16_t)message->bytes[0] | (uint16_t)message->bytes[1] << 8;
            contents.yi = (uint16_t)message->bytes[2] | (uint16_t)message->bytes[3] << 8;
            contents.xf = (uint16_t)message->bytes[4] | (uint16_t)message->bytes[5] << 8;
            contents.yf = (uint16_t)message->bytes[6] | (uint16_t)message->bytes[7] << 8;
            contents.color = (uint32_t)message->bytes[8] | (uint32_t)message->bytes[9] << 8 | (uint32_t)message->bytes[10] << 16; 
            contents.thickness = message->bytes[11];
            break;
        case MSG_DRAW_SHAPE:
            contents.shape = message->bytes[0];
            contents.xi = (uint16_t)message->bytes[1] | (uint16_t)message->bytes[2] << 8;
            contents.yi = (uint16_t)message->bytes[3] | (uint16_t)message->bytes[4] << 8;
            contents.xf = (uint16_t)message->bytes[5] | (uint16_t)message->bytes[6] << 8;
            contents.yf = (uint16_t)message->bytes[7] | (uint16_t)message->bytes[8] << 8;
            contents.color = (uint32_t)message->bytes[9] | (uint32_t)message->bytes[10] << 8 | (uint32_t)message->bytes[11] << 16; 
            contents.thickness = message->bytes[12];        
            break;
        case MSG_BUCKET:
            contents.xi = (uint16_t)message->bytes[0] | (uint16_t)message->bytes[1] << 8;
            contents.yi = (uint16_t)message->bytes[2] | (uint16_t)message->bytes[3] << 8;
            contents.color = (uint32_t)message->bytes[4] | (uint32_t)message->bytes[5] << 8 | (uint32_t)message->bytes[6] << 16; 
            break;
        case MSG_EMOTE:
            contents.emote = message->bytes[0];
            break;
        case MSG_NUMBER:
            contents.number = (int)message->bytes[0] | (int)message->bytes[1] << 8 | (int)message->bytes[2] << 16 | (int)message->bytes[3] << 24;
            break;
        case MSG_TICK_CLOCK:
            contents.time_left = message->bytes[0];
            break;
        default: break;
    }
    return contents;
}

void uart_process_msgs(UARTMessage messages[], uint16_t num_messages) {
    UARTMessageContents cont;
    for (uint16_t i = 0; i < num_messages; i++) {
        cont = uart_process_msg(&messages[i]);
        switch (cont.type) {
            case MSG_DRAW_LINE: canvas_draw_line(cont.xi, cont.yi, cont.xf, cont.yf, cont.color, cont.thickness); break;
            case MSG_DRAW_LINE2: canvas_draw_line2(cont.xi, cont.yi, cont.xf, cont.yf, cont.color, cont.thickness); break;
            case MSG_DRAW_SHAPE: canvas_draw_shape(cont.shape, cont.xi, cont.yi, cont.xf, cont.yf, cont.color, cont.thickness); break;
            case MSG_TRASH: canvas_set_color(WHITE); break;
            case MSG_BUCKET: bucket_tool(cont.xi, cont.yi, cont.color); break;
            case MSG_TICK_CLOCK: wordgame_set_clock(cont.time_left); break;
            case MSG_EMOTE: draw_emote(cont.emote); break;
        }
    }
}

