#pragma once

#include "uart_protocol.h"
#include "canvas.h"
#include "sprite.h"
#include "wordpicker.h"
#include "bitmaps.h"
#include "emote.h"

#define MSG_DRAW_LINE       0
#define MSG_DRAW_LINE2      1
#define MSG_TRASH           2
#define MSG_BUCKET          3
#define MSG_DRAW_SHAPE      4

#define MSG_DRAWER_READY    5
#define MSG_GUESSER_READY   6

#define MSG_ROUND_WON       7
#define MSG_ROUND_LOST      8

#define MSG_TICK_CLOCK      9
#define MSG_EMOTE           10
#define MSG_NUMBER          11

#define MSG_COLLAB_READY    12


#define MSG_DRAW_LINE_SIZE  12
#define MSG_DRAW_LINE2_SIZE 12
#define MSG_BUCKET_SIZE     7
#define MSG_TICK_CLOCK_SIZE 1
#define MSG_EMOTE_SIZE      1
#define MSG_NUMBER_SIZE     4
#define MSG_DRAW_SHAPE_SIZE 13
/**
 * @brief Structure containing possible contents for the UART Messages. The used elements are "indicated" by the type member
 * 
 */
typedef struct {
    uint8_t type;            ///< Message ID
    uint16_t xi, yi, xf, yf; ///< 2 pairs of cursor positions
    uint32_t color;          ///< Drawing color
    uint8_t thickness;       ///< Pencil thickness
    uint8_t time_left;       ///< Time left in the game clock
    Shape shape;             ///< Shape to draw
    Emote emote;             ///< Emote to draw
    int number;              ///< Received number
} UARTMessageContents;

/**
 * @brief Sends a message through the Serial Port, indicating the receiver to draw a line.
 * 
 * @param xi X position where the line starts
 * @param yi Y position where the line starts
 * @param xf X position where the line ends
 * @param yf Y position where the line ends
 * @param color Color of the line
 * @param thickness Line thickness
 */
void uart_send_draw_line(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness);

/**
 * @brief Sends a message through the Serial Port, indicating the receiver to draw a line, using a Square pattern.
 * 
 * @param xi X position where the line starts
 * @param yi Y position where the line starts
 * @param xf X position where the line ends
 * @param yf Y position where the line ends
 * @param color Color of the line
 * @param thickness Line thickness
 */
void uart_send_draw_line2(uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, uint32_t color, uint8_t thickness);

/**
 * @brief Sends a message through the Serial Port, indicating the receiver to draw a shape, receiving two mouse clicks only.
 * 
 * @param shape Shape to draw
 * @param click1_x X position of the 1st mouse click
 * @param click1_y Y position of the 1st mouse click
 * @param click2_x X position of the 2nd mouse click
 * @param click2_y Y position of the 2nd mouse click
 * @param color Color of the shape
 * @param thickness Shape thickness (if applied)
 */
void uart_send_draw_shape(Shape shape, uint16_t click1_x, uint16_t click1_y, uint16_t click2_x, uint16_t click2_y, uint32_t color, uint16_t thickness);

/**
 * @brief Sends a message through the Serial Port, indicating the receiver to erase the canvas.
 * 
 */
void uart_send_trash();

/**
 * @brief Sends a message through the Serial Port, indicating the receiver to flood fill an area.
 * 
 * @param xi X position of the starting point
 * @param yi Y position of the starting point
 * @param color Bucket color
 */
void uart_send_bucket(uint16_t xi, uint16_t yi, uint32_t color);

/**
 * @brief Sends a message through the UART, indicating the receiver that the sender is drawing this round and it is ready to start * 
 * Also sends the round solution.
 * 
 * @param solution Solution for the round
 */
void uart_send_drawer_ready(char* solution);

/**
 * @brief Sends a message through the UART, indicating the receiver to drawn an emote on the screen.
 * 
 * @param emote Emote to draw.
 */
void uart_send_emote(Emote emote);

/**
 * @brief Sends a 4 byte signed integer through the UART.
 * 
 * @param number Number to send.
 */
void uart_send_number(int number);

/**
 * @brief Sends a message through the UART, indicating the receiver to change the time left in their game clock.
 * 
 * @param time_left Time left in the clock.
 */
void uart_send_tick_clock(uint8_t time_left);


/**
 * @brief Processes a list of messages, doing what they order.
 * 
 * @param messages Array of messages to process
 * @param num_messages Size of the message array
 */
void uart_process_msgs(UARTMessage messages[], uint16_t num_messages);

/**
 * @brief Processes a message, not doing what they order.
 * 
 * @param message Message to process
 * @return UARTMessageContents Returns the contents of the message in a structure.
 */
UARTMessageContents uart_process_msg(UARTMessage* message);


