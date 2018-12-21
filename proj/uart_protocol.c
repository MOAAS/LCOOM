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

void free_messages(UARTMessage msgs[], uint16_t num_msgs) {
    for (uint16_t i = 0; i < num_msgs; i++) {
        free(msgs[i].bytes);
    }
}


void uart_send_message(UARTMessage* msg) {
    uint8_t size_id = (msg->size << 4) | msg->type;
    uart_fifo_send(MSG_PREFIX);
    uart_fifo_send(size_id);
    for (int i = 0; i < msg->size; i++) {
        uart_fifo_send(msg->bytes[i]);
    }
    uart_fifo_send(MSG_TRAILER);
    destroy_message(msg);
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
            new_message.bytes = malloc(new_message.size*sizeof(uint8_t));
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

void uart_send_draw_line1(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness) {
    uint8_t bytes[MSG_DRAW_LINE1_SIZE]; // size = 12;
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
    UARTMessage* message = create_message(MSG_DRAW_LINE1_SIZE, MSG_DRAW_LINE1, bytes);
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

void uart_process_msg(UARTMessage* message) {
    switch (message->type) {
        case MSG_DRAW_LINE1: {
            uint16_t xi = (uint16_t)message->bytes[0] | (uint16_t)message->bytes[1] << 8;
            uint16_t yi = (uint16_t)message->bytes[2] | (uint16_t)message->bytes[3] << 8;
            uint16_t xf = (uint16_t)message->bytes[4] | (uint16_t)message->bytes[5] << 8;
            uint16_t yf = (uint16_t)message->bytes[6] | (uint16_t)message->bytes[7] << 8;
            uint32_t color = (uint32_t)message->bytes[8] | (uint32_t)message->bytes[9] << 8 | (uint32_t)message->bytes[10] << 16; 
            uint8_t thickness = message->bytes[11];
            canvas_draw_line1(xi, yi, xf, yf, color, thickness);
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
            tick_clock();
            break;
        default: break;
    }
}

void uart_process_msgs(UARTMessage messages[], uint16_t num_messages) {
    for (uint16_t i = 0; i < num_messages; i++) {
        uart_process_msg(&messages[i]);
    }
}

UARTMessage* get_msg_by_id(uint8_t id, UARTMessage messages[], uint16_t num_messages){
    for (uint16_t i = 0; i < num_messages; i++) {
        if (messages[i].type == id)
            return &messages[i];
    }
    return NULL;
}
