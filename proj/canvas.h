#pragma once
#include "layer.h"
#include "video.h"

/**
 * @brief Represents a canvas to draw on.
 * 
 */
typedef struct {
    uint16_t xMin, xMax; 
    uint16_t yMin, yMax;
    uint16_t width;
    uint16_t height;
    Layer* layer;
} Canvas;

/**
 * @brief Creates a canvas and draws it on the screen, with a black outline.
 * 
 * @param layer Layer to draw the canvas on.
 * @param xMin X position of the upper left corner.
 * @param yMin Y position of the upper left corner.
 * @param xMax X position of the lower right corner.
 * @param yMax Y position of the lower right corner.
 * @param color Color to fill the canvas with
 */
void create_canvas(Layer* layer, uint16_t xMin, uint16_t yMin, uint16_t xMax, uint16_t yMax, uint32_t color);

/**
 * @brief Frees the memory used by the canvas, so that a new one can be created later.
 * 
 */
void destroy_canvas();

/**
 * @brief Draws a line on the canvas, updating the screen
 * 
 * @param x0 X position of the left most end
 * @param y0 Y position of the leftmost end
 * @param xf X position of the rightmost end
 * @param yf Y position of the rightmost end
 * @param color Color to draw the line with
 * @param thickness Line thickness
 */
void canvas_draw_line(uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf, uint32_t color, uint16_t thickness);

/**
 * @brief Draws a line on the canvas, updating the screen. Uses a Square pattern.
 * 
 * @param x0 X position of the left most end
 * @param y0 Y position of the leftmost end
 * @param xf X position of the rightmost end
 * @param yf Y position of the rightmost end
 * @param color Color to draw the line with
 * @param thickness Line thickness
 */
void canvas_draw_line2(uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf, uint32_t color, uint16_t thickness);

/**
 * @brief Draws a filled circle on the canvas.
 * 
 * @param x X position of the center
 * @param y Y position of the center
 * @param radius Circle radius
 * @param color Circle color
 */
void canvas_draw_circle(uint16_t x, uint16_t y, uint16_t radius, uint32_t color);

/**
 * @brief Draws a circumference on the canvas.
 * 
 * @param x X position of the center
 * @param y Y position of the center
 * @param radius circumference radius
 * @param thickness circumference thickness
 * @param color circumference color
 */
void canvas_draw_circumference(uint16_t x, uint16_t y, uint16_t radius, uint8_t thickness, uint32_t color);


/**
 * @brief Draws a pixel on the canvas (assuming it's within limits)
 * 
 * @param x X position of the pixel
 * @param y Y position of the pixel
 * @param color Pixel color
 */
void canvas_draw_pixel(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line on the canvas
 * 
 * @param x X position of the leftmost end
 * @param y Y position of the leftmost end
 * @param len Line length
 * @param color Line color
 */
void canvas_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a vertical line on the canvas
 * 
 * @param x X position of the uppermost end
 * @param y Y position of the uppermost end
 * @param len Line length
 * @param color Line color
 */
void canvas_draw_vline(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle on the canvas
 * 
 * @param x X Position of the upper left corner
 * @param y Y Position of the upper left corner
 * @param width Rectangle width
 * @param height Rectangle height
 * @param color Rectangle color
 */
void canvas_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Draws a rectangle outline on the canvas
 * 
 * @param x X Position of the upper left corner
 * @param y Y Position of the upper left corner
 * @param width Rectangle width
 * @param height Rectangle height
 * @param thickness Outline thickness 
 * @param color Rectangle color
 */
void canvas_draw_rectangle_outline(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t thickness, uint32_t color);

/**
 * @brief Draws a square on the canvas
 * 
 * @param x X Position of the upper left corner
 * @param y Y Position of the upper left corner
 * @param side_len Side legth
 * @param color Square color
 */
void canvas_draw_square(uint16_t x, uint16_t y, uint16_t side_len, uint32_t color);

/**
 * @brief Draws an image on the canvas.
 * 
 * @param bitmap Bitmap to be drawn
 */
void canvas_draw_image(Bitmap* bitmap);

/**
 * @brief Sets the canvas color, filling it.
 * 
 * @param color Color to fill with
 */
void canvas_set_color(uint32_t color);

/**
 * @brief Sets the canvas outline (thickness 5)
 * 
 * @param color Color to draw the outline with
 */
void canvas_set_outline(uint32_t color);

/**
 * @brief Following a rainbow gradient, gets the next color in the gradient.
 * 
 * @param old_color Previous color
 * @return uint32_t Returns the following color.
 */
uint32_t rainbow(uint32_t old_color);

/**
 * @brief Flood fills the canvas.
 * 
 * @param x X position to flood fill
 * @param y Y position to flood fill
 * @param cor_balde Color to draw with
 */
void bucket_tool(uint16_t x, uint16_t y, uint32_t cor_balde);

/**
 * @brief Checks if a position is inside the canvas
 * 
 * @param x X position to check
 * @param y Y position to check
 * @return true The position is inside the canvas
 * @return false The position is outside the canvas
 */
bool is_inside_canvas(uint16_t x, uint16_t y);

/**
 * @return char* Returns a pixel map of the canvas. (allocated memory)
 */
char* canvas_get_map();

/**
 * @return int Returns the canvas height.
 */
int canvas_get_height();

/**
 * @return int Returns the canvas width.
 */
int canvas_get_width();

typedef enum {
    NoShape = 0,
    Circle = 1,
    Circumference = 2,
    Rectangle = 3,
} Shape;

/**
 * @brief Draws a shape on the canvas, based on two mouse clicks
 * 
 * @param shape Shape to draw (Enum)
 * @param click1_x X position of the first mouse click
 * @param click1_y Y position of the first mouse click
 * @param click2_x X position of the second mouse click
 * @param click2_y Y position of the second mouse click
 * @param color Color of the shape
 * @param thickness Shape thickness (if applied)
 */
void canvas_draw_shape(Shape shape, uint16_t click1_x, uint16_t click1_y, uint16_t click2_x, uint16_t click2_y,  uint32_t color, uint16_t thickness);

/**
 * @brief Stores the current canvas pixel map in a buffer (for the next undo action). Sets an internal isDrawing flag.
 * 
 */
void canvas_save_drawing();

/**
 * @brief Saves drawing for the next undo action, only if the cursor position is inside the canvas and the isDrawing flag is not set.
 * 
 * @param x X position of the cursor
 * @param y Y position of the cursor
 */
void canvas_start_drawing(uint16_t x, uint16_t y);

/**
 * @brief Unsets the isDrawing flag.
 * 
 */
void canvas_stop_drawing();

/**
 * @brief Edits the canvas to the last saved pixel map.
 * 
 */
void canvas_undo();

/**
 * @brief Updates the canvas with its layer contents.
 * 
 */
void canvas_update();
