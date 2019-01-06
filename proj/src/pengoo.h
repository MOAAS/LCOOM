#pragma once

#include <stdlib.h>
#include <math.h>

#include "bitmap.h"
#include "bitmaps.h"
#include "canvas.h"
#include "clock.h"
#include "event.h"
#include "emote.h"
#include "flappy.h"
#include "game_info.h"
#include "game_state.h"
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

/** @defgroup pengoo pengoo
 * @{
 *
 * Functions used to run the entire Pen-Goo hub.
 */

/**
 * @brief Loads the cursor and initializes the DrawingState.
 * 
 */
void loadCursor();

/**
 * @brief Initializes the used buttons.
 * 
 */
void loadButtons();

/**
 * @brief Loads the game background layer and draws it. Also initializes sprites that go on the background layer.
 * 
 */
void loadBackground();

/**
 * @brief Runs the welcome screen. Waits until the user presses enter to continue. Updates the clock on the top left every second.
 * 
 */
void start_screen();

/**
 * @brief Runs the main hub. Draws all the main menu buttons, allowing the user to proceed by clicking on them.
 * Some of the buttons also display respective instructions.
 * 
 */
void main_menu();

/**
 * @brief Runs the word game, as a guesser. Displays on the screen what the other user draws. 
 * User can press ENTER to type a guess.
 * 
 */
void guess();

/**
 * @brief Runs the word game, as a guesser. Displays on the screen what the other user draws.
 * User can type a guess, press ESC or ENTER to insert his guess.
 * 
 */
void type_guess();

/**
 * @brief Runs the word game, as a drawer. Sends the drawing to the other user on every move. Sends the clock updates to the other user, every second.
 * User can use the several buttons and mouse to draw on the screen.
 * 
 */
void draw();

/**
 * @brief Runs the word game, as a drawer. Sends the drawing to the other user on every move. Sends the clock updates to the other user, every second.
 * User can select a color from the open window.
 * 
 */
void color_picker();

/**
 * @brief Starts a word game round, setting up the canvas and in case the user is drawing, sets up the pencil and buttons.
 * Displaying the word on top or not also depends on whether the user is drawing or not.
 * 
 * @param solution Game solution.
 */
void start_round(char* solution);

/**
 * @brief Ends a game round, adding the gained points. Displays on the screen whether they won or not, displaying the solution.
 * 
 */
void end_round();

/**
 * @brief Ends and resets the game, showing the total amount of points. The user can return to the main menu by pressing enter.
 * 
 */
void end_game();

/**
 * @brief Runs the training/single-player mode. Same as soloing a guessing game but with added Undo, Save and Load buttons.
 * 
 */
void training();

/**
 * @brief Runs the saving of the canvas drawing.
 * 
 * @param saved_img_folder Full path to the folder where the saved images are stored.
 */
void save(char* saved_img_folder);

/**
 * @brief Runs the loading of a .bmp file to the canvas.
 * 
 * @param saved_img_folder Full path to the folder where the saved images are stored.
 */
void load(char* saved_img_folder);

/**
 * @brief Creates a string containing the full path of the saved .bmp file. (String memory is allocated)
 * 
 * @param file_name File name, e.g "drawing"
 * @param saved_img_folder Full path for the folder to save on
 * @return char* Returns the full path (e.g "folder/drawing.bmp"). 
 */
char* create_bmp_path(char* file_name, char* saved_img_folder);

/**
 * @brief Runs the collab/multi-player mode. Same as training mode, but without the Undo and Load buttons.
 * In this game, both users get to draw and see other's actions in real time.
 * 
 */
void collab_drawing();

/**
 * @brief Runs the rainbow snake game. User can exit at any time by pressing ESC.
 * 
 */
void play_snake();

/**
 * @brief Runs the flappy rainbow game. User can exit at any time by pressing ESC.
 * 
 */
void play_flappy();

// Syncronizing

/**
 * @brief Waits for the other user, and randomly decide who is drawing first in the word game.
 * They do this by choosing a random number and sending it every clock tick. The highest number draws first.
 * 
 */
void select_drawer();

/**
 * @brief Displays on the screen the message "Waiting for player..."
 * 
 */
void waiting_for_player();

/**
 * @brief Waits for the other user to show up to the multi-player mode.
 * 
 */
void wait_for_collab();

/**
 * @brief Waits for the guesser to be ready to start a wordgame round (user is drawing)
 * 
 */
void wait_for_guesser();

/**
 * @brief Waits for the drawer to be ready to start a wordgame round (user is guessing)
 * 
 */
void wait_for_drawer();

/**
 * @brief Reads an event and the cursor and updates the emote "wheel" accordingly (applies in the word game).
 * If the emote "wheel" button was pressed, toggles the window. If an emote was pressed on, sends it to the other player.
 * 
 * @param event Event to be read.
 */
void update_emote_wheel(Event_t event);

/**
 * @brief Sets the canvas up, along with the buttons, depending on the current gameState.
 * 
 */
void setup_canvas();

/**
 * @brief Resets the drawing state to the initial one. (Black/White, thickness 4, NoTool, etc.)
 * 
 */
void reset_pencil();

/**
 * @brief Changes the drawing-state/pencil tool, updating the cursor and the paint tubes as well.
 * 
 * @param tool Tool to be changed to.
 */
void change_tool(Tool tool);

/**
 * @brief Uses the pencil by reading an event. Depending on the tool, it may draw on the screen, update colors, etc.
 * 
 * @param event 
 */
void usePencil(Event_t event);

/**
 * @brief Updates the drawing state, by checking whether or not a painting button was pressed.
 * Updates the thickness slider if needed.
 * Updades the cursor if needed.
 * 
 * @param event Event to be read.
 */
void update_pencil(Event_t event);

/**
 * @brief Changes the pencil thickness, to a maximum of 10.
 * 
 * @param thickness 
 */
void changeThickness(uint8_t thickness);

/**
 * @brief Draws a line with the rainbow tool. Updates the paint tubes and the pencil color.
 * 
 * @param x0 Coordinates of the beggining of the line
 * @param y0 Coordinates of the beggining of the line
 * @param xf Coordinates of the end of the line
 * @param yf Coordinates of the end of the line
 */
void rainbow_draw(uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf);



