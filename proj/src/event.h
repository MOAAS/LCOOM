#pragma once

#include <stdio.h>
#include <ctype.h>

#include "uart_protocol.h"
#include "interrupts.h"
#include "i8042.h"

/** @defgroup event event
 * @{
 *
 * Functions used to receive and process events.
 */


/**
 * @brief Enumerates all the possible mouse events
 * 
 */
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

/**
 * @brief Represents a mouse event.
 * 
 */
typedef struct {
    MouseEventType type; ///< Type of mouse event.
    int16_t delta_x;     ///< X movement (Left is negative)
    int16_t delta_y;     ///< Y movement (Up is positive)
} MouseEvent;

/**
 * @brief Enumerates all the possible keyboard events
 * 
 */
typedef enum {
    CTRL_PRESS,         ///< Left Ctrl was pressed
    CTRL_RELEASE,       ///< Left Ctrl was released
    LSHIFT_PRESS,       ///< Left Shift was pressed
    LSHIFT_RELEASE,     ///< Left Shift was released
    RSHIFT_PRESS,       ///< Right Shift was pressed
    RSHIFT_RELEASE,     ///< Right Shift was released
    ESC_PRESS,          ///< Esc Key was pressed
    BACKSPACE_PRESS,    ///< Backspace Key was pressed
    SPACEBAR_RELEASE,   ///< Spacebar was released
    ENTER_PRESS,        ///< Enter Key was pressed
    ENTER_RELEASE,      ///< Enter Key was released
    CHARACTER_PRESS,    ///< A character was pressed (number/alphabet/punctuation/space)
    ARROW_UP_PRESS,     ///< Arrow Key Up was pressed
    ARROW_UP_RELEASE,   ///< Arrow Key Up was released
    ARROW_LEFT_PRESS,   ///< Arrow Key Left was pressed
    ARROW_LEFT_RELEASE, ///< Arrow Key Left was released
    ARROW_DOWN_PRESS,   ///< Arrow Key Down was pressed
    ARROW_DOWN_RELEASE, ///< Arrow Key Down was released
    ARROW_RIGHT_PRESS,  ///< Arrow Key Right was pressed
    ARROW_RIGHT_RELEASE,///< Arrow Key Right was released
    OTHER_KEY           ///< None of the above
} KeyboardEventType;

/**
 * @brief Represents a keyboard event.
 * 
 */
typedef struct {
    KeyboardEventType type;     ///< Type of keyboard event.
    unsigned char character;    ///< If the event was a character press, holds the pressed character (takes into account shift pressing for upper case)
    uint16_t scancode;          ///< Scancode that originated from the interrupt
} KeyboardEvent;

/**
 * @brief Represents a timer event.
 * 
 */
typedef struct {
    uint32_t timer_counter;     ///< Timer auxiliary counter
    uint32_t seconds_passed;    ///< Number of seconds passed since the last timer reset
    bool has_second_passed;     ///< Set to true if the aux counter is a multiple of the timer's frequency
} TimerEvent;

/**
 * @brief Represents an event.
 * 
 */
typedef struct {
    bool isTimerEvent;              ///< Set to true iff there was a timer interrupt/event
    bool isRtcEvent;                ///< Set to true iff there was a RTC interrupt/event
    bool isKeyboardEvent;           ///< Set to true iff there was a KBD interrupt/event
    bool isMouseEvent;              ///< Set to true iff there was a mouse interrupt/event
    bool isUARTEvent;               ///< Set to true iff there was a UART interrupt/event
    KeyboardEvent keyboardEvent;    ///< KEYBOARD: Keyboard interrupt information
    MouseEvent mouseEvent;          ///< MOUSE: Mouse interrupt information
    TimerEvent timerEvent;          ///< TIMER: Timer interrupt information
    UARTMessage uart_messages[5000];///< UART: Array containing the unprocessed serial port message
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
