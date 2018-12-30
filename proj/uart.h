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
#define UART_COM_ERR        (BIT(1) | BIT(2) | BIT(3))

#define UART_DLAB           BIT(7)    

#define UART_IER_RECEIVE    BIT(0)
#define UART_IER_TRANSMIT   BIT(1)
#define UART_IER_STATUS     BIT(2)

#define UART_NO_INT         BIT(0)
#define UART_INT_ERR        (BIT(1) | BIT(2))
#define UART_INT_RECEIVE    BIT(2)
#define UART_INT_CHAR_TO    (BIT(2) | BIT(3))
#define UART_INT_TRANSMIT   BIT(1)
#define UART_INT_ID         ~(BIT(0) | BIT(4) | BIT(5) | BIT(6) | BIT(7))

#define UART_FIFO_ENABLE    BIT(0)
#define UART_FIFO_CLR_RCVR  BIT(1)
#define UART_FIFO_CLR_XMIT  BIT(2)
#define UART_FIFO64_ENABLE  BIT(5)

typedef enum {
    Received,
    Transmitted,
    Error,
    CharTimeout
} UART_IntType;

typedef struct {
    Queue* toSend;
    Queue* received;
    UART_IntType last_int_type;
} UART_Int_Info;

/**
 * @brief Subscribes and enables UART interrupts, in exclusive mode.
 *
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes UART interrupts
 *
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_unsubscribe_int();

/**
 * @brief Reads a one byte register.
 * 
 * @param port Register port
 * @param byte Address of memory where the byte will be stored
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_read_reg(uint32_t port, uint8_t* byte);

/**
 * @brief Writes a byte to a register
 * 
 * @param port Register port
 * @param byte Byte to write
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_write_reg(uint32_t port, uint8_t byte);

/**
 * @brief Reads the UART status register
 * 
 * @param byte Address of memory where the status byte will be stored
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_read_status(uint8_t* status);

/**
 * @brief Reads a byte from the RBR
 * 
 * @param byte Address of memory where the byte will be stored
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_read_data(uint8_t* data);

/**
 * @brief Writes a byte to the THR
 * 
 * @param data Byte to write
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_send_data(uint8_t data);

/**
 * @brief Sets the DLAB bit.
 * 
 * @param bit True - set to 1. False - set to 0.
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_set_dlab(bool bit);

/**
 * @brief Polls the UART, until it receives a byte.
 * 
 * @param data Pointer to received byte
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_receive_poll(uint8_t* data);

/**
 * @brief Polls the UART, until it can transmit a byte.
 * 
 * @param data Byte to send
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_transmit_poll(uint8_t data);

/**
 * @brief Disables UART interrupts, by writing to the IER
 * 
 * @param bits Bits from the IER to be turned off.
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_disable_ier(uint8_t bits);

/**
 * @brief Enables UART interrupts, by writing to the IER
 * 
 * @param bits Bits  from the IER to be turned on
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_enable_ier(uint8_t bits);

/**
 * @brief Reads the IIR.
 * 
 * @param id Address that will store the interrupt id.
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_get_int_id(uint8_t* id);

/**
 * @brief UART Interrupt handler. Reads the IIR and processes its contents.
 */
void uart_ih();

/**
 * @brief Processes an interrupt, by checks the interrupt type. 
 * If it has received something, and there were no communication errors, adds all the bytes to a queue, by order of arrival.
 * If it is ready to send something, clears the toSend queue (16 byte limit).
 * If there was an error, discards the contents of the RBR (if there's any). 
 * @param int_id IIR contents.
 */
void uart_process_int(uint8_t int_id);


/**
 * @brief Pushes a byte to the toSend queue. If the THR is empty, also clears the toSend queue.
 * 
 * @param data Byte to be sent
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_fifo_send(uint8_t data);

/**
 * @brief Sends the first 16 bytes of the queue to the transmitting fifo.
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_clear_toSend_queue();

/**
 * @brief Enables the 64 Byte FIFO, creates the toSend and received queue.
 * 
 */
void uart_setup_fifo();

/**
 * @brief Disables the 64 Byte FIfo, destroys the toSend and received queues.
 * 
 */
void uart_destroy_fifo();

/**
 * @brief Enables and clears the 64 Byte fifo.
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_enable_fifo();

/**
 * @brief Disables and clears the 64 Byte fifo.
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int uart_disable_fifo();

/**
 * @brief Resets the fifo and clears it.
 * 
 */
void uart_reset_fifo();
