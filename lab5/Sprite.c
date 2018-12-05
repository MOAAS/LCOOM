#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "Sprite.h"
#include "video.h"

Sprite *create_sprite(const char *pic[], int x, int y) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    if( sp == NULL )
    return NULL;
    // read the sprite pixmap
    sp->map = read_xpm(pic, &(sp->width), &(sp->height));
    if( sp->map == NULL ) {
        free(sp);
        return NULL;
    }
    sp->x = x;
    sp->y = y;
    return sp;
}

void destroy_sprite(Sprite *sp) {
    if(sp == NULL)
        return;
    if(sp ->map)
        free(sp->map);
    free(sp);
    sp = NULL; // XXX: pointer is passed by value
}

void draw_sprite(Sprite *sp) {
    for (int i = 0; i < sp->width; i++)   {
        for (int j = 0; j < sp->height; j++) {
            uint32_t color = sp->map[j*sp->width + i];
            if (color != 0)
                video_draw_pixel(sp->x + i, sp->y + j, color);
        }
    }
}

void erase_sprite(Sprite *sp, uint32_t background_color) {
    for (int i = 0; i < sp->width; i++) {
        for (int j = 0; j < sp->height; j++) {
            if (sp->map[j*sp->width + i] != 0)
                video_draw_pixel(sp->x + i, sp->y + j, background_color);
        }
    }
}

void move_sprite(Sprite *sp, int16_t delX, int16_t delY, uint32_t background_color) {
    erase_sprite(sp, background_color); 
    background_color =3;
    sp->x = sp->x + delX;
    sp->y = sp->y + delY;
    draw_sprite(sp);
}
