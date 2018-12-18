#include <lcom/lcf.h>
#include <stdint.h>

#include "uart_protocol.h"

UARTMessage* create_message(uint8_t size, uint8_t type, uint8_t* bytes) {
    UARTMessage* msg = malloc(sizeof(UARTMessage));
    msg->size = size;
    msg->type = type;
    msg->bytes = malloc(size*sizeof(uint8_t));
    for (uint8_t i = 0; i < size; i++) {
        msg->bytes[i] = bytes[i];
    }
    return msg;
}

void destroy_message(UARTMessage* msg) {
    free(msg->bytes);
    free(msg);
}

void uart_send_message(UARTMessage* msg) {
    uint8_t size_id = (msg->size << 4) | msg->type;
    uart_fifo_send(137);
    uart_fifo_send(size_id);
    for (int i = 0; i < msg->size; i++) {
        uart_fifo_send(msg->bytes[i]);
    }
    uart_fifo_send(138);
}

bool uart_assemble_received_message(uint8_t byte, UARTMessage* msg_ptr) {
    static uint8_t byte_no = 0;
    static UARTMessage new_message;
   // printf("BYTE = %d... | byte_no = %d | size = %d\n", byte, byte_no, new_message.size);
  // static uint8_t size = 0;
  // static uint8_t type = 0;
  // static uint8_t bytes[128];
    switch (byte_no) {
        case 0:
            if (byte == 137)
                byte_no = 1;
            return false;
        case 1:
            new_message.size = (byte >> 4) & 0xF;
            new_message.bytes = malloc(new_message.size*sizeof(uint8_t));
            new_message.type = byte & 0xF;
            byte_no = 2;
            return false;
        default:
            if (byte_no == new_message.size + 2) {
             //   free(new_message.bytes); 
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

void uart_send_draw_line1(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness) {
    uint8_t bytes[MESSAGE_DRAW_LINE1_SIZE]; // size = 12;
    printf("xi = %d \n", xi);
    printf("xf = %d \n", xf);
    printf("yi = %d \n", yi);
    printf("yf = %d \n", yf);
    printf("color = %d \n", color);
    printf("thickness = %d \n", thickness);
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
    UARTMessage* message = create_message(MESSAGE_DRAW_LINE1_SIZE, MESSAGE_DRAW_LINE1_ID, bytes);
    uart_send_message(message);   
}

// talvez mudar para process_msg e por noutro sitio tipo um event dispatcher.....
void uart_receive_draw_line1(UARTMessage* message) {
    if (message->size != MESSAGE_DRAW_LINE1_SIZE)
        return;
    uint16_t xi = (uint16_t)message->bytes[0] | (uint16_t)message->bytes[1] << 8;
    uint16_t yi = (uint16_t)message->bytes[2] | (uint16_t)message->bytes[3] << 8;
    uint16_t xf = (uint16_t)message->bytes[4] | (uint16_t)message->bytes[5] << 8;
    uint16_t yf = (uint16_t)message->bytes[6] | (uint16_t)message->bytes[7] << 8;
    uint32_t color = (uint32_t)message->bytes[8] | (uint32_t)message->bytes[9] << 8 | (uint32_t)message->bytes[10] << 16; 
    uint8_t thickness = message->bytes[11];
  //printf("receive xi = %d \n", xi);
  //printf("receive xf = %d \n", xf);
  //printf("receive yi = %d \n", yi);
  //printf("receive yf = %d \n", yf);
  //printf("receive color = %d \n", color);
  //printf("receive thickness = %d \n", thickness);
    canvas_draw_line1(xi, yi, xf, yf, color, thickness);
}

/*

struct UartMsgNode {
	UARTMessage msg;
	struct Node *next;
};
 
typedef struct {
	struct UartMsgNode *front;
	struct UartMsgNode *last;
	unsigned int size;
} UARTMessageQueue; 
 
UARTMessageQueue* queue_create();
uint8_t queue_front(UARTMessageQueue *q);
void queue_pop(UARTMessageQueue *q); 
void queue_push(UARTMessageQueue *q, UARTMessage msg);
*/
