#pragma once

#include <stdlib.h>
#include <math.h>

#include "bitmap.h"
#include "bitmaps.h"
#include "canvas.h"
#include "event.h"
#include "emote.h"
#include "flappy.h"
#include "game_info.h"
#include "interrupts.h"
#include "i8042.h"
#include "i8254.h"
#include "layer.h"
#include "mouse.h"
#include "keyboard.h"
#include "timer.h"
#include "textbox.h"
#include "snake.h"
#include "sprite.h"
#include "uart_protocol.h"
#include "uart_wordgame.h"
#include "vbe.h"
#include "video.h"
#include "wordpicker.h"

typedef enum  {
    Start,
    MainMenu,
    StartWordGame,
    WaitDrawing,
    WaitGuessing,
    Drawing,
    PickingColor,
    Guessing,
    TypingGuess,
    EndRoundWin,
    EndRoundLoss,
    EndGame,
    WaitCollab,
    CollabDrawing,
    Training,
    Loading,
    Saving,
    ExitGame,
    PlayingSnake,
    PlayingFlappy
} GameState;

typedef enum  {
    NoTool,
    Brush,
    Rainbow,
    Bucket,
    Rubber,
    ColorPicker,
    ShapeDraw
} Tool;

typedef struct {
    Tool tool;
    uint8_t thickness;
    uint32_t color1;
    uint32_t color2;
    uint32_t rainbowColor;
    Sprite* cursor;
    bool midShape;
    Shape shape;
    uint16_t x_shapeClick1;
    uint16_t y_shapeClick1;
} DrawingState;

void play_snake();
void play_flappy();

void create_big_ben();
void destroy_big_ben();
void update_big_ben(bool force);

void changeState(GameState newState);

void projeto();
void start_screen();
void main_menu();
void guess();
void type_guess();
void draw();
void color_picker();
void start_round(char* solution);
void end_round();
void end_game();
void training();
void save();
void load();
char* create_bmp_path(char* file_name);

void wait_for_collab();
void collab_drawing();

void select_drawer();
void wait_for_guesser();
void wait_for_drawer();

void setup_canvas();
void display_waiting_for_player();
void update_pencil(DrawingState* pencil, Event_t event);
void update_emote_wheel(Event_t event);


void useTextbox(TextBox* textbox, Event_t event, GameState previous_state, GameState next_state);
void usePencil(DrawingState* pencil, Event_t event);

DrawingState* create_pencil();
void reset_pencil(DrawingState* pencil);
void change_tool(DrawingState* pencil, Tool tool);
void rainbow_draw(DrawingState* pencil, uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf);
void changeThickness(DrawingState* pencil, uint8_t thickness);



