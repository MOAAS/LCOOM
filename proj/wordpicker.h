#pragma once
#include "textbox.h"

void loadDictionary();
void word_pick_start(Layer* bg, char* solution, bool isDrawing);
void word_pick_end();
char* get_random_word();
void reveal_letter();
bool verify_guess(char* guess);

