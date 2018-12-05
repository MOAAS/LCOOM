// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

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

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (video_test_init)(uint16_t mode, uint8_t delay) {
  if(video_init(mode) == NULL) {
    printf("Video_init failed! \n");
    return 1;
  }
  sleep(delay);
  vg_exit();
  return 0;
}

int (video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  if(video_init(mode) == NULL) {
    printf("Video_init failed! \n");
    return 1;
  }
  video_draw_rectangle(x, y, width, height, color);
  // Subscribe KBD interrupts
  if (subscribe_device(Keyboard) != 0) {
    printf("subscribe_device(Keyboard) function fail! \n");
    return 1;
  }
  // Wait for ESC Release
  while(1) {
    Notification notification = GetNotification();
    if (notification.keyboardNotif && notification.scancode == ESC_BREAK)
      break;
  }
  // Unsubscribe KBD interrupts
  if (unsubscribe_device(Keyboard) != 0) {
    printf("unsubscribe_device(Keyboard) function fail! \n");
    return 1;
  }
  vg_exit();
  return 1;
}

int (video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if(video_init(mode) == NULL) {
    printf("Video_init failed! \n");
    return 1;
  }
  struct color first_RGB = vg_decompose_RGB(first);
  uint32_t color;
  int rect_width = video_get_hres()/no_rectangles;
  int rect_height = video_get_vres()/no_rectangles;
  int total_width = rect_width * no_rectangles;
  int total_height = rect_height * no_rectangles;
  for(int i=0, col = 0; i < total_width; i += rect_width, col++) {
    for(int j=0, row = 0; j < total_height; j += rect_height, row++) {
        if(vg_mode_is_indexed())
          color = (first + (row * no_rectangles + col) * step) % (1 << video_get_bits_pp());
        else {
          uint8_t colorR = (first_RGB.R + col * step) % (1 << vg_get_mode().RedMaskSize);
          uint8_t colorG = (first_RGB.G + row * step) % (1 << vg_get_mode().GreenMaskSize);
          uint8_t colorB = (first_RGB.B + (col + row) * step) % (1 << vg_get_mode().BlueMaskSize);
          color = vg_compose_RGB(colorR, colorG, colorB);
        }
        video_draw_rectangle(i, j, rect_width, rect_height, color);
    }
  }
  // Subscribe KBD interrupts
  if (subscribe_device(Keyboard) != 0) {
    printf("subscribe_device(Keyboard) function fail! \n");
    return 1;
  }
  // Wait for ESC release
  while(1) {
    Notification notification = GetNotification();
    if (notification.keyboardNotif && notification.scancode == ESC_BREAK)
      break;
  }
  // Unsubscribe KBD interrupts
  if (unsubscribe_device(Keyboard) != 0) {
    printf("unsubscribe_device(Keyboard) function fail! \n");
    return 1;
  }
  vg_exit();
  return 0;
}

int (video_test_xpm)(const char *xpm[], uint16_t x, uint16_t y) {
  if(video_init(VBE_INDEXED_MODE) == NULL) {
    printf("Video_init failed! \n");
    return 1;
  }
  int width, height;
  char *map = read_xpm(xpm, &width, &height);
  if (map == NULL)
    return 1;
  vg_draw_image(map, width, height, x, y);
  // Subscribe KBD interrupts
  if (subscribe_device(Keyboard) != 0) {
    printf("subscribe_device(Keyboard) function fail! \n");
    return 1;
  }
  // Wait for ESC release
  while(1) {
    Notification notification = GetNotification();
    if (notification.keyboardNotif && notification.scancode == ESC_BREAK)
      break;
  }
  // Unsubscribe KBD interrupts
  if (unsubscribe_device(Keyboard) != 0) {
    printf("unsubscribe_device(Keyboard) function fail! \n");
    return 1;
  }
  vg_exit();
  return 1;
}

extern uint16_t scancode;

int (video_test_move)(const char *xpm[], uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf, int16_t speed, uint8_t fr_rate) {
  if(video_init(VBE_INDEXED_MODE) == NULL) {
    printf("Video_init failed! \n");
    return 1;
  }
  if (fr_rate == 0) {
    printf("Frame_rate can't be 0!\n");
    return 1;
  }
  uint32_t counts_per_frame = sys_hz() / fr_rate;
  uint32_t counts_per_pixel = counts_per_frame * -speed; // for negative speeds only
  // Calculate displacement...
  int displacement;
  if (speed > 0) {
    if (yf > yi || xf > xi) displacement = speed;
    else displacement = -speed;
  }
  else if (yf > yi || xf > xi) displacement = 1;
  else displacement = -1;
  // Subscribe KBD interrupts
  if (subscribe_device(Keyboard) != 0) {
    printf("subscribe_device(Keyboard) function fail! \n");
    return 1;
  }
  // Subscribe Timer interrupts
  if (subscribe_device(Timer) != 0) {
    printf("subscribe_device(Timer) function fail! \n");
    return 1;
  }
  timer_reset_counter();
  Sprite* sprite = create_sprite(xpm, xi, yi);
  draw_sprite(sprite);
  /////////////////// Usar o GetNotification() falhava alguns testes
  bool esc_released = false;
  int ipc_status; message msg;
  while(!esc_released) { 
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
            if (speed > 0) { // Positive speed
              if (sprite->x != xf && (timer_get_counter() % counts_per_frame == 0)) {
                if (displacement > 0) move_sprite(sprite, MIN(displacement, xf - sprite->x), 0, 0);
                else move_sprite(sprite, MAX(displacement, xf - sprite->x), 0, 0);
              }
              if (sprite->y != yf && (timer_get_counter() % counts_per_frame == 0)) {
                if (displacement > 0) move_sprite(sprite, 0, MIN(displacement, yf - sprite->y), 0);
                else move_sprite(sprite, 0, MAX(displacement, yf - sprite->y), 0);
              }
            }
            else { // Negative speed
              if (sprite->x != xf && timer_get_counter() % counts_per_pixel == 0)
                move_sprite(sprite, displacement, 0, 0);
              if (sprite->y != yf && timer_get_counter() % counts_per_pixel == 0)
                move_sprite(sprite, 0, displacement, 0);
            }
          }
          if (msg.m_notify.interrupts & BIT(IRQ_SET_KEYBOARD)) { // subscribed interrupt
            kbc_ih();
            if (kbd_assemble_scancode() && scancode == ESC_BREAK)
              esc_released = true;
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
  }
  ////////////

  /*
  Notification notification;

  if(speed > 0) {//velocidade positiva
    while(1) {
      notification = GetNotification();
      if (pengu->x < xf && notification.timerNotif && notification.timer_counter % counts_per_frame == 0)
        move_sprite(pengu, MIN(speed, xf - pengu->x), 0, 0);
      if (pengu->y < yf && notification.timerNotif && notification.timer_counter % counts_per_frame == 0)
        move_sprite(pengu, 0, MIN(speed, yf - pengu->y), 0);
      if (notification.keyboardNotif && notification.scancode == ESC_BREAK)
        break;
    }
  }
  else { //velocidade negativa 
    while(1) {
      notification = GetNotification();
      if (pengu->x < xf && notification.timerNotif && notification.timer_counter % counts_per_pixel == 0)
        move_sprite(pengu, 1, 0, 0);
      if (pengu->y < yf && notification.timerNotif && notification.timer_counter % counts_per_pixel == 0)
        move_sprite(pengu, 0, 1, 0);
      if (notification.keyboardNotif && notification.scancode == ESC_BREAK)
        break;
    }
  }
  */

  destroy_sprite(sprite);
  // Unsubscribe Timer interrupts
  if (unsubscribe_device(Timer) != 0) {
    printf("unsubscribe_device(Timer) function fail! \n");
    return 1;
  }
  // Unsubscribe KBD interrupts
  if (unsubscribe_device(Keyboard) != 0) {
    printf("unsubscribe_device(Keyboard) function fail! \n");
    return 1;
  }
  vg_exit();
  return 1;
}

int (video_test_controller)() {
  void * init_address;
  while ((init_address = lm_init(false)) == NULL) {
    printf("Lm_init failed! Retrying... \n");
  }
  vg_vbe_contr_info_t contr_info;
  if (vbe_get_controllerInfo(&contr_info, init_address) != 0) {
    printf("vbe_get_controllerInfo failed! \n");
    return 1;
  }
  if (vg_display_vbe_contr_info(&contr_info) != 0) {
    printf("vg_display_vbe_contr_info failed! \n");
    return 1;
  }
  return 0;
}  
