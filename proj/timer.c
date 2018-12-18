#include <lcom/lcf.h>
#include <stdint.h>
#include "i8254.h"
#include "timer.h"
#include "interrupts.h"

#define FUNCTION_FAIL 1
#define BAD_PARAMS 2

int hook_id_timer = TIMER0_IRQ;

static uint32_t timer_aux_counter = 0;
static uint32_t timer_frequency = 60;

int (timer_subscribe_int)(uint8_t *bit_no) {  
  *bit_no = hook_id_timer = TIMER0_IRQ;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id_timer) != OK)
    return FUNCTION_FAIL;
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id_timer) != OK)
    return FUNCTION_FAIL;
  return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (timer < 0 || timer > 2 || freq <= 0)
    return BAD_PARAMS;
  if (freq < 20) // In case of overflow, will prevent it.
    freq = 20;
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
  timer_frequency = freq;
  return 0;
}

void (timer_int_handler)() {
  timer_aux_counter++;
}

void timer_reset_counter() {
  timer_aux_counter = 0;
}

uint32_t timer_get_counter() {
  return timer_aux_counter;
}

uint32_t timer_get_frequency() {
  return timer_frequency;
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

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)(val & 0xFF);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t)(val >> 8);
  return 0;
}
