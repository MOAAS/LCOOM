#pragma once

#include "uart.h"
#include "canvas.h"
#include "clock.h"

#define MSG_PREFIX          137
#define MSG_TRAILER         138

#define MSG_DRAW_LINE       0
#define MSG_DRAW_LINE2      1
#define MSG_TRASH           2
#define MSG_BUCKET          3

#define MSG_DRAWER_READY    4
#define MSG_GUESSER_READY   5

#define MSG_GAME_WON        6
#define MSG_GAME_LOST       7

#define MSG_TICK_CLOCK      8

#define MSG_DRAW_LINE_SIZE  12
#define MSG_DRAW_LINE2_SIZE 12
#define MSG_TRASH_SIZE      0
#define MSG_BUCKET_SIZE     7
#define MSG_GAME_WON_SIZE   0
#define MSG_GAME_LOST_SIZE  0
#define MSG_TICK_CLOCK_SIZE 0

typedef struct {
    uint8_t size;
    uint8_t type;
    uint8_t* bytes;
} UARTMessage;

UARTMessage* create_message(uint8_t size, uint8_t type, uint8_t* bytes);

void destroy_message(UARTMessage* msg);

void free_messages(UARTMessage msgs[], uint16_t num_msgs);

void uart_send_message(UARTMessage* msg);
bool uart_assemble_received_message(uint8_t byte, UARTMessage* msg_ptr);

void uart_send_draw_line(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness);
void uart_send_draw_line2(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness);
void uart_send_trash();
void uart_send_bucket(uint16_t xi, uint16_t yi, uint32_t color);

void uart_send_guesser_ready();
void uart_send_drawer_ready(char* solution);

void uart_send_game_win();
void uart_send_game_loss();

void uart_send_tick_clock();

void uart_process_msgs(UARTMessage messages[], uint16_t num_messages);
void uart_process_msg(UARTMessage* message);

UARTMessage* get_msg_by_id(uint8_t id, UARTMessage messages[], uint16_t num_messages);
