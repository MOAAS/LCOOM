#pragma once
#include "video.h"
#include "event.h"
#include "layer.h"
#include "bitmap.h"


typedef struct {
  int x,y;             /**< current sprite position */
  int width, height;   /**< sprite dimensions */
  Bitmap* bitmap;      /**< the sprite bitmap */
  uint32_t color;     
} Sprite;



/** Creates a new sprite with pixmap "pic", with specified position (within the screen limits).
* Does not draw the sprite on the screen
* Returns NULL on invalid pixmap.
*/
Sprite *create_sprite(Bitmap* bitmap, int x, int y);

/** The "fig" sprite is erased from memory whose address is "base"
 * and used resources released.
 */
void destroy_sprite(Sprite *fig);

/**
 * @brief Draws a sprite on the screen
 * 
 * @param sp Sprite to be drawn
 */
void draw_sprite(Sprite *sp);

/**
 * @brief Draws a sprite on the screen, and replaces every color GREEN with its color atribute.
 * 
 * @param sp Sprite to be drawn
 */
void draw_sprite_color(Sprite *sp);

/**
 * @brief Erases a sprite from the screen
 * 
 * @param sp Sprite to be erased
 */
void erase_sprite(Sprite *sp);

/**
 * @brief Moves a sprite
 * 
 * @param sp Sprite to move
 * @param delX Delta X (x difference) 
 * @param delY Delta Y (y difference)
 */
void move_sprite(Sprite *sp, int16_t delX, int16_t delY);

/**
 * @brief Changes the color of a sprite
 * 
 * @param sp Sprite to alter
 * @param color New color
 */
void sprite_set_color(Sprite *sp, uint32_t color);

/**
 * @brief Gets the x position a cursor (sprite) would  be on after moving a horizontal amount.
 * 
 * @param cursor Sprite to check for
 * @param delX Distance moved
 * @return uint16_t Future position
 */
uint16_t cursor_get_xf(Sprite* cursor, int16_t delX);

/**
 * @brief Gets the y position a cursor (sprite) would  be on after moving a vertical amount.
 * 
 * @param cursor Sprite to check for
 * @param delY Distance moved
 * @return uint16_t Future position
 */
uint16_t cursor_get_yf(Sprite* cursor, int16_t delY);

/**
 * @brief Updates a cursor (sprite), by reading an event.
 * 
 * @param cursor Sprite to be updated
 * @param event Event to read
 */
void update_cursor(Sprite *cursor, Event_t event);

/**
 * @brief Change the bitmap of a sprite
 * 
 * @param cursor Sprite to be changed
 * @param bitmap New bitmap
 */
void cursor_change_bmp(Sprite *cursor, Bitmap* bitmap);

/**
 * @brief Rectangle shape representing an area belonging to a sprite.
 * 
 */
typedef struct {
  int x1, x2, y1, y2; 
  int width, height;
} Hitbox;

/**
 * @brief Creates a hitbox from 2 positions
 * 
 * @param hitbox Hitbox to be created
 * @param x1 Leftmost x
 * @param x2 Rightmost x
 * @param y1 Uppermost y
 * @param y2 Downmost y
 */
void make_hitbox(Hitbox* hitbox, int16_t x1, int16_t x2, int16_t y1, int16_t y2);

/**
 * @brief Moves a hitbox a certain amount
 * 
 * @param hitbox Hitbox to be altered
 * @param delx Delta X
 * @param dely Delta Y
 */
void move_hitbox(Hitbox* hitbox, int16_t delx, int16_t dely);

/**
 * @brief Checks if two hitboxs collided
 * 
 * @param hitboxA Hitbox 1
 * @param hitboxB Hitbox 2
 * @return true Hitbox 1 and 2 collided
 * @return false Hitbox 1 and 2 did not collide
 */
bool check_hitbox_collision(Hitbox hitboxA, Hitbox hitboxB);


typedef struct {
  uint16_t x, y;   /**< button position */
  Bitmap* bitmap;  /**< bitmap being used */
  Bitmap* bitmapIdle;  /**< bitmap used when idle */
  Bitmap* bitmapHighlighted; /**< bitmap used when cursor hovers the button */ 
  Layer* layer; /**< Layer where the button is on */
  bool singleState; /**< Set to false only if the button can both be "on" or "off" */
  bool isHighlighted; /**< Set to true if the cursor is hovering */
  bool isPressed; /**< Set to true if the button is pressed */
} Button;

/**
 * @brief Creates a button
 * 
 * @param x X position
 * @param y Y position
 * @param layer Layer to place the button on
 * @param bitmapIdle Bitmap when idle
 * @param bitmapHighlighted Bitmap when highlighted
 * @return Button* The created button
 */
Button* create_button(uint16_t x, uint16_t y, Layer* layer, Bitmap* bitmapIdle, Bitmap* bitmapHighlighted);

/**
 * @brief Draws a button on the screen, highlighting it if cursor is on the button
 * 
 * @param cursor Cursor being used
 * @param button Button to be drawn
 */
void draw_button(Sprite* cursor, Button* button);

/**
 * @brief Draws a number of buttons on the screen.
 * 
 * @param cursor Cursor being used.
 * @param buttons Array of buttons to be drawn
 * @param num_buttons Size of the array
 */
void draw_buttons(Sprite* cursor, Button* buttons[], uint8_t num_buttons);

/**
 * @brief Unpresses and unhighlights all the buttons 
 * 
 * @param buttons Array of buttons to be drawn
 * @param num_buttons Size of the array
 */
void disable_buttons(Button* buttons[], uint8_t num_buttons);

/**
 * @brief Checks if a cursor is on a button
 * 
 * @param cursor Cursor being used
 * @param button Button to be checked
 * @return true The cursor is on the button
 * @return false The cursor is not on the button
 */
bool is_on_button(Sprite* cursor, Button* button);

/**
 * @brief Highlights a button, updating it.
 * 
 * @param button Button to be highlighted
 */
void highlight_button(Button* button);

/**
 * @brief Unghlights a button, updating it.
 * 
 * @param button Button to be unhighlighted
 */
void unhighlight_button(Button* button);

/**
 * @brief Presses a button, updating it.
 * 
 * @param button Button to be pressed
 */
void press_button(Button* button);

/**
 * @brief Releases a button, updating it.
 * 
 * @param button Button to be released
 */
void unpress_button(Button* button);

/**
 * @brief If there was a mouse event, checks the new cursor position and whether it has clicked on a button. If so, it returns its index from the provided array.
 * If the pressed button is not singleState, all the other buttons are released.
 * 
 * @param event Event to be read
 * @param cursor Cursor being used
 * @param buttons Array of buttons to be drawn
 * @param num_buttons Size of the array
 * @return int Index of the pressed button. Returns -1 if no buttons were pressed.
 */
int checkButtonPress(Event_t event, Sprite* cursor, Button* buttons[], uint8_t num_buttons);

/**
 * @brief Same as a regular sprite, except GREEN is always replaced by its color when drawn, cannot be moved, and is not necessarily drawn on top of all the other elements.
 * (It's on a layer)
 * 
 */
typedef struct {
  uint16_t x, y; 
  Bitmap* bitmap;
  Layer* layer;  
  uint32_t color;
} IdleSprite;

/**
 * @brief Create an idle sprite.
 * 
 * @param x X position
 * @param y Y position
 * @param layer Respective layer
 * @param bitmap Sprite bitmap
 * @param color Color to replace GREEN on the BMP
 * @return IdleSprite* Returns the IdleSprite object
 */
IdleSprite* create_idle_sprite(uint16_t x, uint16_t y, Layer* layer, Bitmap* bitmap, uint32_t color);

/**
 * @brief Destroys an IdleSprite object, freeing all used memory.
 * 
 * @param idlesprite Sprite to be destroyed.
 */
void destroy_idle_sprite(IdleSprite* idlesprite);

/**
 * @brief Draws an IdleSprite on its layer, replacing all GREEN in its BMP with its color attribute.
 * 
 * @param idlesprite 
 */
void draw_idle_sprite(IdleSprite* idlesprite);

/**
 * @brief Changes the IdleSprite's color.
 * 
 * @param idlesprite Object to be changed
 * @param color New color
 */
void idle_sprite_change_color(IdleSprite* idlesprite, uint32_t color);

typedef struct {
  Layer* layer;
  uint16_t x, y;
  uint16_t height, width;
  uint16_t handle_pos;  /**< absolute X position of the Slider's handle */
  uint32_t color;       /**< Color that replaces GREEN for the "inactive" part of the slider */
  uint32_t color_active; /**< Color that replaces GREEN for the "active" part of the slider */
  bool isPressed; /**< Set to true if slider is pressed */
} Slider;

/**
 * @brief Creates a slider object.
 * 
 * @param x X position
 * @param y Y position
 * @param perc_filled How much of the slider is "filled" with active color
 * @param layer Layer to place it on
 * @param color color attribute 
 * @param color_active color_active attribute 
 * @return Slider* 
 */
Slider* create_slider(uint16_t x, uint16_t y, double perc_filled, Layer* layer, uint32_t color, uint32_t color_active);

/**
 * @brief Draws a slider on its layer.
 * 
 * @param slider Slider to be drawn.
 */
void draw_slider(Slider* slider);

/**
 * @brief Moves the sliders handle, and updates its sprite
 *  
 * @param slider Slider to alter
 * @param pos New X position of the handle
 */
void slider_move_handle(Slider* slider, uint16_t pos);

/**
 * @brief Checks if a cursor (sprite) is on a slider.
 * 
 * @param cursor Cursor being used
 * @param slider Slider to be checked
 * @return true The cursor is on the slider
 * @return false The cursor is not on the slider
 */
bool is_on_slider(Sprite* cursor, Slider* slider);

/**
 * @brief Calculates how much of the slider is "filled" (how much to the right the handle is)
 * 
 * @param slider Slider to be checked
 * @return double Returns the calculated percentage
 */
double slider_get_perc(Slider* slider);

/**
 * @brief Updates a slider, based on an event and a cursor. 
 * If the cursor presses the LB on the slider, or if it's holding the left button while the slider still wasn't released, it moves the handle to the cursor's x position.
 * 
 * @param event Event to be read.
 * @param cursor Cursor being used
 * @param slider Slider to update
 * @return true The slider's handle was moved
 * @return false The slider's handle was not moved
 */
bool update_slider(Event_t event, Sprite* cursor, Slider* slider);



