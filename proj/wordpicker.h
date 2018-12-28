#pragma once

#include <math.h>
#include <stdlib.h>
#include "textbox.h"

void wordgame_set_rounds(uint16_t num_rounds);
bool is_wordgame_over();
void reset_wordgame();


void loadDictionary(char* folderPath);
void wordgame_start_round(Layer* bg, char* solution, bool isDrawing);
void wordgame_end_round(int points);
int wordgame_get_score();

char* get_random_word();
char* get_solution();

void reveal_letter();
bool verify_guess(char* guess);

void wordgame_showstats();

void loadWordGameHighscore(char* path);
void saveWordGameHighscore();

// clock!

void wordgame_create_clock(Bitmap* bmp);
void wordgame_destroy_clock();
uint16_t wordgame_get_time_left();
bool wordgame_time_up();
void wordgame_tick_clock();
