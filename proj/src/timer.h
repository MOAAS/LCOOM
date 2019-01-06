#pragma once

/** @defgroup timer timer
 * @{
 *
 * Functions used to program the PC timer, adding to the lab2 timer functions.
 */

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
 * @brief Enables the PC speaker, connected to timer 2.
 * 
 * @return int Returns 0 on success. Non-zero otherwise.
 */
int timer_enable_speaker();

/**
 * @brief Disables the PC speaker.
 * 
 * @return int Returns 0 on success. Non-zero otherwise.
 */
int timer_disable_speaker();
