#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "flappy.h"

static Bird bird;

static int time_til_tube = 0;
static int num_tubes = 0;
static FlappyTube tubes[10];

static int game_speed = 2;
static uint32_t sky_color = CYAN;

extern Bitmap* flappy_tube0_bmp;
extern Bitmap* flappy_tube1_bmp;
extern Bitmap* flappy_tube2_bmp;
extern Bitmap* flappy_tube3_bmp;
extern Bitmap* textbox4_bmp;
extern Bitmap* textbox5_bmp;

static Layer* score_layer = NULL;
static TextBox* score_textbox = NULL;

static int highscore = 0;
static char * file_path;

void start_flappy(Layer* background) {
    sky_color = CYAN;
    create_canvas(background, 0, 0, vg_get_hres(), vg_get_vres(), sky_color);
    bird.x = 200;
    bird.y = 300;
    bird.min_y = 100;
    bird.max_y = vg_get_vres() - 1;
    bird.velocity = 0;
    bird.acceleration = 0.1;
    bird.goingUp = false;
    bird.isDead = false;
    bird.score = 0;
    bird.radius = 2;
    bird.color = sky_color;
    flappy_update_hitbox();
    game_speed = 2;
    score_layer = create_layer(0, 0, vg_get_hres(), 100);
    score_textbox = create_textbox(score_layer, textbox5_bmp, 350, 50, 3, CenterAlign);
    textbox_write(score_textbox, "Score: 0");
}

void end_flappy() {
    destroy_canvas();
    destroy_textbox(score_textbox);
    while (num_tubes > 0)
        flappy_remove_tube(0);
}

void flappy_gameover() {
    free(score_textbox);
    score_textbox = create_textbox(score_layer, textbox5_bmp, 200, 50, 2, CenterAlign);
    char score[20];
    sprintf(score, "Game Over! Score: %d", bird.score);
    textbox_clear(score_textbox);
    textbox_write(score_textbox, score);
}

void flappy_showstats() {
    free(score_textbox);
    score_textbox = create_textbox(score_layer, textbox5_bmp, 200, 50, 3, CenterAlign);
    if (bird.score > highscore) {
        highscore = bird.score;
        saveFlappyHighscore();
    }
    char score[20];
    sprintf(score, "Highscore: %d", highscore);
    textbox_clear(score_textbox);
    textbox_write(score_textbox, score);
}


void flappy_update_hitbox() {
    make_hitbox(&bird.hitbox, bird.x - bird.radius, bird.x + bird.radius, bird.y - bird.radius, bird.y + bird.radius);
}


void flappy_tick() {
    flappy_move();
    flappy_update_screen();
    flappy_clear_dead_tubes();    
    flappy_update_score();
    if (!bird.isDead && flappy_collided()) {
        bird.isDead = true;
        bird.acceleration *= 5;
        flappy_gameover();
    }
    if (time_til_tube <= 0) {
        uint16_t rand_num = rand() % 7;
        if (bird.score > 10)
            rand_num++;
        if (bird.score > 25)
            rand_num++;
        if (bird.score > 50)
            rand_num++;
        if (bird.score > 100)
            rand_num++;
        flappy_add_tube(rand_num);
        time_til_tube = rand() % 60 + 120; // 2-3 s
    }
    else time_til_tube--;
}


void flappy_move() {
    bird.y_prev = bird.y;
    bird.y += bird.velocity;
    bird.velocity += bird.acceleration;            
    if (bird.y < bird.min_y) {
        bird.y = bird.min_y;
        bird.velocity = 0;
    }
    else if (bird.y > bird.max_y) {
        bird.y = 9999;
    }
    flappy_update_hitbox();
}

void flappy_increase_score() {
    bird.score++;
    bird.radius = log2(bird.score);
    if (bird.radius > 8)
        bird.radius = 8;
    else if (bird.radius < 2)
        bird.radius = 2;
}

void flappy_update_score() {
    if (bird.isDead)
        return;
    bool scoreUpdate = false;
    for (int i = 0; i < num_tubes; i++) {
        if (!tubes[i].passed && tubes[i].hitbox.x2 < bird.hitbox.x1) {
            flappy_increase_score();
            tubes[i].passed = true;
            scoreUpdate = true;
        }
    }
    if (!scoreUpdate)
        return;
    char score[20];
    sprintf(score, "Score: %d", bird.score);
    textbox_clear(score_textbox);
    textbox_write(score_textbox, score);
}

void flappy_update_screen() {
    sky_color = bird.color = rainbow(sky_color);
    vg_move(bird.min_y, bird.max_y - bird.min_y + 5, -game_speed, sky_color);
    canvas_draw_line(bird.x, bird.y_prev, bird.x, bird.y, bird.color, bird.radius);
    //canvas_draw_circle(200, bird.y, 4, rainbow());            
    flappy_move_tubes();
}

bool flappy_dead() {
    return bird.isDead;
}

bool flappy_collided() {
    if (bird.y > bird.max_y)
        return true;
    for (int i = 0; i < num_tubes; i++) {
        if (check_hitbox_collision(bird.hitbox, tubes[i].hitbox)) {
            return true;
        }
    }
    return false;
}

void flappy_clear_dead_tubes() {
    for (int i = 0; i < num_tubes; i++) {
        if (tubes[i].hitbox.x2 < 0) {
            flappy_remove_tube(i);
            return;
        }
    }
}


void flappy_move_tubes() {
    for (int i = 0; i < num_tubes; i++) {
        move_hitbox(&tubes[i].hitbox, -game_speed, 0);
        if (tubes[i].hitbox.x2 + game_speed > (int)vg_get_hres())
            draw_bitmap(tubes[i].bmp, tubes[i].hitbox.x1, tubes[i].hitbox.y1);
    }
}

void flappy_add_tube(uint16_t tube_type) {
    uint16_t y = bird.min_y;
    switch (tube_type) {
        case 0: tubes[num_tubes].bmp = flappy_tube0_bmp; y = rand() % (int)ceil(75.0 / (bird.score + 1)) + rand() % 75 + 450; break;
        case 1: tubes[num_tubes].bmp = flappy_tube1_bmp; break;
        case 2: tubes[num_tubes].bmp = flappy_tube2_bmp; break;
        case 3: tubes[num_tubes].bmp = flappy_tube3_bmp; break;
        case 4: flappy_add_tube(0); flappy_add_tube(1); return;
        case 5: flappy_add_tube(0); flappy_add_tube(2); return;
        default: flappy_add_tube(0); flappy_add_tube(3); return;
    }
    uint16_t height = tubes[num_tubes].bmp->bitmapInfoHeader.height;
    uint16_t width = tubes[num_tubes].bmp->bitmapInfoHeader.width;
    make_hitbox(&tubes[num_tubes].hitbox, 1100, 1100 + width, y, y + height);
    tubes[num_tubes].passed = false;
    num_tubes++;
}

void flappy_remove_tube(int index) {
    num_tubes--;
    tubes[index] = tubes[num_tubes];
}

void flappy_jump() {
    if (bird.isDead || bird.goingUp)  
        return;
    bird.goingUp = true;
    bird.velocity = -3.5;
}

void flappy_endjump() {
    bird.goingUp = false;
}

void loadFlappyHighscore(char* path) {
    file_path = path;
    FILE *filePtr;
    filePtr = fopen(file_path, "rb");
    if (filePtr == NULL) {
        printf("flappy file not found, highscore set to 0. File: %s \n", path);
        highscore = 0;
        return;
    }
    char temp[100];
    fscanf(filePtr, "%s %d", temp, &highscore);
    fclose(filePtr);
}

void saveFlappyHighscore() {
    FILE *filePtr;
    filePtr = fopen(file_path, "w");
    fprintf(filePtr, "Highscore: %d", highscore);
    fclose(filePtr);
}
