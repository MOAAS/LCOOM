#pragma once

#include "uart_protocol.h"
#include "canvas.h"
#include "sprite.h"
#include "wordpicker.h"
#include "bitmaps.h"
#include "emote.h"

#define MSG_DRAW_LINE       0
#define MSG_DRAW_LINE2      1
#define MSG_TRASH           2
#define MSG_BUCKET          3
#define MSG_DRAW_SHAPE      4

#define MSG_DRAWER_READY    5
#define MSG_GUESSER_READY   6

#define MSG_ROUND_WON       7
#define MSG_ROUND_LOST      8

#define MSG_TICK_CLOCK      9
#define MSG_EMOTE           10
#define MSG_NUMBER          11

#define MSG_COLLAB_READY    12


#define MSG_DRAW_LINE_SIZE  12
#define MSG_DRAW_LINE2_SIZE 12
#define MSG_BUCKET_SIZE     7
#define MSG_EMOTE_SIZE      1
#define MSG_NUMBER_SIZE     4
#define MSG_DRAW_SHAPE_SIZE 13


typedef struct {
    uint8_t type;
    uint16_t xi, yi, xf, yf;
    uint32_t color;
    uint8_t thickness;
    Shape shape;
    Emote emote;
    int number;
} UARTMessageContents;

void uart_send_draw_line(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness);
void uart_send_draw_line2(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness);
void uart_send_draw_shape(Shape shape, uint16_t click1_x, uint16_t click1_y, uint16_t click2_x, uint16_t click2_y, uint32_t color, uint16_t thickness);

void uart_send_trash();
void uart_send_bucket(uint16_t xi, uint16_t yi, uint32_t color);

void uart_send_drawer_ready(char* solution);

void uart_send_emote(Emote emote);
void uart_send_number(int number);


void uart_process_msgs(UARTMessage messages[], uint16_t num_messages);
UARTMessageContents uart_process_msg(UARTMessage* message);


