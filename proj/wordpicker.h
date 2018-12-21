#pragma once

#include <stdlib.h>
#include "textbox.h"

void loadDictionary(char* folderPath);
void word_pick_start(Layer* bg, char* solution, bool isDrawing);
void word_pick_end();
char* get_random_word();
void reveal_letter();
bool verify_guess(char* guess);

