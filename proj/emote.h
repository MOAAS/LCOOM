#pragma once

#include "sprite.h"

typedef enum {
    Hello = 0,
    GoodGame = 1,
    ThankYou = 2,
    YouWelcome = 3,
    GoodLuck = 4
} Emote;

/**
 * @brief Draws an emote bubble on the screen. If there's already one, destroys it first.
 * 
 * @param emote Enumerator representing the emote to draw on.
 */
void draw_emote(Emote emote);

/**
 * @brief Destroys the current emote bubble on-screen (if there's one).
 * 
 */
void destroy_emote();

/**
 * @brief Checks if the emote "wheel" pop-up is open and showing up on screen or not
 * 
 * @return true It is open
 * @return false It is not open 
 */
bool is_emote_wheel_on();

/**
 * @brief Toggles the emote wheel and its button between on and off.
 * 
 * @param emote_wheel Button to toggle
 * @param cursor Mouse cursor
 */
void toggle_emote_wheel(Button* emote_wheel, Sprite* cursor);

/**
 * @brief Draws the emote "wheel" on the screen
 * 
 * @param cursor Mouse cursor
 */
void draw_emote_wheel(Sprite* cursor);

/**
 * @brief Removes the emote wheel from the screen.
 * 
 */
void destroy_emote_wheel();

/**
 * @brief Reads an event and checks if the user pressed on any emote to send. 
 * Also updates buttons based on whether they are highlighted/pressed or not
 * 
 * @param event Event to read
 * @param cursor Mouse cursor
 * @return int Emote number. Returns -1 if no emote was pressed on.
 */
int check_emote_press(Event_t event, Sprite* cursor);
