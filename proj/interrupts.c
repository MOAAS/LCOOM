#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "interrupts.h"

extern uint16_t scancode;
extern struct packet mouse_packet;

int subscribe_device(Device device) {
    uint8_t irq_set;
    switch(device) {
        case Timer: return timer_subscribe_int(&irq_set);
        case Keyboard: return keyboard_subscribe_int(&irq_set);
        case Mouse:
            // Eventualmente por a verificar return values! (n necessario p/ lab5)
            mouse_subscribe_int(&irq_set);
            mouse_disable_int();
            mouse_set_stream();
            mouse_enable_data_report();
            mouse_enable_int();
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

        default: return 1;
    }
}

Notification GetNotification() {
    bool gotNotification = false;
    Notification notif;
    notif.timerNotif = notif.keyboardNotif = notif.mouseNotif = false;
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
                if (msg.m_notify.interrupts & BIT(IRQ_SET_TIMER)) { // subscribed interrupt
                  timer_int_handler();
                  notif.timer_counter = timer_get_counter();
                  notif.seconds_passed = timer_get_counter() / 60;
                  notif.timerNotif = true;
                  gotNotification = true;
                }
                if (msg.m_notify.interrupts & BIT(IRQ_SET_KEYBOARD)) { // subscribed interrupt
                    kbc_ih();
                    if (kbd_assemble_scancode()) {
                        notif.scancode = scancode;
                        notif.keyboardNotif = true;
                        gotNotification = true;
                    }
                }
                if (msg.m_notify.interrupts & BIT(IRQ_SET_MOUSE)) { // subscribed interrupt
                    mouse_ih();
                    int8_t byte_read_no = mouse_assemble_packet();
                    if (byte_read_no == 2) {
                        notif.mouse_packet = mouse_packet;
                        notif.mouseNotif = true;
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
