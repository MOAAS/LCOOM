#pragma once

#include "video.h"

/** @defgroup bitmap bitmap
 * @{
 *
 * Functions used to create and manipulate BMP files.
 */

#pragma pack(push, 1)

/**
 * @brief Represents a bitmap file header
 * 
 */
typedef struct {
    unsigned short int type;        ///< specifies the file type
    unsigned int size;              ///< specifies the size in bytes of the bitmap file
    unsigned short reserved1;       ///< reserved; must be 0
    unsigned short int reserved2;   ///< reserved; must be 0
    unsigned int offset;            ///< specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;

#pragma pack(pop)

#pragma pack(push, 1)

/**
 * @brief Represents a bitmap info header
 * 
 */
typedef struct {
    unsigned int size; ///< specifies the number of bytes required by the struct
    int width; ///< specifies width in pixels
    int height; ///< specifies height in pixels
    unsigned short planes; ///< specifies the number of color planes, must be 1
    unsigned short bitsPerPixel; ///< specifies the number of bit per pixel
    unsigned int compression; ///< specifies the type of compression
    unsigned int imageSize; ///< size of image in bytes
    int xResolution; ///< number of pixels per meter in x axis
    int yResolution; ///< number of pixels per meter in y axis
    unsigned int nColors; ///< number of colors used by the bitmap
    unsigned int importantColors; ///< number of colors that are important
} BitmapInfoHeader;

#pragma pack(pop)

/**
 * @brief Represents a Bitmap
 * 
 */
typedef struct {
    BitmapInfoHeader bitmapInfoHeader;  ///< Bitmap info header
    char* bitmapData;                   ///< Pointer to the pixel map buffer
    unsigned int bytes_per_pixel;       ///< Bytes per pixel used by the bitmap
    unsigned int padding;               ///< Bitmap padding per row
    unsigned int actual_bytes_per_row;  ///< Number of bytes per row (including padding)
} Bitmap;

/**
 * @brief Saves a bitmap in a .bmp file.
 * 
 * @param path Full path of the saved file.
 * @param width Bitmap width, in pixels
 * @param height Bitmap height, in pixels
 * @param address Pointer to a pixel map.
 */
void saveBitmap(char* path, unsigned int width, unsigned int height, char* address);

/**
 * @brief Loads a .bmp file.
 * 
 * @param folderPath Full folder path.
 * @param filename File name, including .bmp
 * @return Bitmap* Pointer to the respective bitmap structure
 */
Bitmap* loadBitmap(const char* folderPath, const char* filename);

/**
 * @brief Copies part of a bitmap from a given bitmap to another bitmap
 * 
 * @param bitmap "Bigger" bitmap to read
 * @param x X position to start copying the contents from
 * @param y Y position to start copying the contents from
 * @param width Width of the area to copy
 * @param height Height of the area to copy
 * @return Bitmap* Returns a pointer to the cropped bitmap
 */
Bitmap* loadBitmapSection(Bitmap* bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

/**
 * @brief Resizes a bitmap by a given factor.
 * 
 * @param bitmap Bitmap to resize
 * @param factor Factor (Must be an integer :( ) 
 * @return Bitmap* Returns a pointer to the resized bitmap
 */
Bitmap* resizeBitmap(Bitmap* bitmap, uint16_t factor);

/**
 * @brief Draws a bitmap on the screen.
 * 
 * @param bmp Pointer to the Bitmap that will be drawn
 * @param x X position of the upper left corner
 * @param y Y position of the upper left corner
 */
void draw_bitmap(Bitmap* bmp, int x, int y);

/**
 * @brief Draws a bitmap on the screen, replacing every GREEN pixel with another color
 * 
 * @param bmp Pointer to the Bitmap that will be drawn
 * @param x X position of the upper left corner
 * @param y Y position of the upper left corner
 * @param new_color Color to replace GREEN
 */
void draw_bitmap_color(Bitmap* bmp, int x, int y, uint32_t new_color);

/**
 * @brief Draws a bitmap on the screen, allowing the caller to use transparency.
 * The color BLACK ignores transparency values.
 * 
 * @param bmp Bitmap to draw
 * @param x X position of the upper left corner
 * @param y Y position of the upper left corner
 * @param alpha Transparency (1 is opaque, 0 is invisible)
 */
void draw_bitmap_transp(Bitmap* bmp, int x, int y, double alpha);

/**
 * @brief Draws a bitmap on the screen, rotated by an angle.
 * 
 * @param bmp Pointer to the Bitmap that will be drawn
 * @param x X position of the upper left corner
 * @param y Y position of the upper left corner
 * @param angle Angle rotation
 */
void draw_bitmap_rotate(Bitmap* bmp, int x, int y, double angle);


/**
 * @brief Gets the color of a certain bitmap pixel
 * 
 * @param bmp Pointer to a Bitmap to be read
 * @param x X position of the pixel, where (0,0) is the upper left corner
 * @param y Y position of the pixel, where (0,0) is the upper left corner
 * @return uint32_t Respective color
 */
uint32_t get_bitmap_color(Bitmap* bmp, uint16_t x, uint16_t y);

/**
 * @brief Deletes a bitmap, freeing the used memory
 * 
 * @param bmp Bitmap to be deleted
 */
void deleteBitmap(Bitmap* bmp);

/**
 * @brief Checks if a color is transparent (MAGENTA)
 * 
 * @param color Color to compare
 * @return true The color is transparent (MAGENTA)
 * @return false The color is transparent (MAGENTA)
 */
bool is_transparent(uint32_t color);

