#pragma once
#include "vbe.h"

#define BLACK   0x000000
#define RED     0xFF0000
#define GREEN   0x00FF00
#define BLUE    0x0000FF

#define CYAN    0x00FFFF
#define YELLOW  0xFFFF00
#define MAGENTA 0xFF00FF

#define WHITE   0xFFFFFF

struct color {
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

/**
 * @brief Initializes MINIX in graphics mode
 * 
 * @param mode Mode to be set
 * @return void* Pointer to the video memory on success, NULL otherwise.
 */
void* video_init(uint16_t mode);

/**
 * @brief Gets the color of a pixel
 * 
 * @param x X position
 * @param y Y position
 * @return uint32_t Pixel color
 */
uint32_t vg_get_pixel(uint16_t x, uint16_t y);

/**
 * @brief Draws a pixel on the screen
 * 
 * @param x X position
 * @param y Y position
 * @param color Pixel color
 */
void vg_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Given a base address, calculates the address of a specific position
 * 
 * @param init Base address
 * @param x X position
 * @param y Y position
 * @param width Pixels per row
 * @return char* Returns the corresponding address
 */
char* calc_address(char* init, int16_t x, int16_t y, uint16_t width);

/**
 * @brief Given a pixel address, inserts a color in it
 * 
 * @param address Pointer to a pixel
 * @param color Color to insert
 */
void vg_insert(char* address, uint32_t color);

/**
 * @brief Given a pixel address, returns the color associated with it.
 * 
 * @param address Pointer to a pixel
 * @return uint32_t Returns the color
 */
uint32_t vg_retrieve(char* address);

/**
 * @brief Draws an image on the screen
 * 
 * @param map Color map
 * @param width Image width
 * @param height Image height
 * @param x X position of the uppermost corner
 * @param y Y position of the uppermost corner
 */
void vg_draw_image(char* map, uint16_t width, uint16_t height, uint16_t x, uint16_t y);

/**
 * @brief Moves a portion of the screen in an horizontal direction
 * 
 * @param y Y position from which to move the screen
 * @param height Height of the portion to move
 * @param delx X distance to move the screen
 * @param fill_color Color to fill the "empty" area with
 */
void vg_move(uint16_t y, uint16_t height, int16_t delx, uint32_t fill_color);

/**
 * @brief Given the RGB components, calculates the color value
 * 
 * @param R Red component
 * @param G Green component
 * @param B Blue component
 * @return uint32_t 
 */
uint32_t vg_compose_RGB(uint8_t R, uint8_t G, uint8_t B);

/**
 * @brief Gets the R,G and B components of a color
 * 
 * @param RGB Direct color
 * @return struct color R, G and B components
 */
struct color vg_decompose_RGB(uint32_t RGB);

/**
 * @return unsigned int Returns the number of pixels per row
 */
unsigned int vg_get_hres();

/**
 * @return unsigned int Returns the number of pixels per column
 */
unsigned int vg_get_vres();

/**
 * @return unsigned int Returns the bytes per pixel
 */
unsigned int vg_get_bytes_pp();

/**
 * @return unsigned int Returns the bits per pixel
 */
unsigned int vg_get_bits_pp();

/**
 * @return char* Returns the address to the Video memory
 */
char* vg_get_video_mem();

/**
 * @brief Checks if an indexed mode is being used
 * 
 * @return true The current mode is indexed
 * @return false The current mode is not indexed
 */
bool vg_mode_is_indexed();

/**
 * @brief Gets the currently used VBE mode.
 * 
 * @return vbe_mode_info_t Returns the VBE mode structure.
 */
vbe_mode_info_t vg_get_mode();

/**
 * @brief Draws a horizontal line on the screen
 * 
 * @param x X position of the leftmost end
 * @param y Y position of the leftmost end
 * @param len Line length
 * @param color Line color
 */
void video_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a vertical line on the screen
 * 
 * @param x X position of the uppermost end
 * @param y Y position of the uppermost end
 * @param len Line length
 * @param color Line color
 */
void video_draw_vline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle on the screen
 * 
 * @param x X Position of the upper left corner
 * @param y Y Position of the upper left corner
 * @param width Rectangle width
 * @param height Rectangle height
 * @param color Rectangle color
 */
void video_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Fills the screen with a specific color
 * 
 * @param color Color to fill the screen with
 */
void vg_set_background(uint32_t color);
