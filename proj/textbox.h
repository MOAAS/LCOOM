#pragma once

#include <math.h>
#include "layer.h"
#include "bitmap.h"

typedef enum {
    LeftAlign,  ///< Align text to the left
    CenterAlign ///< Align text to the center (vertically and horizontally)
} TextAlignment;

typedef struct {
    Layer* layer;           ///< Layer to draw text and bitmap on
    uint16_t x;             ///< X position of the text box
    uint16_t y;             ///< Y position of the text box
    int16_t cursorX;        ///< Current X position of the text cursor
    int16_t cursorY;        ///< Current Y position of the text cursor     
    int16_t cursorX_init;   ///< Initial X position of the text cursor
    int16_t cursorY_init;   ///< Initial Y position of the text cursor
    int16_t cursorX_limit;  ///< Maximum X position of the text cursor
    int16_t cursorY_limit;  ///< Maximum Y position of the text cursor
    uint16_t text_size;     ///< Number of inserted characters
    uint8_t font_size;      ///< Number of pixels per row/column
    Bitmap* bitmap;         ///< Bit map image of the text box
    char text[200];         ///< Text contained within the text box
    TextAlignment alignment;///< Tells how to align each character
    bool isEmpty;           ///< Helper to signal if no text has been inserted by the user yet
} TextBox;

/**
 * @brief Loads the bitmap arrays, from the letters_bmp Bitmap, in order to make drawing of larger letters less time consuming.
 * 
 */
void loadLetterMap();

/**
 * @brief Creates an empty textbox and draws it.
 * 
 * @param layer Layer to draw the text box on.
 * @param bitmap Bitmap to draw with
 * @param x_disp Initial horizontal displacement of the text cursor
 * @param y_disp Initial vertical displacement of the text cursor
 * @param font_scale font_size will be 16*font_scale
 * @param alignment How to align each character written
 * @return TextBox* Returns the created textbox
 */
TextBox* create_textbox(Layer* layer, Bitmap* bitmap, uint16_t x_disp, uint16_t y_disp, uint8_t font_scale, TextAlignment alignment);

/**
 * @brief Destroys a textbox and the layer used by it, freeing all used memory.
 * 
 * @param textbox Textbox to destroy
 */
void destroy_textbox(TextBox* textbox);

/**
 * @brief Writes a string on a textbox, updating the screen
 * 
 * @param textbox Textbox to write on
 * @param string String to write
 */
void textbox_write(TextBox* textbox, char* string);

/**
 * @brief Writes a number on a textbox, updating the screen
 * 
 * @param textbox Textbox to write on
 * @param number Number to write
 */
void textbox_write_int(TextBox* textbox, int number);

/**
 * @brief Clears all the text in a textbox
 * 
 * @param textbox Textbox to clear
 */
void textbox_clear(TextBox* textbox);

/**
 * @brief Moves the cursor to its initial X position and moves it down by font_size
 * 
 * @param textbox Textbox to line feed
 */
void textbox_linefeed(TextBox* textbox);

/**
 * @brief Writes a character on a textbox, updating the screen
 * 
 * @param textbox Textbox to write on
 * @param character Character to write
 */
void textbox_put(TextBox* textbox, char character);

/**
 * @brief Deletes the last written character, updating the screen
 * 
 * @param textbox Textbox where the character will be erased
 */
void textbox_backspace(TextBox* textbox);

/**
 * @brief Draws a character on the screen.
 * 
 * @param layer Layer to draw on.
 * @param character Character to draw
 * @param x X position of the character
 * @param y Y position of the character
 * @param font_scale font_size will be 16*font_scale
 * @param alignment How to align the character:
 * CenterAlign means (X,Y) will be the character center.
 * LeftAlign means that (X,Y) will be the upper left corner.
 */
void draw_char(Layer* layer, char character, int16_t x, int16_t y, uint16_t font_scale, TextAlignment alignment);

/**
 * @brief Draws a string on the screen.
 * 
 * @param layer Layer to draw on.
 * @param word String to draw
 * @param x X position of the string
 * @param y Y position of the string
 * @param font_scale font_size will be 16*font_scale
 * @param alignment How to align the string: 
 * CenterAlign means (X,Y) will be the center of the string. 
 * LeftAlign means that (X,Y) will be the upper left corner of the first character.
 */
void draw_word(Layer* layer, char* word, int16_t x, int16_t y, uint16_t font_scale, uint16_t space_scale, TextAlignment alignment);

