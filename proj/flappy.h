#pragma once

#include <math.h>
#include "video.h"
#include "sprite.h"
#include "canvas.h"
#include "bitmap.h"
#include "textbox.h"

typedef struct {
    double y;
    double y_prev;
    int min_y;
    int max_y;
    int x;
    double velocity;
    double acceleration;
    bool goingUp;
    bool isDead;
    int score;
    uint8_t radius;
    uint32_t color;
    Hitbox hitbox;
} Bird;

typedef struct {
    Bitmap* bmp;
    Hitbox hitbox;
    bool passed;
} FlappyTube;

void loadFlappyHighscore(char* path);
void saveFlappyHighscore();

void start_flappy(Layer* background);
void end_flappy();
void flappy_gameover();
void flappy_showstats();
void flappy_tick();
void flappy_move();
void flappy_update_hitbox();
void flappy_update_score();
void flappy_update_screen();
bool flappy_dead();
bool flappy_collided();
void flappy_move_tubes();
void flappy_clear_dead_tubes();
void flappy_add_tube(uint16_t tube_no);
void flappy_remove_tube(int index);
void flappy_jump();
void flappy_endjump();
