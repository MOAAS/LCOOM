#pragma once

#include "uart.h"

#define MSG_PREFIX          137
#define MSG_TRAILER         138

typedef struct {
    uint8_t size;
    uint8_t type;
    uint8_t bytes[20];
} UARTMessage;

UARTMessage* create_message(uint8_t size, uint8_t type, uint8_t* bytes);

void destroy_message(UARTMessage* msg);

void uart_send_message(UARTMessage* msg);
bool uart_assemble_received_message(uint8_t byte, UARTMessage* msg_ptr);

UARTMessage* get_msg_by_id(uint8_t id, UARTMessage messages[], uint16_t num_messages);
