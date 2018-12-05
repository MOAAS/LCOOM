#pragma once

#include "event.h"
#include "textbox.h"

void create_clock(Bitmap* bmp, uint16_t time_left);
void destroy_clock();
void update_clock(Event_t event);
uint16_t clock_get_time_left();

