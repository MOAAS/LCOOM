#include <lcom/lcf.h>
#include <stdint.h>
#include "uart.h"

static int hook_id_uart = COM1_IRQ;

int uart_subscribe_int(uint8_t *bit_no) {
    *bit_no = hook_id_uart = COM1_IRQ;
    if (sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , &hook_id_uart) != OK)
        return 1;
    return 0;
}

int uart_unsubscribe_int() {
    return sys_irqrmpolicy(&hook_id_uart);
}

int uart_read_reg(uint32_t port, uint8_t* byte) {
    uint32_t temp;
    if (sys_inb(port, &temp) != OK)
        return 1;
    *byte = temp;
    return 0;    
}

int uart_write_reg(uint32_t port, uint8_t byte) {
    return sys_outb(port, (uint32_t)byte);
}



int uart_read_status(uint8_t* status) {
    return uart_read_reg(UART_LSR, status);
}

int uart_read_data(uint8_t* data) {    
    return uart_read_reg(UART_RBR, data);
}

int uart_send_data(uint8_t data) {    
    return uart_write_reg(UART_THR, data);
}


int uart_set_dlab(bool bit) {
    uint8_t ctrl_byte;
    if (uart_read_reg(UART_LCR, &ctrl_byte) != OK)
        return 1;
    if (bit) ctrl_byte |= UART_DLAB;
    else ctrl_byte &= ~UART_DLAB;
    ctrl_byte = ctrl_byte | BIT(0) | BIT(1) | BIT(2);
    uart_write_reg(UART_LCR, ctrl_byte);
    return 0;
}


int uart_receive_poll(uint8_t* data) {
    uint8_t status;
    if (uart_read_status(&status) != 0)
        return 1;
    while((status & UART_RECEIVER_DATA) == 0) {
        util_delay(2);
        if (uart_read_status(&status) != 0)
            return 1;
    }
    return uart_read_data(data) || status & UART_COM_ERR;
}

int uart_transmit_poll(uint8_t data) {
    uint8_t status;
    if (uart_read_status(&status) != 0)
        return 1;
    while ((status & UART_THR_EMPTY) == 0) {
        util_delay(2);
        if (uart_read_status(&status) != 0)
            return 1;
    }
    return uart_send_data(data);
}



int uart_enable_ier(uint8_t bit_no) {
    uint8_t ier;
    uart_read_reg(UART_IER, &ier);
    uart_write_reg(UART_IER, ier | bit_no);
    return 0;
}

int uart_disable_ier(uint8_t bit_no) {
    uint8_t ier;
    uart_read_reg(UART_IER, &ier);
    uart_write_reg(UART_IER, ier & ~bit_no);
    return 0;
}

int uart_get_int_id(uint8_t* id) {
    return uart_read_reg(UART_IIR, id);
}

UART_Int_Info uart_int_info;
bool uart_conflict = false;

void uart_setup_fifo() {
    uart_enable_fifo();
    uart_int_info.toSend = queue_create(1000);
    uart_int_info.received = queue_create(1000);
    uart_int_info.last_int_type = Transmitted;
}

void uart_destroy_fifo() {
    uart_disable_fifo();
    queue_destroy(uart_int_info.toSend);
    queue_destroy(uart_int_info.received);
}

void uart_ih() {
    uint8_t int_id;
    uart_get_int_id(&int_id);
    if (int_id & UART_NO_INT)
        return;
    uart_process_int(int_id);
}

void uart_process_int(uint8_t int_id) {
    uint8_t status;
    uint8_t data;
    uart_read_status(&status);
    switch(int_id & UART_INT_ID) {
        case UART_INT_RECEIVE: 
        case UART_INT_CHAR_TO:
           // printf("Receive interrupt (status = %d)\n", status);
            while (status & UART_RECEIVER_DATA) {
                uart_read_data(&data);
                if (status & UART_COM_ERR)
                    printf("Receiver error!");
                queue_push(uart_int_info.received, data);
                uart_read_status(&status);
            }
            uart_conflict = false;
            uart_int_info.last_int_type = Received;
            break; 
        case UART_INT_TRANSMIT:
            //printf("Transmit interrupt: Queue size = %d \n", uart_int_info.toSend->size);
            uart_clear_toSend_queue();
            uart_int_info.last_int_type = Transmitted;
            break; 
        case UART_INT_ERR:
            uart_int_info.last_int_type = Error;
            printf("Int Error! \n");
            while (status & UART_RECEIVER_DATA) {
                uart_read_data(&data);
                uart_read_status(&status);
            }
            break; /* notify upper level */
        default: printf("Unrecognized int: %d \n", int_id & UART_INT_ID); break;
    }
}

int uart_fifo_send(uint8_t data) {
    queue_push(uart_int_info.toSend, data);
    uint8_t status;
    uint8_t byte;
    uart_read_status(&status);

    // ultra test
    while (status & UART_RECEIVER_DATA) {
        uart_read_data(&byte);
        if (status & UART_COM_ERR)
            printf("Receiver error (send) : %d!", status);
        queue_push(uart_int_info.received, byte);
        uart_read_status(&status);
        uart_conflict = true;
    }
    if (uart_conflict)
        printf("Uart conflict! \n");
        
            
    if (status & UART_THR_EMPTY)
        uart_clear_toSend_queue();
    if (status & UART_COM_ERR) {
        printf("error status = %d \n", status);
    }
    return 0;
}

int uart_clear_toSend_queue() {
    for (int i = 0; i < 16 && uart_int_info.toSend->size != 0; i++) {
        if (uart_send_data(queue_front(uart_int_info.toSend)) != 0)
            printf("Send data error.\n");
        queue_pop(uart_int_info.toSend);
    }
    return 0;
}

int uart_enable_fifo() {
    //fcr = (fcr & ~BIT(6)) | BIT(7); // trigger level 8
    uart_write_reg(UART_FCR, UART_FIFO_ENABLE | UART_FIFO64_ENABLE | UART_FIFO_CLR_RCVR | UART_FIFO_CLR_XMIT);
    return 0;
}

int uart_disable_fifo() {
    uint8_t status;
    uart_read_status(&status);
    while (status & UART_RECEIVER_DATA) {
        uart_read_data(&status);
        printf("GOT SOMETHING! \n");
        util_delay(25);
        uart_read_status(&status);
    }
    return uart_write_reg(UART_FCR, UART_FIFO_CLR_XMIT | UART_FIFO_CLR_RCVR);
}

void uart_reset_fifo() {
    uart_destroy_fifo();
    uart_setup_fifo();
}
