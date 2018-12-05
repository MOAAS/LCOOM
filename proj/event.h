#pragma once

#include <stdio.h>
#include <ctype.h>

#include "interrupts.h"
#include "i8042.h"

// Se forem preciso mais de alguma coisa poe-se!

typedef enum {
    LB_PRESS,
    RB_PRESS,
    MB_PRESS,
    LB_RELEASE,
    RB_RELEASE,
    MB_RELEASE,
    BUTTON_EVT,
    MOUSE_MOVE
} MouseEventType;

typedef struct {
    MouseEventType type;
    int16_t delta_x;
    int16_t delta_y;
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
    ENTER_PRESS,
    CHARACTER_PRESS,
    OTHER_KEY
} KeyboardEventType;

typedef struct {
    KeyboardEventType type;
    uint16_t scancode; // talvez n preciso
    unsigned char character;
} KeyboardEvent;

typedef struct {
    uint32_t timer_counter;
    uint32_t seconds_passed;
    bool has_second_passed;
} TimerEvent;

typedef struct {
    bool isKeyboardEvent;
    bool isMouseEvent;
    KeyboardEvent keyboardEvent;
    MouseEvent mouseEvent;
    TimerEvent timerEvent;
    bool isCtrlPressed;
    bool isLShiftPressed;
    bool isRShiftPressed;
    bool isLBPressed;
    bool isMBPressed;
    bool isRBPressed;
    bool isTimerEvent;
} Event_t;

Event_t GetEvent();
MouseEvent mouse_detect_ev(struct packet *pp);
KeyboardEvent kbd_detect_ev(uint16_t scancode);
void print_event(Event_t event);
