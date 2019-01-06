#pragma once

#include "canvas.h"
#include "sprite.h"
#include "textbox.h"

/** @defgroup game_state game_state
 * @{
 *
 * Functions used to update and maintain a record of the game state.
 */

/**
 * @brief Enumerates all the possible game states
 * 
 */
typedef enum  {
    Start,          ///< On welcome screen
    MainMenu,       ///< On the main menu
    StartWordGame,  ///< Waiting for another player to play (decide who's drawing first)
    WaitDrawing,    ///< Drawing next round, waiting for player to be ready
    WaitGuessing,   ///< Guessing next round, waiting for player to be ready
    Drawing,        ///< Currently drawing
    PickingColor,   ///< Currently drawing, but with palette open
    Guessing,       ///< Currently guessing
    TypingGuess,    ///< Currently guessing, but with text box open
    EndRoundWin,    ///< Round ends, players won
    EndRoundLoss,   ///< Round ends, players lost
    EndGame,        ///< Game ends
    WaitCollab,     ///< Waiting for another player to both draw
    CollabDrawing,  ///< Two player drawing
    Training,       ///< Single player drawing
    Loading,        ///< Choosing a file to load
    Saving,         ///< Choosing a file name to save the drawing
    ExitGame,       ///< Exit the game
    PlayingSnake,   ///< Playing rainbow snake
    PlayingFlappy   ///< Playing flappy rainbow
} GameState;

/**
 * @brief Enumerates all the used drawing tools
 * 
 */
typedef enum  {
    NoTool,     ///< No tool used
    Brush,      ///< Normal brush
    Rainbow,    ///< Rainbow brush
    Bucket,     ///< Bucket tool
    Rubber,     ///< Eraser
    ColorPicker,///< Color picker tool
    ShapeDraw   ///< Drawing a shape
} Tool;

/**
 * @brief Contains information about the drawing state.
 * 
 */
typedef struct {
    Tool tool;          ///< Current tool (NoTool if not drawing)
    uint8_t thickness;  ///< Pencil thickness
    uint32_t color1;    ///< Primary color
    uint32_t color2;    ///< Secondary color
    uint32_t rainbowColor;  ///< Current color on the rainbow tool gradient
    Sprite* cursor;     ///< Corresponding cursor
    bool midShape;      ///< Set to true iff the Tool is ShapeDraw and the first click has been made 
    Shape shape;        ///< Shape currently being drawn
    uint16_t x_shapeClick1; ///< Position of the first click (unused if tool is not ShapeDraw)
    uint16_t y_shapeClick1; ///< Position of the first click (unused if tool is not ShapeDraw)
} DrawingState;

/**
 * @brief Changes the game state. Saving the current state on lastGameState
 * 
 * @param newState New game state.
 */
void changeState(GameState newState);

/**
 * @brief Creates a pencil.
 * 
 * @param cursor Cursor to be associated with the pencil.
 */
void create_pencil(Sprite* cursor);

/**
 * @brief Reads a keyboard event and uses a text box, depending on the given event. If a keyboard character was pressed, inserts it.
 * If esc or enter was pressed, changes gameState
 * 
 * @param textbox Textbox to be used
 * @param event Event to be read
 * @param previous_state State to change to, if ESC was pressed
 * @param next_state State to change to, if ENTER was pressed
 */
void useTextbox(TextBox* textbox, Event_t event, GameState previous_state, GameState next_state);

