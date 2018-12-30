#pragma once
#include "i8042.h"

/**
 * @brief Resets the timer auxiliary counter to 0. Useful to synchronize the timer with the program.
 * 
 */
void timer_reset_counter();

/**
 * @return uint32_t Returns the timer auxiliary counter.
 */
uint32_t timer_get_counter();

/**
 * @brief Enabler the PC speaker, connected to timer 2.
 * 
 * @return int 0 on success. Non-zero otherwise.
 */
int timer_enable_speaker();
