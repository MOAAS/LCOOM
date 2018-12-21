#pragma once

#include "textbox.h"

void create_clock(Bitmap* bmp);
void destroy_clock();
void tick_clock();
uint16_t clock_get_time_left();
bool clock_time_up();

