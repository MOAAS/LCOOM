#pragma once
#include "keyboard.h"
#include "i8042.h"

/** @defgroup mouse mouse
 * @{
 *
 * Functions and constants used to program the mouse and its controller, the KBC.
 */


#define IRQ_SET_MOUSE   12

#define MOUSE_LB        BIT(0)
#define MOUSE_RB        BIT(1)
#define MOUSE_MB        BIT(2)
#define MOUSE_XSIGN     BIT(4)
#define MOUSE_YSIGN     BIT(5)
#define MOUSE_X_OVF     BIT(6)
#define MOUSE_Y_OVF     BIT(7)

#define SIGNED_DELTA    0xFF00
#define UNSIGNED_DELTA  0x0000

// Interruption enablers/disablers. They return 0 upon success and non-zero otherwise 

/**
 * @brief Subscribes mouse interrupts in exclusive mode.
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes mouse interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe_int();

/**
 * @brief Enables mouse interrupts.
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_enable_int();

/**
 * @brief Disables mouse interrupts
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_disable_int();


/**
 * @brief Writes a byte to the mouse directly, by sending the appropriate command to the KBC first.
 * 
 * @param byte Byte to be sent to the mouse
 * @param hasArgument Set to True if the command has an argument to be sent as well
 * @param arg The argument to the command (Ignored if hasArgument is set to false)
 * @return Return 0 upon success and non-zero otherwise 
 */
int mouse_write_cmd(uint8_t byte, bool hasArgument, uint8_t arg);

/**
 * @brief Polls the OUT_BUF until it gets a valid ACK byte (ACK, NACK or ERROR)
 * 
 * @param ack_byte Address of memory which will store the ACK Byte
 * @return Return 0 upon success and non-zero otherwise 
 */
int mouse_get_ack(uint8_t* ack_byte);

// Wrapper functions that call mouse_write_cmd() with the appropriate byte. They return whatever mouse_write_cmd() returns.

/**
 * @brief Sets stream mode, by calling mouse_write_cmd() with the appropriate byte.
 * 
 * @return Returns whatever mouse_write_cmd() returns (0 upon success and non-zero otherwise) 
 */
int mouse_set_stream();

/**
 * @brief Sets remote mode, by calling mouse_write_cmd() with the appropriate byte.
 * 
 * @return Returns whatever mouse_write_cmd() returns (0 upon success and non-zero otherwise) 
 */
int mouse_set_remote();

/**
 * @brief Enables data reporting, by calling mouse_write_cmd() with the appropriate byte.
 * 
 * @return Returns whatever mouse_write_cmd() returns (0 upon success and non-zero otherwise) 
 */
int mouse_enable_data_report();

/**
 * @brief Disables data reporting, by calling mouse_write_cmd() with the appropriate byte.
 * 
 * @return Returns whatever mouse_write_cmd() returns (0 upon success and non-zero otherwise) 
 */
int mouse_disable_data_report();

/**
 * @brief Polls the mouse, by calling mouse_write_cmd() with the appropriate byte.
 * 
 * @return Returns whatever mouse_write_cmd() returns (0 upon success and non-zero otherwise) 
 */
int mouse_read_data();

/**
 * @brief Assembles the global struct variable mouse_packet, by using another global variable packet_byte. 
 * Usually called after a packet_byte has been read.
 * 
 * @return int8_t Returns the byte_no that was just read (0, 1 or 2)
 */
int8_t mouse_assemble_packet();

/**
 * @brief Tests a possible desync on the packet aligning, by checking BIT 3 if we're dealing with the byte no. 0
 * 
 * @param packet_byte Packet to be analyzed
 * @param byte_no Number of the packet (0, 1 or 2)
 * @return true byte_no is different from 0, or BIT 3 is set to 1
 * @return false Otherwise
 */
bool mouse_is_packet_synced(uint8_t packet_byte, uint8_t byte_no);

