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
    Typing,
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
    bool isDrawing;
    uint8_t thickness;
    uint32_t color1;
    uint32_t color2;
    uint32_t rainbowColor;
} DrawingState;

void projeto();
void guess();
void draw();
void type();
void start_game();
void end_game();

void useTextbox(TextBox* textbox, Event_t event);

void main_menu();
void usePencil(DrawingState* pencil, Event_t event);
DrawingState* create_pencil();

void increaseThickness(DrawingState* pencil);
void decreaseThickness(DrawingState* pencil);

