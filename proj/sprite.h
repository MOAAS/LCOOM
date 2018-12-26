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
void draw_sprite(Sprite *sp);
void draw_sprite_color(Sprite *sp);


void erase_sprite(Sprite *sp);
void move_sprite(Sprite *sp, int16_t delX, int16_t delY);
void sprite_set_color(Sprite *sp, uint32_t color);

uint16_t cursor_get_xf(Sprite* cursor, int16_t delX);
uint16_t cursor_get_yf(Sprite* cursor, int16_t delY);
void update_cursor(Sprite *cursor, Event_t event);
void cursor_change_bmp(Sprite *cursor, Bitmap* bitmap);

typedef struct {
  int x1, x2, y1, y2;
  int width, height;
} Hitbox;

void make_hitbox(Hitbox* hitbox, int16_t x1, int16_t x2, int16_t y1, int16_t y2);
void move_hitbox(Hitbox* hitbox, int16_t delx, int16_t dely);
bool check_hitbox_collision(Hitbox hitboxA, Hitbox hitboxB);


typedef struct {
  uint16_t x, y;
  Bitmap* bitmap;
  Bitmap* bitmapIdle;
  Bitmap* bitmapHighlighted;
  Layer* layer;
  bool singleState;
  bool isHighlighted;
  bool isPressed;
} Button;

Button* create_button(uint16_t x, uint16_t y, Layer* layer, Bitmap* bitmapIdle, Bitmap* bitmapHighlighted);
void draw_button(Sprite* cursor, Button* button);
void draw_buttons(Sprite* cursor, Button* buttons[], uint8_t num_buttons);

void disable_buttons(Button* buttons[], uint8_t num_buttons);

bool is_on_button(Sprite* cursor, Button* button);
void highlight_button(Button* button);
void unhighlight_button(Button* button);

void press_button(Button* button);
void unpress_button(Button* button);

int checkButtonPress(Event_t event, Sprite* cursor, Button* buttons[], uint8_t num_buttons);

typedef struct {
  uint16_t x, y;
  Bitmap* bitmap;
  Layer* layer;
} NNY;

NNY* create_NNY(uint16_t x, uint16_t y, Layer* layer, Bitmap* bitmap);
void draw_nny(NNY* nny);
void nny_change_color(NNY* nny, uint32_t color);

