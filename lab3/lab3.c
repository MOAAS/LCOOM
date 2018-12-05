#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>

#include "keyboard.h"
#include "i8042.h"

#define FUNCTION_FAIL   1
#define KBD_COMM_ERROR  2
#define KBD_TIMEOUT     3

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern uint32_t sys_in_counter;
extern uint8_t scancode;
extern uint8_t kbc_ih_return;
uint8_t scancodes[2];
bool scancode_has_second_byte;
bool esc_key_released = false;

void (kbd_call_print_code)() {
  // This function is called after a scancode has been retrieved.
  // Pretty much a state machine with 2 states, it will either read the first byte or the second one.
  if (scancode_has_second_byte) {
    scancodes[1] = scancode;
    // is_make_code will be the complement of the MSB of the scancode
    // which means it will be 'true' if it's a make code
    bool is_make_code = (scancode >> 7) == 0;
    scancode_has_second_byte = false;
    if (kbd_print_scancode(is_make_code, 2, scancodes) != 0)
      printf("Function fail: kbd_print_scancode() \n"); 
  }
  else if (scancode == SCANCODE_PREFIX) {
    scancodes[0] = scancode;
    scancode_has_second_byte = true;
  }
  else {
    scancodes[0] = scancode;
    // is_make_code will be the complement of the MSB of the scancode
    // which means it will be 'true' if it's a make code
    bool is_make_code = (scancode >> 7) == 0;
    if (kbd_print_scancode(is_make_code, 1, scancodes) != 0)
      printf("Function fail: kbd_print_scancode() \n"); 
  }
}

int (kbd_test_scan)(bool assembly) {
  // Subscribing interrupts
  uint8_t irq_set;
  if (keyboard_subscribe_int(&irq_set) == FUNCTION_FAIL) {
    printf("Function fail: keyboard_subscribe_int() \n");
    return 1;
  }
  // Interrupt loop
  int ipc_status; message msg;
  while(!esc_key_released) { 
    int r;
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */                                
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            if (assembly) {
              kbc_asm_ih();
              kbd_call_print_code();
              if (scancodes[0] == ESC_KEY_BREAK) esc_key_released = true;
            }
            else {
              kbc_ih();
              switch(kbc_ih_return) {
                case FUNCTION_FAIL: 
                  printf("Warning: Interrupt handler failed during a function call!. \n");
                  break;
                case KBD_COMM_ERROR:
                  printf("Warning: Communication error between Keyboard and KBC! \n");
                  break;
                default: // Only reaches here if there was actually a read scancode         
                  kbd_call_print_code();
                  if (scancodes[0] == ESC_KEY_BREAK) esc_key_released = true;
                  break;
              }
            }
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */        
      }
    }
  }
  if (keyboard_unsubscribe_int() == FUNCTION_FAIL) {
    printf("Function fail: keyboard_unsubscribe_int() \n");
    return 1;
  }
  // Will skip kbd_print_no_sysinb if assembly == true 
  // (Doesn't print sysinb count for the asm handler)
  if (!assembly && kbd_print_no_sysinb(sys_in_counter) != 0) {
    printf("Function fail: kbd_print_no_sysinb() \n");
    return 1;
  }
  printf("Function ended successfully. \n");
  return 0;
}

int (kbd_test_poll)() {
    while(!esc_key_released) { 
      switch(kbc_enable_poll()) {
        case FUNCTION_FAIL: 
          printf("Warning: Polling failed during a function call. \n");
          break;
        case KBD_COMM_ERROR:
          printf("Warning: Communication error between Keyboard and KBC. \n");
          break;
        default:
          kbd_call_print_code();
          if (scancodes[0] == ESC_KEY_BREAK) esc_key_released = true;
          break;
      }
    }
    // Now it will just write a command byte that will reactivate interruptions
    // But needs to keep the other current settings
    uint8_t cmd_byte;
    switch (keyboard_get_cmd_byte(&cmd_byte)) {
        case FUNCTION_FAIL: 
          printf("Function fail: keyboard_get_cmd_byte() \n");
          return 1;
        case KBD_COMM_ERROR:
          printf("Communication error: keyboard_get_cmd_byte() \n");
          return 1;
        default: break;      
    }
    switch (keyboard_write_cmd_byte(cmd_byte | KBC_INT_KBD)) {
        case FUNCTION_FAIL: 
          printf("Function fail: keyboard_write_cmd_byte() \n");
          return 1;
        case KBD_COMM_ERROR:
          printf("Communication error: keyboard_write_cmd_byte() \n");
          return 1;
        default: break;      
    }
  if (kbd_print_no_sysinb(sys_in_counter) != 0) {
    printf("Function fail: kbd_print_no_sysinb() \n");
    return 1;
  }
  printf("Function ended successfully. \n");
  return 0;
}

extern uint32_t timer_aux_counter;

int (kbd_test_timed_scan)(uint8_t n) {
  bool end_of_time = false; // will be true when n seconds passed without any keys pressed / released
  // Subscribing interrupts
  uint8_t irq_set_kbd, irq_set_timer;
  if (timer_subscribe_int(&irq_set_timer)== FUNCTION_FAIL) {
    printf("Function fail: timer_subscribe_int() \n");
    return 1;
  }
  if (keyboard_subscribe_int(&irq_set_kbd) == FUNCTION_FAIL) {
    printf("Function fail: keyboard_subscribe_int() \n");
    return 1;
  }
  int ipc_status; message msg;
  while(!esc_key_released && !end_of_time) { 
    int r;
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */                                     
          if (msg.m_notify.interrupts & irq_set_kbd) { // Got a keyboard interrupt...
            timer_aux_counter = 0;
            kbc_ih();
            switch(kbc_ih_return) {
              case FUNCTION_FAIL: 
                printf("Warning: Interrupt handler failed during a function call!. \n");
                break;
              case KBD_COMM_ERROR:
                printf("Warning: Communication error between Keyboard and KBC! \n");
                break;
              default:
                kbd_call_print_code();
                if (scancodes[0] == ESC_KEY_BREAK) esc_key_released = true;
                break;
            }
          }
          if (msg.m_notify.interrupts & irq_set_timer) { // Got a timer interrupt...
            timer_int_handler();  // Simply increments the counter
            if (timer_aux_counter >= n*60) end_of_time = true;
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */       
      }
    }
  }
  // Unsubscribing both devices
  if (keyboard_unsubscribe_int() == FUNCTION_FAIL) {
    printf("Function fail: keyboard_unsubscribe_int() \n");
    return 1;
  } 
  if (timer_unsubscribe_int() == FUNCTION_FAIL) {
    printf("Function fail: keyboard_unsubscribe_int() \n");
    return 1;
  } 
  printf("Function ended successfully. \n");
  return 0;
}
