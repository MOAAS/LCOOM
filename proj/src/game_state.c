#include <lcom/lcf.h>

#include "game_state.h"


GameState gameState = Start;
GameState lastGameState = Start;
DrawingState* pencil;


void changeState(GameState newState) {
    lastGameState = gameState;
    gameState = newState;
}

void create_pencil(Sprite* cursor) {
    pencil = malloc(sizeof(DrawingState));
    pencil->color1 = BLACK;
    pencil->color2 = WHITE;
    pencil->tool = NoTool;
    pencil->thickness = 4;
    pencil->rainbowColor= RED;
    pencil->cursor = cursor;
    pencil->midShape = false;
}

void useTextbox(TextBox* textbox, Event_t event, GameState previous_state, GameState next_state) {
    if (!event.isKeyboardEvent)
        return;
    if (textbox->isEmpty && event.keyboardEvent.type != ENTER_RELEASE) {
        textbox->isEmpty = false;
        textbox_clear(textbox);
    }
    switch(event.keyboardEvent.type) {
        case CHARACTER_PRESS: 
            textbox_put(textbox, event.keyboardEvent.character); 
            return;
        case BACKSPACE_PRESS: 
            textbox_backspace(textbox);
            return;
        case ESC_PRESS:
            changeState(previous_state); 
            return;
        case ENTER_PRESS: changeState(next_state); return;
        default: return;
    }
}
