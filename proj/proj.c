// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "video.h"
#include "keyboard.h"
#include "timer.h"
#include "mouse.h"
#include "i8042.h"
#include "i8254.h"
#include "interrupts.h"
#include "Sprite.h"
#include "vbe.h"
#include "projeto.h"
#include "bitmap.h"
#include "canvas.h"
#include "layer.h"
#include "uart.h"
#include "rtc.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (proj_main_loop)() { //int argc, char *argv[]) {
 //timer_enable_speaker();
 //timer_set_frequency(2, 1000);
//return 0;
 
 
 /*
  subscribe_device(RTC);
  for(int i=0 ; i< 10 ;){
    Notification notification = GetNotification();
    if(notification.rtcAlarmNotif){
      Mini_Date d = rtc_get_mini_date();
      print_mini_date(d);
      i++;
    }
  }
 unsubscribe_device(RTC);
 */

 projeto();
 return 0;
  subscribe_device(SerialPort);
  subscribe_device(Keyboard);
  subscribe_device(Mouse);
  subscribe_device(Timer);
  int rcv_cnt = 0, xmit_cnt = 0;
  while (1) {
    Event_t event = GetEvent();
    if (event.isTimerEvent && event.timerEvent.timer_counter % (rand() % 60 + 1) == 0) {
      printf("Sending...\n");
      uart_send_drawer_ready("HI HOW ARE");
     //uart_send_drawer_ready("HI HOW ARE YOU");
     //uart_send_drawer_ready("HI HOW ARE YOU");
     //uart_send_drawer_ready("HI HOW ARE YOU");
     //uart_send_drawer_ready("HI HOW ARE YOU");
      xmit_cnt++;
      printf("Transmit count = %d \n", xmit_cnt);
    }
    if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS)
      break;
    if (event.isKeyboardEvent && event.keyboardEvent.type == ENTER_PRESS) {
      uint8_t status;
      uart_read_status(&status);
      while (status & UART_RECEIVER_DATA) {
        printf("status = %d \n", status);
        uart_read_data(&status);
        uart_read_status(&status);
      }
      printf("done (status = %d)", status);

    }

    if (event.isUARTEvent) {
     // printf("num uart_messages: %d\n", event.num_uart_messages);
      for (int j = 0; j < event.num_uart_messages; j++) {
        //uart_process_msg(&event.uart_messages[j]);
        if (event.uart_messages[j].type == MSG_DRAWER_READY) {
          printf ("string: ");
          printf ("\"%s\" \n", event.uart_messages[j].bytes);
          rcv_cnt++;
          printf("Receiver count = %d \n", rcv_cnt);
        }
      }
    }
  }
  unsubscribe_device(Mouse);
  unsubscribe_device(Keyboard);
  unsubscribe_device(SerialPort);
  return 0;
}


