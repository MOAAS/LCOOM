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

#define MSG_DRAWER_READY    4
#define MSG_GUESSER_READY   5

#define MSG_ROUND_WON       6
#define MSG_ROUND_LOST      7

#define MSG_TICK_CLOCK      8
#define MSG_EMOTE           9
#define MSG_NUMBER          10

#define MSG_COLLAB_READY    11

#define MSG_DRAW_LINE_SIZE  12
#define MSG_DRAW_LINE2_SIZE 12
#define MSG_BUCKET_SIZE     7
#define MSG_EMOTE_SIZE      1
#define MSG_NUMBER_SIZE     4


typedef struct {
    uint8_t type;
    uint16_t xi, yi, xf, yf;
    uint32_t color;
    uint8_t thickness;
    Emote emote;
    int number;
} UARTMessageContents;

void uart_send_draw_line(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness);
void uart_send_draw_line2(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness);
void uart_send_trash();
void uart_send_bucket(uint16_t xi, uint16_t yi, uint32_t color);

void uart_send_drawer_ready(char* solution);

void uart_send_emote(Emote emote);
void uart_send_number(int number);

void uart_process_msgs(UARTMessage messages[], uint16_t num_messages);
UARTMessageContents uart_process_msg(UARTMessage* message);


