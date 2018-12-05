#pragma once

#define IRQ_SET_TIMER 0

int timer_wait(uint32_t time_s);
void timer_reset_counter();
uint32_t timer_get_counter();
uint32_t timer_get_frequency();
