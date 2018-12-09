#pragma once

#include "bitmap.h"
#include "canvas.h"
#include "clock.h"
#include "event.h"
#include "interrupts.h"
#include "i8042.h"
#include "i8254.h"
#include "layer.h"
#include "mouse.h"
#include "keyboard.h"
#include "timer.h"
#include "textbox.h"
#include "sprite.h"
#include "vbe.h"
#include "video.h"
#include "wordpicker.h"

typedef enum  {
    MainMenu,
    Drawing,
    Guessing,
    TypingGuess,
    Saving,
    EndGameWin,
    EndGameLoss,
    ExitGame
} GameState;

typedef enum  {
    None,
    Brush,
    Rainbow,
    Bucket,
    Rubber,
    ColorPicker
} Tool;

typedef struct {
    Tool tool;
    Sprite* cursor;
    bool isDrawing;
    uint8_t thickness;
    uint32_t color1;
    uint32_t color2;
    uint32_t rainbowColor;
} DrawingState;

void projeto();
void main_menu();
void guess();
void type_guess();
void draw();
void start_game();
void end_game();
void save();



void useTextbox(TextBox* textbox, Event_t event, GameState previous_state, GameState next_state);
void usePencil(DrawingState* pencil, Event_t event);

DrawingState* create_pencil();
void setupDrawing();
void change_tool(DrawingState* pencil, Tool tool);
void rainbow_draw(DrawingState* pencil, uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf);
void increaseThickness(DrawingState* pencil);
void decreaseThickness(DrawingState* pencil);

