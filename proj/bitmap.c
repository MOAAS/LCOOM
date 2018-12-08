#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "bitmap.h"
#include "video.h"



Bitmap* loadBitmap(const char* filename) {
    // allocating necessary size
    Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));
    // open filename in read binary mode
    FILE *filePtr;
    filePtr = fopen(filename, "rb");
    if (filePtr == NULL) {
        printf("File not found! %s\n", filename);
        return NULL;
    }
    // read the bitmap file header
    BitmapFileHeader bitmapFileHeader;
    fread(&bitmapFileHeader, sizeof(BitmapFileHeader), 1, filePtr);

    // verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.type != 0x4D42) {
        fclose(filePtr);
        printf("Error reading file: %s! Not a BMP!\n", filename);
        return NULL;
    }

    // read the bitmap info header
    BitmapInfoHeader bitmapInfoHeader;
    fread(&bitmapInfoHeader, sizeof(BitmapInfoHeader), 1, filePtr);

    // move file pointer to the begining of bitmap data
    fseek(filePtr, bitmapFileHeader.offset, SEEK_SET);

    unsigned int bytes_per_pixel = ceil(bitmapInfoHeader.bitsPerPixel / 8.0);

    // calculate the padding , width and size
    bmp->padding = (4 - (bitmapInfoHeader.width * bytes_per_pixel) % 4) % 4;
    bmp->actual_width = bitmapInfoHeader.width + (bmp->padding != 0); // + 1 if padding != 0
    bitmapInfoHeader.imageSize = bitmapInfoHeader.height * (bmp->actual_width) * bytes_per_pixel; 
    unsigned char* bitmapImage = (unsigned char*) malloc(bitmapInfoHeader.imageSize);
    // verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        printf("Error reading file: %s! Bad allocation.\n", filename);
        return NULL;
    }
    // read in the bitmap image data
    fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);
    // make sure bitmap image data was read
    if (bitmapImage == NULL) {
        fclose(filePtr);
        printf("Error reading file: %s! Bad reading.\n", filename);
        return NULL;
    }

    /* Nao necessario, mas pode ser util mais tarde...
    unsigned char tempRGB;  //our swap variable
    //swap the r and b values to get RGB (bitmap is BGR)
    for (unsigned int imageIdx = 0; imageIdx < bitmapInfoHeader.imageSize; imageIdx += 3) { // fixed semicolon
        tempRGB = bitmapImage[imageIdx];
        bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
        bitmapImage[imageIdx + 2] = tempRGB;
    }
    */

    // close file and return bitmap
    fclose(filePtr);

    bmp->bitmapData = bitmapImage;
    bmp->bitmapInfoHeader = bitmapInfoHeader;
    bmp->bytes_per_pixel = bytes_per_pixel;
    return bmp;
}

Bitmap* loadBitmapSection(Bitmap* bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    Bitmap* section = malloc(sizeof(Bitmap));
    *section = *bitmap;
    uint8_t bytes_per_pixel = bitmap->bytes_per_pixel;
    uint32_t interval = (bitmap->actual_width - width) * bytes_per_pixel;

    section->padding = (4 - (width * bytes_per_pixel) % 4) % 4;
    section->bitmapInfoHeader.width = width;
    section->bitmapInfoHeader.height = height;
    section->actual_width = width + (section->padding != 0); // + 1 if padding != 0
    section->bitmapInfoHeader.imageSize = height * (section->actual_width) * bytes_per_pixel; 
    section->bitmapData  = (unsigned char*) malloc(section->bitmapInfoHeader.imageSize);

    unsigned char* original = bitmap->bitmapData + ((bitmap->bitmapInfoHeader.width + bitmap->padding) * y + x) * bytes_per_pixel;
    unsigned char* new = section->bitmapData;
    for (int i = 0; i < height; i++) { // y
    	for(int j = 0; j < width; j++) { // x
            uint32_t color = *(uint32_t*)original;
            for (size_t offset = 0; offset < bytes_per_pixel; offset++, color = color >> 8) 
                *(new + offset) = color;
            original += bytes_per_pixel;
            new += bytes_per_pixel;
        }
        original += interval;
        new += section->padding;
    }
    return section;
}

Bitmap* resizeBitmap(Bitmap* bitmap, uint16_t factor) {
    if (factor == 1)
        return bitmap;
    Bitmap* resized = malloc(sizeof(Bitmap));
    *resized = *bitmap;
    uint16_t width = bitmap->bitmapInfoHeader.width * factor;
    uint16_t height = bitmap->bitmapInfoHeader.height * factor;

    uint8_t bytes_per_pixel = bitmap->bytes_per_pixel;
    resized->padding = (4 - (width * bytes_per_pixel) % 4) % 4;
    resized->bitmapInfoHeader.width = width;
    resized->bitmapInfoHeader.height = height;
    resized->actual_width = width + (resized->padding != 0); // + 1 if padding != 0
    resized->bitmapInfoHeader.imageSize = height * (resized->actual_width) * bytes_per_pixel; 
    resized->bitmapData  = (unsigned char*) malloc(resized->bitmapInfoHeader.imageSize);
    unsigned char* original = bitmap->bitmapData;
    unsigned char* new = resized->bitmapData;
    for (int i = 0; i < height; i++) { // y
    	for(int j = 0; j < width; j++) { // x
            uint32_t color = *(uint32_t*)original;
            for (size_t offset = 0; offset < bytes_per_pixel; offset++, color = color >> 8) 
                *(new + offset) = color;
            if (j % factor == factor - 1)
                original += bytes_per_pixel;
            new += bytes_per_pixel;
        }
        if (i % factor != factor - 1)
            original -= bitmap->bitmapInfoHeader.width * bytes_per_pixel;
        original += bitmap->padding;
        new += resized->padding;
    }
    return resized;
}

void saveBitmap(char* filename, unsigned int width, unsigned int height, char* address) {
    FILE* file = fopen(filename, "w");
	static unsigned char bmp_header[54] = {66,77,0,0,0,0,0,0,0,0,54,0,0,0,40,0,0,0,0,0,0,0,0,0,0,0,1,0,24}; //rest is zeroes
	unsigned int bytes_per_row = width * vg_get_bytes_pp();
	unsigned int padding = (4 - (bytes_per_row % 4)) % 4;
	unsigned int* sizeOfFileEntry = (unsigned int*)&bmp_header[2];
	*sizeOfFileEntry = 54 + (bytes_per_row + padding) * height;  
	unsigned int* widthEntry = (unsigned int*)&bmp_header[18];    
	*widthEntry = width;
	unsigned int* heightEntry = (unsigned int*)&bmp_header[22];    
	*heightEntry = height;
    fwrite(bmp_header, sizeof(char), 54, file);
	static unsigned char zeroes[3] = {0,0,0}; // padding
    uint8_t bytes_per_pixel = vg_get_bytes_pp();
    address += (height - 1) * bytes_per_row;
    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            fwrite(address, 1, bytes_per_pixel, file);
            address += bytes_per_pixel;
        }
        address -= 2 * bytes_per_row;
        fwrite(zeroes, sizeof(char), padding, file);
    }
	fclose(file);

}

void draw_bitmap(Bitmap* bmp, int x, int y, Alignment alignment) {
    if (bmp == NULL)
        return;
    int width = bmp->bitmapInfoHeader.width;
    int height = bmp->bitmapInfoHeader.height;
    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;
    uint16_t xCoord = x;
    uint16_t yCoord = y + height - 1;  
    unsigned char* img = bmp->bitmapData;
    for (int i = 0; i < height; i++, yCoord--) { // y
        xCoord = x;
    	for(int j = 0; j < width; j++, xCoord++) { // x
            uint32_t color = *(uint32_t*)img;
            if (!is_transparent(color))
                vg_draw_pixel(xCoord, yCoord, color);
            img += bmp->bytes_per_pixel;
        }
        img += bmp->padding; // * bmp->bytes_per_pixel;
    }
}

void draw_bitmap_color(Bitmap* bmp, int x, int y, Alignment alignment , uint32_t new_color) {
    if (bmp == NULL)
        return;
    int width = bmp->bitmapInfoHeader.width;
    int height = bmp->bitmapInfoHeader.height;
    if (alignment == ALIGN_CENTER)
        x -= width / 2;
    else if (alignment == ALIGN_RIGHT)
        x -= width;
    uint16_t xCoord = x;
    uint16_t yCoord = y + height - 1;  
    unsigned char* img = bmp->bitmapData;
    for (int i = 0; i < height; i++, yCoord--) { // y
        xCoord = x;
    	for(int j = 0; j < width; j++, xCoord++) { // x
            uint32_t color = *(uint32_t*)img;
            if (!is_transparent(color)) {
                if((color & 0xFFFFFF) == GREEN)
                    vg_draw_pixel(xCoord, yCoord, new_color);
                else vg_draw_pixel(xCoord, yCoord, color);
            }
            img += bmp->bytes_per_pixel;
        }
        img += bmp->padding; // * bmp->bytes_per_pixel;
    }
}

uint32_t get_bitmap_color(Bitmap* bmp, uint16_t x, uint16_t y) {
    if (x >= bmp->bitmapInfoHeader.width || y >= bmp->bitmapInfoHeader.height)
        return 0;
    uint32_t color = 0;
    unsigned char* pixel_ptr = bmp->bitmapData + (bmp->actual_width * (bmp->bitmapInfoHeader.height - y - 1) + x) * bmp->bytes_per_pixel;
    for (int offset = bmp->bytes_per_pixel -  1; offset >= 0; offset--) {
        color = color << 8;
        uint8_t comp = *(pixel_ptr + offset);
        color = color | comp;
    }
    return color;
}

bool is_transparent(uint32_t color) {
    return (color & 0xFFFFFF) == 0xFF00FF;
}

// NAO USADO POR AGORA
char *double_buffering() {
	char* double_buffer = malloc(vg_get_hres() * vg_get_vres() * vg_get_bytes_pp());
	char* video_mem = vg_get_video_mem();
	memcpy(double_buffer, video_mem, vg_get_vres() * vg_get_hres() * vg_get_bytes_pp());
	return double_buffer;
}

void deleteBitmap(Bitmap* bmp) {
    if (bmp == NULL)
        return;
    free(bmp->bitmapData);
    free(bmp);
}

