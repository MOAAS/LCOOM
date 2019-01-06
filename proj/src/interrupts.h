#pragma once

#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "uart.h"
#include "rtc.h"
#include "video.h"
#include "i8254.h"

/** @defgroup interrupts interrupts
 * @{
 *
 * Functions used to receive and process interrupt notifications.
 */


/**
 * @brief Represents a received interrupt notification.
 * 
 */
typedef struct Notification {
    uint32_t timer_counter;         ///< TIMER: Timer's auxiliary counter
    uint32_t seconds_passed;        ///< TIMER: Seconds passed since the last timer reset
    uint16_t scancode;              ///< KBD: Received keyboard scancode
    struct packet mouse_packet;     ///< MOUSE: Mouse packet received
    UART_Int_Info uart_int_info;    ///< UART: Information about the interrupt, containing queues for received and toSend bytes.
    bool timerNotif;                ///< Set to true iff there was a timer interrupt
    bool keyboardNotif;             ///< Set to true iff there was a kbd interrupt
    bool mouseNotif;                ///< Set to true iff there was a mouse interrupt
    bool serialPortNotif;           ///< Set to true iff there was a serial port interrupt
    bool rtcAlarmNotif;             ///< Set to true iff there was a RTC interrupt
} Notification;

typedef enum Device {
    Timer,
    Keyboard,
    Mouse,
    SerialPort,
    RTC
} Device;

/**
 * @brief Waits until an interrupt is received, processes it to a struct.
 * 
 * @return Notification Struct containing all the information of the interrupt.
 */
Notification GetNotification();

/**
 * @brief Subscribes a device's interrupts.
 * For the mouse, enables data reporting in stream mode. 
 * For the UART, enables the 64 Byte FIFO and all the interrupt types (apart from modem status)
 * For the RTC, enables an alarm for every second.
 * 
 * @param device Device to be subscribed. 
 * @return int 0 upon success, non-zero otherwise.
 */
int subscribe_device(Device device);

/**
 * @brief Unsubscribes a device's interrupts
 * For the mouse, disables data reporting in stream mode. 
 * For the UART, disables the FIFO and all the interrupt types (apart from modem status)
 * For the RTC, enables an alarm for every second.
 * 
 * @param device Device to be unsubscribed. 
 * @return int 0 upon success, non-zero otherwise.
 */
int unsubscribe_device(Device device);
