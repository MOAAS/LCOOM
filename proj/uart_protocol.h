#pragma once

#include "uart.h"

#define MSG_PREFIX          137
#define MSG_TRAILER         138

typedef struct {
    uint8_t size;       ///< Message size
    uint8_t type;       ///< Message ID
    uint8_t bytes[20];  ///< Message content
} UARTMessage;


/**
 * @brief Creates a message object.
 * 
 * @param size Message size
 * @param type Message ID
 * @param bytes Message content
 * @return UARTMessage* Returns created the message
 */
UARTMessage* create_message(uint8_t size, uint8_t type, uint8_t* bytes);

/**
 * @brief Destroys a message, freeing its space
 * 
 * @param msg Msg to be destroyed
 */
void destroy_message(UARTMessage* msg);

/**
 * @brief Sends a message through the UART. 
 * Format: 
 * Byte 0 - 137
 * Byte 1 - [size (4) - id (4)]
 * Bytes 2:n-1 - Content
 * Byte n - 138
 * 
 * @param type Message type
 * @param size Size of the bytes array.
 * @param bytes Message content.
 */
void uart_send_message(uint8_t type, uint8_t size, uint8_t* bytes);

/**
 * @brief Sends a message of size 0.
 * 
 * @param id Id of the message.
 */
void uart_send_empty_msg(uint8_t id);


/**
 * @brief Reads a received byte and starts to compose a message. Should be called every time a byte is received
 * 
 * @param byte Byte to proccess
 * @param msg_ptr Address which will return the message, when it's fully assembled
 * @return true The message was fully processed
 * @return false The message was not fully processed
 */
bool uart_assemble_received_message(uint8_t byte, UARTMessage* msg_ptr);

/**
 * @brief Gets a message from a message array
 * 
 * @param id Message id
 * @param messages Array
 * @param num_messages Number of messages
 * @return UARTMessage* Pointer to first message with the specified ID. NULL if none exist.
 */
UARTMessage* get_msg_by_id(uint8_t id, UARTMessage messages[], uint16_t num_messages);
