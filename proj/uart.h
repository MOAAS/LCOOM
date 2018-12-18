#pragma once
#include "queue.h"
#include "keyboard.h"

#define BIT(n)              (0x01<<(n))

#define COM1_IRQ            4
#define COM2_IRQ            3

#define COM1_BASE           0x3F8
#define COM2_BASE           0x2F8

#define UART_RBR            0x3F8
#define UART_THR            0x3F8
#define UART_IER            0x3F9
#define UART_IIR            0x3FA
#define UART_FCR            0x3FA
#define UART_LCR            0x3FB
#define UART_MCR            0x3FC
#define UART_LSR            0x3FD
#define UART_MSR            0x3FE
#define UART_SR             0x3FF

#define UART_DLL            0x3F8
#define UART_DLM            0x3F8

#define UART_RECEIVER_DATA  BIT(0)
#define UART_OVERRUN_ERR    BIT(1)
#define UART_PARITY_ERR     BIT(2)
#define UART_FRAME_ERR      BIT(3)
#define UART_THR_EMPTY      BIT(5)
#define UART_TRANSMIT_EMPTY BIT(6)
#define UART_FIFO_ERR       BIT(7)

#define UART_DLAB           BIT(7)    

#define UART_IER_RECEIVE    BIT(0)
#define UART_IER_TRANSMIT   BIT(1)
#define UART_IER_STATUS     BIT(2)

#define UART_NO_INT         BIT(0)
#define UART_INT_ERR        BIT(1) | BIT(2)
#define UART_INT_RECEIVE    BIT(2)
#define UART_INT_CHAR_TO    BIT(2) | BIT(3)
#define UART_INT_TRANSMIT   BIT(1)
#define UART_INT_ID         ~(BIT(0) | BIT(4) | BIT(5) | BIT(6) | BIT(7))

#define UART_FIFO_ENABLE    BIT(0)
#define UART_FIFO_CLR_RCVR  BIT(1)
#define UART_FIFO_CLR_XMIT  BIT(2)
#define UART_FIFO64_ENABLE  BIT(5)

typedef enum {
    Received = 12,
    Transmitted = 31,
    Error = 5,
    CharTimeout = 57
} UART_IntType;

typedef struct {
    Queue* toSend;
    Queue* received;
    UART_IntType last_int_type;
} UART_Int_Info;


int uart_subscribe_int(uint8_t *bit_no);
int uart_unsubscribe_int();

int uart_read_reg(uint32_t port, uint8_t* byte);
int uart_write_reg(uint32_t port, uint8_t byte);

int uart_read_status(uint8_t* status);
int uart_write_status(uint8_t status);

int uart_read_data(uint8_t* data);
int uart_send_data(uint8_t data);


int uart_set_dlab(bool bit);

int uart_receive_poll(uint8_t* data);
int uart_transmit_poll(uint8_t data);

int uart_disable_ier(uint8_t bit_no);
int uart_enable_ier(uint8_t bit_no);

int uart_get_int_id(uint8_t* id);


void uart_ih();
int uart_fifo_send(uint8_t data);
int uart_clear_toSend_queue();

void uart_setup_fifo();
void uart_destroy_fifo();

int uart_enable_fifo();
int uart_disable_fifo();

bool uart_com_error(uint8_t status);
