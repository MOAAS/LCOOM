#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "interrupts.h"

extern uint16_t scancode;
extern struct packet mouse_packet;
extern UART_Int_Info uart_int_info;
extern bool uart_conflict;
extern bool alarm_int;

int subscribe_device(Device device) {
    uint8_t irq_set;
    switch(device) {
        case Timer: return timer_subscribe_int(&irq_set);
        case Keyboard: return keyboard_subscribe_int(&irq_set);
        case Mouse:
            // Eventualmente por a verificar return values!
            mouse_subscribe_int(&irq_set);
            mouse_disable_int();
            mouse_set_stream();
            mouse_enable_data_report();
            mouse_enable_int();
            return 0;
        case SerialPort:
            uart_subscribe_int(&irq_set);
            uart_set_dlab(0);
            uart_enable_ier(UART_IER_RECEIVE | UART_IER_TRANSMIT | UART_IER_STATUS);
            uart_setup_fifo();
            return 0;
        case RTC:
            rtc_subscribe_int(&irq_set);
            enable_alarm_int_sec();
            return 0;
        default: return 1;
    }
}

int unsubscribe_device(Device device) {
    switch(device) {
        case Timer: return timer_unsubscribe_int();
        case Keyboard: return keyboard_unsubscribe_int();
        case Mouse: 
            // Eventualmente por a verificar return values! (n necessario p/ lab5)
            mouse_disable_int();
            mouse_disable_data_report();
            mouse_enable_int();
            mouse_unsubscribe_int();
            return 0;
        case SerialPort:
            uart_destroy_fifo();
            uart_unsubscribe_int();
            return 0;
        case RTC:
            disable_alarm_int();
            rtc_unsubscribe_int();
            return 0;
        default: return 1;
    }
}

Notification GetNotification() {
    bool gotNotification = false;
    Notification notif;
    notif.timerNotif = notif.keyboardNotif = notif.mouseNotif = notif.serialPortNotif = notif.rtcAlarmNotif = false;
    int ipc_status; message msg;
    while(!gotNotification) {
        int r; // Get a request message.
        if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) { // received notification
            switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE: // hardware interrupt notification                                
                if (msg.m_notify.interrupts & BIT(TIMER0_IRQ)) { // subscribed interrupt
                  timer_int_handler();
                  notif.timer_counter = timer_get_counter();
                  notif.seconds_passed = timer_get_counter() / 60;
                  notif.timerNotif = true;
                  gotNotification = true;
                }
                if (msg.m_notify.interrupts & BIT(KEYBOARD_IRQ)) { // subscribed interrupt
                    kbc_ih();
                    if (kbd_assemble_scancode()) {
                        notif.scancode = scancode;
                        notif.keyboardNotif = true;
                        gotNotification = true;
                    }
                }
                if (msg.m_notify.interrupts & BIT(MOUSE_IRQ)) { // subscribed interrupt
                    mouse_ih();
                    int8_t byte_read_no = mouse_assemble_packet();
                    if (byte_read_no == 2) {
                        notif.mouse_packet = mouse_packet;
                        notif.mouseNotif = true;
                        gotNotification = true;
                    }
                }
                if (msg.m_notify.interrupts & BIT(COM1_IRQ)) { // subscribed interrupt
                    uart_ih();
                    notif.serialPortNotif = true;
                    notif.uart_int_info = uart_int_info;
                    gotNotification = true;
                }
                if (uart_conflict) {
                    printf("UH OH There was a conflict! \n");
                    notif.serialPortNotif = true;
                    notif.uart_int_info = uart_int_info;
                    notif.uart_int_info.last_int_type = Received;
                    gotNotification = true;
                    uart_conflict = false;
                }
                if (msg.m_notify.interrupts & BIT(RTC_IRQ)) { // subscribed interrupt
                    rtc_ih();
                    if(alarm_int){
                        notif.rtcAlarmNotif = true;
                        gotNotification = true;
                    }
                }

                break;
            default:
              break; // no other notifications expected: do nothing
            }
        }
    }
    return notif;
}
