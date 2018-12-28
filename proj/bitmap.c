#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "bitmap.h"
#include "video.h"


Bitmap* loadBitmap(const char* folderPath, const char* filename) {
    char* file_path = malloc((strlen(folderPath) + strlen(filename) + 1) * sizeof(char));
    strcpy(file_path, folderPath);
    strcat(file_path, filename);
    // allocating necessary size
    Bitmap* bmp = (Bitmap*) malloc(sizeof(Bitmap));
    // open filename in read binary mode
    FILE *filePtr;
    filePtr = fopen(file_path, "rb");
    if (filePtr == NULL) {
        printf("File not found! %s\n", file_path);
        return NULL;
    }
    // read the bitmap file header
    BitmapFileHeader bitmapFileHeader;
    fread(&bitmapFileHeader, sizeof(BitmapFileHeader), 1, filePtr);

    // verify that this is a bmp file by check bitmap id
    if (bitmapFileHeader.type != 0x4D42) {
        fclose(filePtr);
        printf("Error reading file: %s! Not a BMP!\n", file_path);
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
    bmp->actual_bytes_per_row = bitmapInfoHeader.width * bytes_per_pixel + bmp->padding;
    bitmapInfoHeader.imageSize = bitmapInfoHeader.height * bmp->actual_bytes_per_row;
    char* bitmapImage = malloc(bitmapInfoHeader.imageSize);
    // verify memory allocation
    if (!bitmapImage) {
        free(bitmapImage);
        fclose(filePtr);
        printf("Error reading file: %s! Bad allocation.\n", file_path);
        return NULL;
    }
    // read in the bitmap image data
    fread(bitmapImage, bitmapInfoHeader.imageSize, 1, filePtr);
    // make sure bitmap image data was read
    if (bitmapImage == NULL) {
        fclose(filePtr);
        printf("Error reading file: %s! Bad reading.\n", file_path);
        return NULL;
    }

    // close file and return bitmap
    fclose(filePtr);
    free(file_path);
    bmp->bitmapData = bitmapImage;
    bmp->bitmapInfoHeader = bitmapInfoHeader;
    bmp->bytes_per_pixel = bytes_per_pixel;
    return bmp;
}

Bitmap* loadBitmapSection(Bitmap* bitmap, uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
    Bitmap* section = malloc(sizeof(Bitmap));
    *section = *bitmap;
    uint8_t bytes_per_pixel = bitmap->bytes_per_pixel;
    section->padding = (4 - (width * bytes_per_pixel) % 4) % 4;
    section->bitmapInfoHeader.width = width;
    section->bitmapInfoHeader.height = height;
    section->actual_bytes_per_row = section->bitmapInfoHeader.width * bytes_per_pixel + section->padding;
    section->bitmapInfoHeader.imageSize = height * section->actual_bytes_per_row; 
    section->bitmapData  = malloc(section->bitmapInfoHeader.imageSize);
    char* original = bitmap->bitmapData + (bitmap->actual_bytes_per_row) * y + x * bytes_per_pixel;
    char* new = section->bitmapData;
    for (int i = 0; i < height; i++) { // y
        memcpy(new, original, width * bytes_per_pixel);
        original += bitmap->actual_bytes_per_row;
        new += section->actual_bytes_per_row;
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

    resized->actual_bytes_per_row = resized->bitmapInfoHeader.width * bytes_per_pixel + resized->padding;
    resized->bitmapInfoHeader.imageSize = height * resized->actual_bytes_per_row; 
    resized->bitmapData  = malloc(resized->bitmapInfoHeader.imageSize);
    char* original = bitmap->bitmapData;
    char* new = resized->bitmapData;
    for (int i = 0; i < height; i++) { // y
    	for(int j = 0; j < width; j++) { // x
            memcpy(new, original, bytes_per_pixel);
            if (j % factor == factor - 1)
                original += bytes_per_pixel;
            new += bytes_per_pixel;
        }
        if (i % factor != factor - 1)
            original -= bitmap->actual_bytes_per_row;
        original += bitmap->padding;
        new += resized->padding;
    }
    return resized;
}

void saveBitmap(char* filename, unsigned int width, unsigned int height, char* address) {
    FILE* file = fopen(filename, "w");
	static unsigned char bmp_header[54] = {66,77,0,0,0,0,0,0,0,0,54,0,0,0,40,0,0,0,0,0,0,0,0,0,0,0,1,0,24}; // info header
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

void draw_bitmap(Bitmap* bmp, int x, int y) {
    if (bmp == NULL)
        return;
    int width = bmp->bitmapInfoHeader.width;
    int height = bmp->bitmapInfoHeader.height;
    int xCoord = x;
    int yCoord = y + height - 1;  
    char* img = bmp->bitmapData;
    char* vmem = calc_address(vg_get_video_mem(), xCoord, yCoord, vg_get_hres());
    for (int i = 0; i < height; i++, yCoord--) { // y
        xCoord = x;
    	for(int j = 0; j < width; j++, xCoord++) { // x
            if (xCoord >= 0 && yCoord >= 0 && xCoord < (int)vg_get_hres() && yCoord < (int)vg_get_vres()) {
                uint32_t color = vg_retrieve(img);
                if (!is_transparent(color))
                    vg_insert(vmem, color);
            }
            img += bmp->bytes_per_pixel;
            vmem += vg_get_bytes_pp();
        }
        img += bmp->padding; // * bmp->bytes_per_pixel;
        vmem = vmem - (vg_get_hres() + width) * vg_get_bytes_pp();
    }
}

void draw_bitmap_color(Bitmap* bmp, int x, int y, uint32_t new_color) {
    if (bmp == NULL)
        return;
    int width = bmp->bitmapInfoHeader.width;
    int height = bmp->bitmapInfoHeader.height;
    uint16_t xCoord = x;
    uint16_t yCoord = y + height - 1;  
    char* img = bmp->bitmapData;
    for (int i = 0; i < height; i++, yCoord--) { // y
        xCoord = x;
    	for(int j = 0; j < width; j++, xCoord++) { // x
            uint32_t color = vg_retrieve(img);
            if ((color & 0xFFFFFF) == GREEN)
                vg_draw_pixel(xCoord, yCoord, new_color);
            else if (!is_transparent(color))
                vg_draw_pixel(xCoord, yCoord, color);
            img += bmp->bytes_per_pixel;
        }
        img += bmp->padding; 
    }
}

uint32_t get_bitmap_color(Bitmap* bmp, uint16_t x, uint16_t y) {
    if (x >= bmp->bitmapInfoHeader.width || y >= bmp->bitmapInfoHeader.height)
        return 0;
    uint32_t color = 0;
    char* pixel_ptr = bmp->bitmapData + (bmp->actual_bytes_per_row * (bmp->bitmapInfoHeader.height - y - 1) + x * bmp->bytes_per_pixel);
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

unsigned char * db;

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

