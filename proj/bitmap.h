#pragma once

#include "video.h"

unsigned char * db;

typedef enum {
    ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
} Alignment;


#pragma pack(push, 1)

typedef struct {
    unsigned short int type;  //specifies the file type
    unsigned int size;  //specifies the size in bytes of the bitmap file
    unsigned short reserved1;  //reserved; must be 0
    unsigned short int reserved2;  //reserved; must be 0
    unsigned int offset;  //specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} BitmapFileHeader;

#pragma pack(pop)

#pragma pack(push, 1)

typedef struct {
    unsigned int size; // specifies the number of bytes required by the struct
    int width; // specifies width in pixels
    int height; // specifies height in pixels
    unsigned short planes; // specifies the number of color planes, must be 1
    unsigned short bitsPerPixel; // specifies the number of bit per pixel
    unsigned int compression; // specifies the type of compression
    unsigned int imageSize; // size of image in bytes
    int xResolution; // number of pixels per meter in x axis
    int yResolution; // number of pixels per meter in y axis
    unsigned int nColors; // number of colors used by the bitmap
    unsigned int importantColors; // number of colors that are important
} BitmapInfoHeader;

#pragma pack(pop)

/// Represents a Bitmap
typedef struct {
    BitmapInfoHeader bitmapInfoHeader;
    unsigned char* bitmapData;
    unsigned int bytes_per_pixel;
    unsigned int padding;
    unsigned int actual_width;
} Bitmap;

Bitmap* loadBitmap(const char* filename);

Bitmap* loadBitmapSection(Bitmap* bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height);

Bitmap* resizeBitmap(Bitmap* bitmap, uint16_t factor);

void draw_bitmap(Bitmap* bmp, int x, int y, Alignment alignment);

uint32_t get_bitmap_color(Bitmap* bmp, uint16_t x, uint16_t y);

void deleteBitmap(Bitmap* bmp);

char *double_buffering();

bool is_transparent(uint32_t color);

