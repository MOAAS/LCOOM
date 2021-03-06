#include <lcom/lcf.h>
#include <stdint.h>

#include "uart_protocol.h"

UARTMessage* create_message(uint8_t size, uint8_t type, uint8_t* bytes) {
    UARTMessage* msg = malloc(sizeof(UARTMessage));
    msg->size = size;
    msg->type = type;
    for (uint8_t i = 0; i < size; i++) {
        msg->bytes[i] = bytes[i];
    }
    return msg;
}

void destroy_message(UARTMessage* msg) {
    free(msg);
}

void uart_send_message(uint8_t type, uint8_t size, uint8_t* bytes) {
    uint8_t size_id = (size << 4) | (type & 0xF);
    uart_fifo_send(MSG_PREFIX);
    uart_fifo_send(size_id);
   // printf("Msg size = %d \n", msg->size);
    for (int i = 0; i < size; i++) {
        uart_fifo_send(bytes[i]);
    }
    uart_fifo_send(MSG_TRAILER);
}

void uart_send_empty_msg(uint8_t id) {
    uart_send_message(id, 0, NULL);
}


bool uart_assemble_received_message(uint8_t byte, UARTMessage* msg_ptr) {
    static uint8_t byte_no = 0;
    static UARTMessage new_message;
    //printf("BYTE = %d... | byte_no = %d | size = %d\n", byte, byte_no, new_message.size);
    switch (byte_no) {
        case 0:
            if (byte == 137)
                byte_no = 1;
            return false;
        case 1:
            new_message.size = (byte >> 4) & 0xF;
            new_message.type = byte & 0xF;
            byte_no = 2;
            return false;
        default:
            if (byte_no == new_message.size + 2) {
                byte_no = 0;
                if (byte == 138) {
                    *msg_ptr = new_message;
                    return true;
                }
                else return false;
            }
            else {
                new_message.bytes[byte_no - 2] = byte;
                byte_no++;
                return false;                
            }
    }
}

UARTMessage* get_msg_by_id(uint8_t id, UARTMessage messages[], uint16_t num_messages){
    for (uint16_t i = 0; i < num_messages; i++) {
        if (messages[i].type == id)
            return &messages[i];
    }
    return NULL;
}
