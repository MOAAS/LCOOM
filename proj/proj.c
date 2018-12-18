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

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
 // lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

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
  projeto();
  return 0;
  subscribe_device(SerialPort);
  subscribe_device(Keyboard);
  subscribe_device(Mouse);
  while (1) {
    Event_t event = GetEvent();
    if (event.isMouseEvent && event.mouseEvent.type == LB_PRESS) {
      printf("SENDING!\n");
      uart_send_draw_line1(364, 364, 364, 364, 16711680, 4);
      uart_send_draw_line1(364, 364, 368, 368, 16711680, 4);
     // uart_fifo_send(notif.mouse_packet.delta_x);
     // uart_fifo_send(notif.mouse_packet.delta_y);
     // uart_fifo_send(notif.mouse_packet.lb);
     // uart_fifo_send(notif.mouse_packet.rb);
     // uart_fifo_send(notif.mouse_packet.mb);
     // uart_fifo_send(notif.mouse_packet.delta_x);
     // uart_fifo_send(notif.mouse_packet.delta_y);
     // uart_fifo_send(notif.mouse_packet.lb);
     // uart_fifo_send(notif.mouse_packet.rb);
     // uart_fifo_send(notif.mouse_packet.mb);
     // uart_fifo_send(notif.mouse_packet.delta_x);
     // uart_fifo_send(notif.mouse_packet.delta_y);
     // uart_fifo_send(notif.mouse_packet.lb);
     // uart_fifo_send(notif.mouse_packet.rb);
     // uart_fifo_send(notif.mouse_packet.mb);
     // uart_fifo_send(notif.mouse_packet.delta_x);
     // uart_fifo_send(notif.mouse_packet.delta_y);
     // uart_fifo_send(notif.mouse_packet.lb);
     // uart_fifo_send(notif.mouse_packet.rb);
     // uart_fifo_send(notif.mouse_packet.mb);
     // uart_fifo_send(notif.mouse_packet.delta_x);
     // uart_fifo_send(notif.mouse_packet.delta_y);
     // uart_fifo_send(notif.mouse_packet.lb);
     // uart_fifo_send(notif.mouse_packet.rb);
     // uart_fifo_send(notif.mouse_packet.mb);
     // uart_fifo_send(notif.mouse_packet.delta_x);
     // uart_fifo_send(notif.mouse_packet.delta_y);
     // uart_fifo_send(notif.mouse_packet.lb);
     // uart_fifo_send(notif.mouse_packet.rb);
     // uart_fifo_send(notif.mouse_packet.mb);
     // uart_fifo_send(notif.mouse_packet.delta_x);
     // uart_fifo_send(notif.mouse_packet.delta_y);
     // uart_fifo_send(notif.mouse_packet.lb);
     // uart_fifo_send(notif.mouse_packet.rb);
     // uart_fifo_send(notif.mouse_packet.mb);
     // uart_fifo_send(notif.mouse_packet.delta_x);
     // uart_fifo_send(notif.mouse_packet.delta_y);
     // uart_fifo_send(notif.mouse_packet.lb);
     // uart_fifo_send(notif.mouse_packet.rb);
     // uart_fifo_send(notif.mouse_packet.mb);
    }
    if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS)
      break;
    if (event.isUARTEvent) {
      printf("num uart_messages: %d\n", event.num_uart_messages);
      for (int j = 0; j < event.num_uart_messages; j++) {
        uart_receive_draw_line1(&event.uart_messages[j]);
      }
    }
  }
  unsubscribe_device(Mouse);
  unsubscribe_device(Keyboard);
  unsubscribe_device(SerialPort);
  return 0;
}


