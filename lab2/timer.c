#include <lcom/lcf.h>
//#include <lcom/timer.h>
#include <stdint.h>
#include "i8254.h"

#define FUNCTION_FAIL 1
#define BAD_PARAMS 2

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  // Check for parameters
  if (timer < 0 || timer > 2 || freq <= 0)
    return BAD_PARAMS;
  // Check for overflow in the counter
  if (TIMER_FREQ / freq > 0xFFFF || freq > TIMER_FREQ)
    return BAD_PARAMS;
  // Gets the 4 lsb of the control word
  uint8_t status;
  if (timer_get_conf(timer, &status))
    return FUNCTION_FAIL;
  status &= 0xF;
  // Calculates values to introduce
  uint16_t counterValue = TIMER_FREQ / freq;
  uint8_t MSB, LSB;
  util_get_LSB(counterValue, &LSB);
  util_get_MSB(counterValue, &MSB);
  // Configures timer  
  port_t timerPort = TIMER_N(timer);
  uint8_t cword = TIMER_SEL(timer) | TIMER_LSB_MSB | status;
  if (sys_outb(TIMER_CTRL, cword) != OK || sys_outb(timerPort, LSB) != OK || sys_outb(timerPort, MSB) != OK)
    return FUNCTION_FAIL;
  return 0;
}

int hook_id;

int (timer_subscribe_int)(uint8_t *bit_no) {  
  hook_id = 0; 
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK)
    return FUNCTION_FAIL;
  *bit_no = BIT(0);
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != OK)
    return FUNCTION_FAIL;
  return 0;
}

uint32_t aux_counter;

void (timer_int_handler)() {
  aux_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (timer < 0 || timer > 2)
    return BAD_PARAMS;
  // Assembling read-back command = 11100000 | BIT(timer+1)
  uint8_t rbCommand = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer);
  if (sys_outb(TIMER_CTRL, rbCommand) != OK)
    return FUNCTION_FAIL;  
  // Determines the timer address, gets configuration, saved on st_temp
  uint32_t st_temp;
  port_t timerPort = TIMER_N(timer);
  if (sys_inb(timerPort, &st_temp) != OK)
    return FUNCTION_FAIL;
  *st = (uint8_t)st_temp;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  if (timer < 0 || timer > 2)
    return BAD_PARAMS;
  union timer_status_field_val conf;
  if (field == all) 
    conf.byte = st;
  else if (field == initial)
    conf.in_mode = (st & TIMER_LSB_MSB) >> 4;
  else if (field == mode) {
    conf.count_mode = (st & TIMER_MODE_MASK) >> 1;
    // Need to check if don't care bit (X) is equal to 1
    if (conf.count_mode == 6 || conf.count_mode == 7)
      conf.count_mode &= ~BIT(2); // Puts BIT(2) equal to 0
  }
  else if (field == base)
    conf.bcd = st & TIMER_BCD;
  else return BAD_PARAMS;
  return timer_print_config(timer, field, conf);
}


