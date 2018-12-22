#include <lcom/lcf.h>
#include <stdint.h>

#include "event.h"

static bool is_CTRL_PRESSED = false;
static bool is_LSHIFT_PRESSED = false;
static bool is_RSHIFT_PRESSED = false;

static bool is_LB_PRESSED = false;
static bool is_RB_PRESSED = false;
static bool is_MB_PRESSED = false;

Event_t GetEvent() {
    Event_t event;
    Notification notification = GetNotification();
    if (notification.keyboardNotif) {
        event.isKeyboardEvent = true;
        event.keyboardEvent = kbd_detect_ev(notification.scancode);
    }
    else event.isKeyboardEvent = false;
    if (notification.mouseNotif) {
        event.isMouseEvent = true;
        event.mouseEvent = mouse_detect_ev(&notification.mouse_packet);
    }
    else event.isMouseEvent = false;
    if (notification.timerNotif) {
        event.isTimerEvent = true;
        event.timerEvent.timer_counter = notification.timer_counter;
        event.timerEvent.has_second_passed = notification.timer_counter % 60 == 0;
        event.timerEvent.seconds_passed = notification.seconds_passed;
    }
    else event.isTimerEvent = false;
    if (notification.serialPortNotif  && notification.uart_int_info.last_int_type == Received && notification.uart_int_info.received->size != 0) {
        event.isUARTEvent = false;
        event.num_uart_messages = 0;
        while(notification.uart_int_info.received->size != 0) {
            if (uart_assemble_received_message(queue_front(notification.uart_int_info.received), &event.uart_messages[event.num_uart_messages])) {
                event.num_uart_messages++;
                event.isUARTEvent = true;
            }
            queue_pop(notification.uart_int_info.received);
        }
    }
    else event.isUARTEvent = false;
    event.isCtrlPressed = is_CTRL_PRESSED;
    event.isLShiftPressed = is_LSHIFT_PRESSED;
    event.isRShiftPressed = is_RSHIFT_PRESSED;
    event.isLBPressed = is_LB_PRESSED;
    event.isMBPressed = is_MB_PRESSED;
    event.isRBPressed = is_RB_PRESSED;
    return event;
}

MouseEvent mouse_detect_ev(struct packet *pp) {
    MouseEvent event;
    // LB was pressed
    if (pp->lb != is_LB_PRESSED) {
        if (pp->lb && !is_LB_PRESSED) event.type = LB_PRESS;
        else event.type = LB_RELEASE;
        is_LB_PRESSED = pp->lb;
    }
    // RB was pressed
    else if (pp->rb != is_RB_PRESSED) {
        if (pp->rb && !is_RB_PRESSED) event.type = RB_PRESS;
        else event.type = RB_RELEASE;
        is_RB_PRESSED = pp->rb;
    }
    // MB was pressed
    else if (pp->mb != is_MB_PRESSED) {
        if (pp->mb && !is_MB_PRESSED) event.type = MB_PRESS;
        else event.type = MB_RELEASE;
        is_MB_PRESSED = pp->mb;
    }
    // Any mouse movement
    else if (pp->delta_x || pp->delta_y) {
        event.type = MOUSE_MOVE;
        event.delta_x = pp->delta_x;
        event.delta_y = pp->delta_y;
    }
    // Some other packet sending that is not supported, such as side mouse button pressing
    else event.type = BUTTON_EVT;
    return event;
}

KeyboardEvent kbd_detect_ev(uint16_t scancode) {
    KeyboardEvent event;
    switch (scancode) {
        case LCTRL_MAKE:
            event.type = CTRL_PRESS;
            is_CTRL_PRESSED = true;
            break;
        case LCTRL_BREAK:
            event.type = CTRL_RELEASE;
            is_CTRL_PRESSED = false;
            break;
        case LSHIFT_MAKE:
            event.type = LSHIFT_PRESS;
            is_LSHIFT_PRESSED = true;
            break;
        case LSHIFT_BREAK:
            event.type = LSHIFT_RELEASE;
            is_LSHIFT_PRESSED = false;
            break;
        case RSHIFT_MAKE:
            event.type = RSHIFT_PRESS;
            is_RSHIFT_PRESSED = true;
            break;
        case RSHIFT_BREAK:
            event.type = RSHIFT_RELEASE;
            is_RSHIFT_PRESSED = false;
            break;
        case ESC_MAKE: 
            event.type = ESC_PRESS; 
            break;
        case BACKSPACE_MAKE: 
            event.type = BACKSPACE_PRESS; 
            break;
        case ENTER_MAKE: 
            event.type = ENTER_PRESS; 
            break;
        case ENTER_BREAK: 
            event.type = ENTER_RELEASE; 
            break;          
        case ARROW_U_MAKE:
            event.type = ARROW_UP_PRESS;
            break;
        case ARROW_D_MAKE:
            event.type = ARROW_DOWN_PRESS;
            break;
        case ARROW_L_MAKE:
            event.type = ARROW_LEFT_PRESS;
            break;
        case ARROW_R_MAKE:
            event.type = ARROW_RIGHT_PRESS;
            break;
        case ARROW_U_BREAK:
            event.type = ARROW_UP_RELEASE;
            break;
        case ARROW_D_BREAK:
            event.type = ARROW_DOWN_RELEASE;
            break;
        case ARROW_L_BREAK:
            event.type = ARROW_LEFT_RELEASE;
            break;
        case ARROW_R_BREAK:
            event.type = ARROW_RIGHT_RELEASE;
            break;
        // letters
        case A_MAKE: event.character = 'a'; event.type = CHARACTER_PRESS; break;
        case B_MAKE: event.character = 'b'; event.type = CHARACTER_PRESS; break;
        case C_MAKE: event.character = 'c'; event.type = CHARACTER_PRESS; break;
        case D_MAKE: event.character = 'd'; event.type = CHARACTER_PRESS; break;
        case E_MAKE: event.character = 'e'; event.type = CHARACTER_PRESS; break;
        case F_MAKE: event.character = 'f'; event.type = CHARACTER_PRESS; break;
        case G_MAKE: event.character = 'g'; event.type = CHARACTER_PRESS; break;
        case H_MAKE: event.character = 'h'; event.type = CHARACTER_PRESS; break;
        case I_MAKE: event.character = 'i'; event.type = CHARACTER_PRESS; break;
        case J_MAKE: event.character = 'j'; event.type = CHARACTER_PRESS; break;
        case K_MAKE: event.character = 'k'; event.type = CHARACTER_PRESS; break;
        case L_MAKE: event.character = 'l'; event.type = CHARACTER_PRESS; break;
        case M_MAKE: event.character = 'm'; event.type = CHARACTER_PRESS; break;
        case N_MAKE: event.character = 'n'; event.type = CHARACTER_PRESS; break;
        case O_MAKE: event.character = 'o'; event.type = CHARACTER_PRESS; break;
        case P_MAKE: event.character = 'p'; event.type = CHARACTER_PRESS; break;
        case Q_MAKE: event.character = 'q'; event.type = CHARACTER_PRESS; break;
        case R_MAKE: event.character = 'r'; event.type = CHARACTER_PRESS; break;
        case S_MAKE: event.character = 's'; event.type = CHARACTER_PRESS; break;
        case T_MAKE: event.character = 't'; event.type = CHARACTER_PRESS; break;
        case U_MAKE: event.character = 'u'; event.type = CHARACTER_PRESS; break;
        case V_MAKE: event.character = 'v'; event.type = CHARACTER_PRESS; break;
        case W_MAKE: event.character = 'w'; event.type = CHARACTER_PRESS; break;
        case X_MAKE: event.character = 'x'; event.type = CHARACTER_PRESS; break;
        case Y_MAKE: event.character = 'y'; event.type = CHARACTER_PRESS; break;
        case Z_MAKE: event.character = 'z'; event.type = CHARACTER_PRESS; break;
        // punctuation
        case COMMA_MAKE: event.character = ','; event.type = CHARACTER_PRESS; break;
        case PERIOD_MAKE: event.character = '.'; event.type = CHARACTER_PRESS; break;
        case SPACE_MAKE: event.character = ' '; event.type = CHARACTER_PRESS; break;
        case KEY12_MAKE: event.character = '\''; event.type = CHARACTER_PRESS; break;
        case KEY55_MAKE: event.character = '-'; event.type = CHARACTER_PRESS; break;
        // numbers
        case NUM0_MAKE: event.character = '0'; event.type = CHARACTER_PRESS; break;
        case NUM1_MAKE: event.character = '1'; event.type = CHARACTER_PRESS; break;
        case NUM2_MAKE: event.character = '2'; event.type = CHARACTER_PRESS; break;
        case NUM3_MAKE: event.character = '3'; event.type = CHARACTER_PRESS; break;
        case NUM4_MAKE: event.character = '4'; event.type = CHARACTER_PRESS; break;
        case NUM5_MAKE: event.character = '5'; event.type = CHARACTER_PRESS; break;
        case NUM6_MAKE: event.character = '6'; event.type = CHARACTER_PRESS; break;
        case NUM7_MAKE: event.character = '7'; event.type = CHARACTER_PRESS; break;
        case NUM8_MAKE: event.character = '8'; event.type = CHARACTER_PRESS; break;
        case NUM9_MAKE: event.character = '9'; event.type = CHARACTER_PRESS; break;
        default: event.type = OTHER_KEY; break;
    }
    if (event.type == CHARACTER_PRESS && (is_LSHIFT_PRESSED || is_RSHIFT_PRESSED)) {
        switch (event.character) {
            case '0': event.character = '='; break;
            case '1': event.character = '!'; break;
            case '2': event.character = '"'; break;
            case '3': event.character = '#'; break;
            case '4': event.character = '$'; break;
            case '5': event.character = '%'; break;
            case '6': event.character = '&'; break;
            case '7': event.character = '/'; break;
            case '8': event.character = '('; break;
            case '9': event.character = ')'; break;
            case '\'': event.character = '?'; break;
            case ',': event.character = ';'; break;
            case '.': event.character = ':'; break;
            case '-': event.character = '_'; break;
            default: event.character = toupper(event.character); break;
        }
    }
    event.scancode = scancode;
    return event;
}

void print_event(Event_t event) {
    if (event.isMouseEvent) {
        switch(event.mouseEvent.type) {
            case LB_PRESS: printf("LB PRESSED \n"); break;
            case MB_PRESS: printf("MB PRESSED \n"); break;
            case RB_PRESS: printf("RB PRESSED \n"); break;
            case LB_RELEASE: printf("LB RELEASED \n"); break;
            case MB_RELEASE: printf("MB RELEASED \n"); break;
            case RB_RELEASE: printf("RB RELEASED \n"); break;
            case MOUSE_MOVE: printf("MOUSE MOV (%d, %d) \n", event.mouseEvent.delta_x, event.mouseEvent.delta_y); break;
            case BUTTON_EVT: printf("BUTTON EVT \n"); break;
            default: printf("Unrecognized mouse event! \n"); break;
        }
    }
    if (event.isKeyboardEvent) {
        switch(event.keyboardEvent.type) {
            case CTRL_PRESS: printf("CTRL PRESS\n"); break;
            case CTRL_RELEASE: printf("CTRL RELEASE \n"); break;
            case LSHIFT_PRESS: printf("LSHIFT PRESS \n"); break;
            case LSHIFT_RELEASE: printf("LSHIFT RELEASE \n"); break;
            case RSHIFT_PRESS: printf("RSHIFT PRESS\n"); break;
            case RSHIFT_RELEASE: printf("RSHIFT RELEASE\n"); break;
            case CHARACTER_PRESS: printf("CHARACTER PRESS: %c\n", event.keyboardEvent.character); break;
            case ESC_PRESS: printf("ESC PRESS \n"); break;
            case BACKSPACE_PRESS: printf("BSPACE PRESS \n"); break;
            case OTHER_KEY: printf("OTHER_KEY\n"); break;
            default: printf("Unrecognized kbd event! \n"); break;
        }
    }
}

