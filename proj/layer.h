#pragma once

#include "video.h"
#include "bitmap.h"


typedef struct {
    char* map;          ///< Pixel map, where the first element is the upper left corner
    uint16_t x;         ///< X position of the upper left corner   
    uint16_t y;         ///< Y position of the upper left corner
    uint16_t xMax;      ///< x + width
    uint16_t yMax;      ///< y + height
    uint16_t width;     ///< Number of pixels per row
    uint16_t height;    ///< Number of pixels per column
    uint8_t layer_no;   ///< Position in the layer array. Lower layers go under.
} Layer;

/**
 * @brief Creates a layer.
 * 
 * @param x X position of the upper left corner  
 * @param y Y position of the upper left corner
 * @param width Number of pixels per row
 * @param height Number of pixels per column
 * @return Layer* The created layer.
 */
Layer* create_layer(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Destroys the layer, erasing all the pixels from the screen (draws the ones under it), and deleteing it from the layer array.
 * 
 * @param layer Layer to be destroyed.
 */
void destroy_layer(Layer* layer);

/**
 * @return Layer* Returns the topmost layer.
 */
Layer* get_highest_layer();

/**
 * @brief Draws a pixel on a layer. If that pixel has no other layer above, updates the screen as well. 
 * 
 * @param layer Layer to draw on.
 * @param x X position to draw on
 * @param y Y position to draw on
 * @param color Color to draw with
 */
void draw_on_layer(Layer* layer, uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Checks if layer is the highest on that position.
 * 
 * @param layer Layer to check
 * @param x X position to check
 * @param y Y position to check
 * @return true Layer is the highest on that position
 * @return false Layer is not the highest on that position
 */
bool is_top_layer(Layer* layer, uint16_t x, uint16_t y);

/**
 * @brief Sets a pixel in the layer map (does not update the screen)
 * 
 * @param layer Layer to set the pixel
 * @param x X position to draw on
 * @param y Y position to draw on
 * @param color Color of the pixel (Value to insert)
 */
void layer_set_pixel(Layer* layer, uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Changes a color of a layer position to the one under it.
 * 
 * @param layer Layer to draw on
 * @param x X position
 * @param y Y position
 */
void layer_erase_pixel(Layer* layer, uint16_t x, uint16_t y);

/**
 * @brief Gets the color of a layer pixel.
 * 
 * @param layer Layer to check.
 * @param x X position of the pixel.
 * @param y Y position of the pixel.
 * @return uint32_t Returns the color
 */
uint32_t layer_get_pixel(Layer* layer, uint16_t x, uint16_t y);

/**
 * @brief Gets the color of a layer pixel. If out of bounds, checks the one under it until it finds a valid layer.
 * 
 * @param layer First layer to check
 * @param x X position of the pixel
 * @param y Y position of the pixel
 * @return uint32_t Returns the color
 */
uint32_t layer_get_pixel_under(Layer* layer, uint16_t x, uint16_t y);

/**
 * @brief Checks if a position is inside a layer.
 * 
 * @param layer Layer to check
 * @param x X position
 * @param y Y position
 * @return true The position is inside the layer
 * @return false The position is not inside the layer
 */
bool is_within_bounds(Layer* layer, uint16_t x, uint16_t y);

/**
 * @brief Draws a bitmap on a layer.
 * 
 * @param layer Layer to draw on
 * @param bmp Bitmap to draw
 * @param x X position of the upper left corner
 * @param y Y position of the upper left corner
 */
void layer_draw_image(Layer* layer, Bitmap* bmp, int x, int y);

/**
 * @brief Draws a bitmap on a layer, replacing all the GREEN with another color.
 * 
 * @param layer Layer to draw on
 * @param bmp Bitmap to draw
 * @param x X position of the upper left corner
 * @param y Y position of the upper left corner
 * @param new_color Color to replace GREEN with.
 */
void layer_draw_image_color(Layer* layer, Bitmap* bmp, int x, int y, uint32_t new_color);




