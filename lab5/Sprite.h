
#define IGNORED_COLOR 0

typedef struct {
  int x,y;             /**< current sprite position */
  int width, height;   /**< sprite dimensions */
  char *map;           /**< the sprite pixmap (use read_xpm()) */
} Sprite;

/** Creates a new sprite with pixmap "pic", with specified position (within the screen limits).
* Does not draw the sprite on the screen
* Returns NULL on invalid pixmap.
*/
Sprite *create_sprite(const char *pic[], int x, int y);

/** The "fig" sprite is erased from memory whose address is "base"
 * and used resources released.
 */
void destroy_sprite(Sprite *fig);

void draw_sprite(Sprite *sp);
void erase_sprite(Sprite *sp, uint32_t background_color);
void move_sprite(Sprite *sp, int16_t delX, int16_t delY, uint32_t background_color);
