#pragma once
#include "textbox.h"

void loadDictionary();
void word_pick_start(Layer* bg, char* solution, bool isDrawing);
char* get_random_word();
void reveal_letter();
char* get_solution();

