#pragma once

#include <stdlib.h>
#include "textbox.h"

void wordgame_set_rounds(uint16_t num_rounds);
bool is_wordgame_over();
void reset_wordgame();


void loadDictionary(char* folderPath);
void word_pick_start(Layer* bg, char* solution, bool isDrawing);
void word_pick_end();
char* get_random_word();
char* get_solution();

void reveal_letter();
bool verify_guess(char* guess);

