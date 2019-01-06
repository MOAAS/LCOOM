#pragma once

#include "layer.h"

/** @defgroup game_info game_info
 * @{
 *
 * Functions used to display game instructions.
 */

/**
 * @brief Enumerator to represent each type of user instruction list.
 * 
 */
typedef enum {
    WordGameInfo,   ///< Word game instructions
    DrawToolInfo,   ///< Draw tool instructions
    SnakeInfo,      ///< Rainbow snake instructions
    FlappyInfo      ///< Flappy rainbow instructions
} InfoType;


/**
 * @brief Toggles the instructions window for a certain game mode. (If it's open closes it, if it's closed opens it and closes the other ones)
 * 
 * @param type Game mode/tool to which will be given/cleared info
 */
void toggle_instructions(InfoType type);

/**
 * @brief Displays, on the screen, instructions for a specific game mode.
 * 
 * @param type Game mode/tool to which will be given info
 */
void display_instructions(InfoType type);

/**
 * @brief If there are any, clears the instructions on the screen.
 * 
 */
void clear_instructions();
