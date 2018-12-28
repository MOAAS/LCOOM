#pragma once

#include <stdio.h>
#include <ctype.h>

#include "uart_protocol.h"
#include "interrupts.h"
#include "i8042.h"

// Se forem preciso mais de alguma coisa poe-se!

typedef enum {
    LB_PRESS,   ///< Left button was pressed, and it previously wasn't  
    RB_PRESS,   ///< Right button was pressed, and it previously wasn't  
    MB_PRESS,   ///< Middle button was pressed, and it previously wasn't  
    LB_RELEASE, ///< Left button was released, and it previously wasn't  
    RB_RELEASE, ///< Right button was released, and it previously wasn't  
    MB_RELEASE, ///< Middle button was released, and it previously wasn't  
    MOUSE_MOVE, ///< No buttons were pressed, and the mouse reported movement.
    BUTTON_EVT  ///< Any other button event.
} MouseEventType;

typedef struct {
    MouseEventType type; ///< Type of mouse event.
    int16_t delta_x;     ///< X movement (Left is negative)
    int16_t delta_y;     ///< Y movement (Up is positive)
} MouseEvent;

typedef enum {
    CTRL_PRESS,
    CTRL_RELEASE,
    LSHIFT_PRESS,
    LSHIFT_RELEASE,
    RSHIFT_PRESS,
    RSHIFT_RELEASE,
    ESC_PRESS,
    BACKSPACE_PRESS,
    SPACEBAR_RELEASE,
    ENTER_PRESS,
    ENTER_RELEASE,
    CHARACTER_PRESS,
    ARROW_UP_PRESS,
    ARROW_UP_RELEASE,
    ARROW_LEFT_PRESS,
    ARROW_LEFT_RELEASE,
    ARROW_DOWN_PRESS,
    ARROW_DOWN_RELEASE,
    ARROW_RIGHT_PRESS,
    ARROW_RIGHT_RELEASE,
    OTHER_KEY
} KeyboardEventType;

typedef struct {
    KeyboardEventType type;     ///< Type of keyboard event.
    unsigned char character;    ///< If the event was a character press, holds the pressed character (takes into account shift pressing for upper case)
} KeyboardEvent;

typedef struct {
    uint32_t timer_counter;     ///< Timer auxiliary counter
    uint32_t seconds_passed;    ///< Number of seconds passed since the last timer reset
    bool has_second_passed;     ///< Set to true if the aux counter is a multiple of the timer's frequency
} TimerEvent;


typedef struct {
    bool isTimerEvent;              ///< Set to true iff there was a timer interrupt
    bool isRtcEvent;                ///< Set to true iff there was a RTC interrupt
    bool isKeyboardEvent;           ///< Set to true iff there was a KBD interrupt
    bool isMouseEvent;              ///< Set to true iff there was a mouse interrupt
    bool isUARTEvent;               ///< Set to true iff there was a UART interrupt
    KeyboardEvent keyboardEvent;    ///< KEYBOARD: Keyboard interrupt information
    MouseEvent mouseEvent;          ///< MOUSE: KeybMouseord interrupt information
    TimerEvent timerEvent;          ///< TIMER: Timer interrupt information
    UARTMessage uart_messages[50];  ///< UART: Array containing the unprocessed serial port message
    uint8_t num_uart_messages;      ///< UART: Array size
    bool isCtrlPressed;             ///< Set to true iff Left Ctrl is pressed
    bool isLShiftPressed;           ///< Set to true iff Left Shift is pressed
    bool isRShiftPressed;           ///< Set to true iff Right Shift is pressed    
    bool isLBPressed;               ///< Set to true iff LB is pressed
    bool isMBPressed;               ///< Set to true iff MB is pressed
    bool isRBPressed;               ///< Set to true iff RB is pressed    
} Event_t;

/**
 * @brief Waits until a notification is received, processes it to an Event_t struct.
 * For the UART:
 * - processes all the received bytes and assembles them into messages.
 * - Will only generate event if the interrupt type is "receiver data"
 * 
 * @return Notification Struct containing all the information of the notification.
 */
Event_t GetEvent();

/**
 * @brief Processes a mouse packet.
 * 
 * @param pp Mouse packet
 * @return MouseEvent Returns the corresponding mouse event.
 */
MouseEvent mouse_detect_ev(struct packet *pp);

/**
 * @brief Processes a keyboard scancode.
 * 
 * @param scancode Scancode
 * @return KeyboardEvent Returns the corresponding keyboard event.
 */
KeyboardEvent kbd_detect_ev(uint16_t scancode);

/**
 * @brief Prints on the console the event name (for debug).
 * 
 * @param event Event whose name will be printed.
 */
void print_event(Event_t event);

/**
 * @brief Waits. (Timer interrupts must be subscribed)
 * 
 * @param ms Time, in milisseconds, to wait for.
 */
void wait_ms(uint16_t ms);
