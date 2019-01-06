#pragma once

#include "textbox.h"
#include "rtc.h"

/** @defgroup clock clock
 * @{
 *
 * Functions used to create and update a date-displaying clock
 */

/**
 * @brief Creates a big clock on the top left of the screen, displaying the current date.
 * 
 */
void create_big_ben();

/**
 * @brief Destroys the big clock
 * 
 */
void destroy_big_ben();

/**
 * @brief Updates the clock on the screen with the current date.
 * 
 * @param force If set to true, it will re-update the entire date. If set to false, will only update what it thinks has changed in the meantime.
 */
void update_big_ben(bool force);
