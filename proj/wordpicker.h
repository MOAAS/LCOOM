#pragma once

#include <math.h>
#include <stdlib.h>
#include "textbox.h"

/**
 * @brief Sets the wordgame round count. (1 round means both players get to draw)
 * 
 * @param num_rounds Number of rounds
 */
void wordgame_set_rounds(uint16_t num_rounds);

/**
 * @brief Checks if the word game is over.
 * 
 * @return true The game is over
 * @return false The game is not over
 */
bool is_wordgame_over();

/**
 * @brief Resets the wordgame. Resets the round count and score.
 * 
 */
void reset_wordgame();

/**
 * @brief Loads a list of words from a .txt fill to an array.
 * 
 * @param path Full path of the .txt file.
 */
void loadDictionary(char* path);

/**
 * @brief Starts a wordgame round.
 * 
 * @param bg Layer where the word will be drawn
 * @param solution Word solution
 * @param isDrawing Set to true iff the player is drawing this round.
 */
void wordgame_start_round(Layer* bg, char* solution, bool isDrawing);

/**
 * @brief Ends a wordgame round.
 * 
 * @param points Number of points scored.
 */
void wordgame_end_round(int points);

/**
 * @return int Returns the score so far.
 */
int wordgame_get_score();

/**
 * @return char* Returns a random "word"
 */
char* get_random_word();

/**
 * @return char* Returns the solution for the current round
 */
char* get_solution();

/**
 * @brief Reveals a letter, for the guesser.
 * 
 */
void reveal_letter();

/**
 * @brief Checks if a guess is correct (case-insensitive)
 * 
 * @param guess Player guess
 * @return true The guess is correct
 * @return false The guess is wrong
 */
bool verify_guess(char* guess);

/**
 * @brief Displays on the screen the highscore and points
 * 
 */
void wordgame_showstats();

/**
 * @brief reads the highscore .txt file for the wordgame. Sets highscore to 0 if the file does not exist.
 * 
 * @param path Full path for the .txt file
 */
void loadWordGameHighscore(char* path);

/**
 * @brief Saves the highscore .txt file for the snake minigame.
 * Assumes loadWordGameHighscore() was previously called.
 * 
 */
void saveWordGameHighscore();

// clock!

/**
 * @brief Creates a clock on the top left, starting at 90 seconds left.
 * 
 * @param bmp Bitmap to be drawn
 */
void wordgame_create_clock(Bitmap* bmp);

/**
 * @brief Destroys the clock.
 * 
 */
void wordgame_destroy_clock();

/**
 * @return uint16_t Returns the time left in the clock
 */
uint16_t wordgame_get_time_left();

/**
 * @brief Checks if the time is up
 *
 * @return true Clock time == 0.
 * @return false Clock time != 0.
 */
bool wordgame_time_up();

/**
 * @brief Ticks the clock. Reduces its count by 1, updates the screen.
 * 
 */
void wordgame_tick_clock();
