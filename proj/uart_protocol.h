#pragma once

#include "uart.h"
#include "canvas.h"

#define MESSAGE_DRAW_LINE1_ID    1
#define MESSAGE_DRAW_LINE1_SIZE  12

typedef struct {
    uint8_t size;
    uint8_t type;
    uint8_t* bytes;
} UARTMessage;

UARTMessage* create_message(uint8_t size, uint8_t type, uint8_t* bytes);
void uart_send_message(UARTMessage* msg);
bool uart_assemble_received_message(uint8_t byte, UARTMessage* msg_ptr);

void uart_send_draw_line1(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness);
void uart_receive_draw_line1(UARTMessage* message);

