#pragma once
#include "keyboard.h"
#include "i8042.h"

#define IRQ_SET_MOUSE   4

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
int mouse_subscribe_int(uint8_t *bit_no);
int mouse_unsubscribe_int();
int mouse_enable_int();
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
int mouse_set_stream();
int mouse_set_remote();
int mouse_enable_data_report();
int mouse_disable_data_report();
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

/**
 * @brief Receives a packet and returns any mouse event that might have happened.
 * 
 * @param pp Packet to be analyzed
 * @return Respective mouse event
 */
struct mouse_ev mouse_detect_ev(struct packet *pp);

/**
 * @brief Prints, in a human friendly way, a mouse event.
 * 
 * @param event Address of memory which stores the event structure
 */
void mouse_print_evt(struct mouse_ev* event);
