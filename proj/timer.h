#pragma once
#include "i8042.h"


int timer_wait(uint32_t time_s);
void timer_reset_counter();
uint32_t timer_get_counter();
uint32_t timer_get_frequency();
int timer_enable_speaker();
