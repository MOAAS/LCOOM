#pragma once

#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "uart.h"
#include "i8254.h"

typedef struct Notification {
    uint32_t timer_counter;
    uint32_t seconds_passed;
    uint16_t scancode;
    struct packet mouse_packet;
    UART_Int_Info uart_int_info;
    bool timerNotif;
    bool keyboardNotif;
    bool mouseNotif;
    bool serialPortNotif;
} Notification;

typedef enum Device {
    Timer,
    Keyboard,
    Mouse,
    SerialPort
} Device;

Notification GetNotification();
int subscribe_device(Device device);
int unsubscribe_device(Device device);
