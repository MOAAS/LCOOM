#include <lcom/lcf.h>

#include <stdint.h>
#include <stdint.h>
#include <stdio.h>

#include "snake.h"

static SnakeArena arena;
static Snake snake;

static int num_decoys = 0;
static SnakeDecoy decoys[20];

static char* obstacle_map;
static int num_obstacles = 0;
static Obstacle obstacles[30];

static int timeLeftUntilHp = 0;
static int num_powerups = 0;
static Powerup powerups[50];
static int powerup_types = 10;

static Hitbox hpbar;

static Layer* background = NULL;
static Layer* textbox_layer = NULL;

static TextBox* score_textbox = NULL;
static TextBox* speed_textbox = NULL;

extern Bitmap* snake_head;

extern Bitmap* textbox3_bmp;
extern Bitmap* textbox4_bmp;
extern Bitmap* obstacle0_bmp;
extern Bitmap* obstacle1_bmp;
extern Bitmap* obstacle2_bmp;
extern Bitmap* obstacle3_bmp;

extern Bitmap* hpbar_bmp;
extern Bitmap* hpbar_hidden_bmp;

extern Bitmap* powerup0_bmp;
extern Bitmap* powerup1_bmp;
extern Bitmap* powerup2_bmp;
extern Bitmap* powerup3_bmp;
extern Bitmap* powerup4_bmp;
extern Bitmap* powerup5_bmp;
extern Bitmap* powerup6_bmp;
extern Bitmap* powerup7_bmp;
extern Bitmap* powerup8_bmp;
extern Bitmap* powerup9_bmp;


static int highscore = 0;
static char * file_path;

void create_snake(double speed, int max_health) {
    for (int i = 0; i < 50; i++) {
        snake.x = arena_get_random_x();
        snake.y = arena_get_random_y();
        if (!is_obstacle(snake.x, snake.y))
            break;
    }
    snake.speed = speed;
    snake.max_speed = 30;
    snake.angle = M_PI * (rand() % 360) / 180.0;
    snake.health = max_health;
    snake.max_health = max_health;
    snake.color = RED;
    snake.turning_left = false;
    snake.turning_right = false;
    snake.ricocheting = false;
    snake.isDead = false;
    snake.timeSinceDeath = 0;
    snake.score = 0;
    snake.top_speed = speed;
    snake.obstacleImmunityLeft = 60; // 1 segundo inicial
    snake.invincibilityLeft = 0;
    snake.switcherooLeft = 0;
    snake.blindLeft = 0;
    snake.doubleLeft = 0;
    snake.bleedingLeft = 0;
    for (int i = 0; i < 20; i++) {
        decoys[i].isDead = true;
    }
}

void snake_showstats() {
    char top_speed_str[100];
    char score_str[100];
    char highscore_str[100];
    // gravar no ficheiro, confuso mas funcionaaa
    if (snake.score < highscore) {
        sprintf(highscore_str, "Highscore: %d", highscore);
    }
    else {
        highscore = snake.score;
        sprintf(highscore_str, "New highscore: %d!", highscore);
        saveSnakeHighscore();
    }
    sprintf(top_speed_str, "Top speed: %.1f", snake.top_speed);
    sprintf(score_str, "Score: %d", snake.score);
    draw_bitmap_transp(textbox4_bmp, 60, 250, 0.8);
    draw_word(background, score_str, 500, 300, 3, 0, CenterAlign);
    wait_ms(800);
    draw_word(background, top_speed_str, 500, 400, 2, 0, CenterAlign);
    wait_ms(800);
    draw_word(background, highscore_str, 500, 500, 2, 0, CenterAlign);
}

void snake_game_tick() {
    snake_move_decoys();
    arena_update_obstacles();
    if (snake.isDead) {
        snake.timeSinceDeath++;
        return;
    }
    snake.switcherooLeft--;
    snake.obstacleImmunityLeft--; 
    snake.blindLeft--; 
    snake.doubleLeft--;
    snake.bleedingLeft--;
    if (snake.invincibilityLeft <= 0) {
        snake.health--;
        if (snake.bleedingLeft > 0)
            snake.health--;
    }
    else snake.invincibilityLeft--;
    if (snake.health <= 0) {
        snake.isDead = true;
        snake_multiply(rand() % 8 + 5);
        while(num_powerups > 0)
            arena_remove_powerup(0);
    }
    snake_move();
    arena_update_powerups();
    snake_increase_speed();
    snake_update_scoreboard();
    snake_update_hp_bar();    
}


void snake_increase_speed() {
    snake.speed += 0.01;
    if (snake.speed >= snake.max_speed)
        snake.speed = snake.max_speed;
    if (snake.speed > snake.top_speed)
        snake.top_speed = snake.speed;
}

void snake_update_scoreboard() {
    if (snake.doubleLeft >= 0)
        snake.score += 5 * snake.speed;
    else snake.score += snake.speed;
    char speed[20];
    char score[20];
    sprintf(speed, "%.1f", snake.speed);
    sprintf(score, "%d", snake.score);
    textbox_clear(speed_textbox);
    textbox_write(speed_textbox, speed);
    textbox_clear(score_textbox);
    textbox_write(score_textbox, score);
}

void snake_move() {
    double xNext = snake.x + snake.speed * cos(snake.angle);
    double yNext = snake.y + snake.speed * sin(snake.angle);
    // Check obstacle collision
    if (snake.invincibilityLeft <= 0 && snake.obstacleImmunityLeft <= 0 && is_obstacle(xNext, yNext)) {
        snake.obstacleImmunityLeft = 60;
        snake.health -= 60;
        snake_multiply(rand() % 8 + 5);
    }
    // Check powerup collision
    for (int i = 0; i < num_powerups; i++) {
        Hitbox hitbox = powerups[i].hitbox;
        if (snake.x >= hitbox.x1 && snake.x < hitbox.x2 && snake.y >= hitbox.y1 && snake.y < hitbox.y2) {
            snake_consume_powerup(powerups[i].type);
            arena_remove_powerup(i); 
            break;
        }
    }
    // Ricochet
    if (xNext > arena.xMax || xNext < arena.xMin) {
        snake.angle = M_PI - snake.angle;
        snake.speed *= 0.8;
        snake.ricocheting = true;
    }
    else if (yNext > arena.yMax || yNext < arena.yMin) {
        snake.angle = 2 * M_PI - snake.angle;
        snake.speed *= 0.8;
        snake.ricocheting = true;
    }
    // Move
    else {
        snake.color = rainbow(snake.color);
        canvas_draw_line((uint16_t)snake.x, (uint16_t)snake.y, (uint16_t)xNext, (uint16_t)yNext, snake.color, 5);
        snake.x = xNext;
        snake.y = yNext;
        snake.ricocheting = false;
    }
    if (snake.turning_left)
        snake_turn_left();
    if (snake.turning_right)
        snake_turn_right();
}

void snake_move_decoy(SnakeDecoy* decoy) {
    if (decoy->isDead)
        return;
    double xNext = decoy->x + decoy->speed * cos(decoy->angle);
    double yNext = decoy->y + decoy->speed * sin(decoy->angle);
    if (xNext > arena.xMax || xNext < arena.xMin)
        decoy->isDead = true;
    else if (yNext > arena.yMax || yNext < arena.yMin)
        decoy->isDead = true;
    if (is_obstacle(xNext, yNext))
        decoy->isDead = true;
    // Check powerup collision
    for (int i = 0; i < num_powerups; i++) {
        Hitbox hitbox = powerups[i].hitbox;
        if (decoy->x >= hitbox.x1 && decoy->x < hitbox.x2 && decoy->y >= hitbox.y1 && decoy->y < hitbox.y2) {
            arena_remove_powerup(i); 
            break;
        }
    }
    decoy->color = rainbow(decoy->color);
    canvas_draw_line((uint16_t)decoy->x, (uint16_t)decoy->y, (uint16_t)xNext, (uint16_t)yNext, decoy->color, 4);
    decoy->x = xNext;
    decoy->y = yNext;
    decoy->angle += (rand() % 30) / 100.0 - 0.15;
}

void snake_move_decoys() {
    for (int i = 0; i < num_decoys; i++) {
        snake_move_decoy(&decoys[i]);
    }    
}

void snake_multiply(int num) {
    num_decoys = num;
    double angleInc = M_PI * 2 / (double)num;
    for (int i = 0; i < num; i++) {
        decoys[i].isDead = false;
        decoys[i].x = snake.x;
        decoys[i].y = snake.y;
        decoys[i].speed = 2 * snake.speed;
        decoys[i].color = snake.color;
        decoys[i].angle = i * angleInc;
    }
}

void snake_set_turning(enum Direction dir, bool value) {
    if (dir == LeftDir)
        snake.turning_left = value;
    else snake.turning_right = value;
}

void snake_turn_left() {
    if (snake.switcherooLeft > 0)
        snake.angle += 0.02 * 1.1 * snake.speed;
    else snake.angle -= 0.02 * 1.1 * snake.speed;
}

void snake_turn_right() {
    if (snake.switcherooLeft > 0)
        snake.angle -= 0.02 * 1.1 * snake.speed;
    else snake.angle += 0.02 * 1.1 * snake.speed;
}

bool snake_isGameOver() {
    return snake.timeSinceDeath >= 120;
}

void snake_update_hp_bar() {
    if (snake.blindLeft > 0) {
        draw_bitmap(hpbar_hidden_bmp, 0, 95);
        return;
    }
    double hp_left = (double)snake.health / (double)snake.max_health;
    if (snake.health <= 0)
        hp_left = 0;
    int middle = hpbar.y1 + (1 - hp_left) * (hpbar.y2 - hpbar.y1);
    video_draw_rectangle(hpbar.x1, hpbar.y1, hpbar.x2 - hpbar.x1, middle - hpbar.y1, RED);
    if (snake.invincibilityLeft > 0)
        video_draw_rectangle(hpbar.x1, middle, hpbar.x2 - hpbar.x1, hpbar.y2 - middle, 0xFFD700);
    else if (snake.bleedingLeft > 0)
        video_draw_rectangle(hpbar.x1, middle, hpbar.x2 - hpbar.x1, hpbar.y2 - middle, 0xBE2635);
    else video_draw_rectangle(hpbar.x1, middle, hpbar.x2 - hpbar.x1, hpbar.y2 - middle, GREEN);

        
}

void create_snake_arena(Layer* layer) {
    arena.xMin = 100;
    arena.xMax = vg_get_hres() - 5;
    arena.yMin = 100;
    arena.yMax = vg_get_vres() - 5;
    background = layer;
    create_canvas(background, arena.xMin, arena.yMin, arena.xMax, arena.yMax, WHITE);
    arena_create_obstacles();
    arena_create_hp_bar();
    textbox_layer = create_layer(0, 0, vg_get_hres(), 100);
    score_textbox = create_textbox(textbox_layer, textbox3_bmp, 300, 50, 2, CenterAlign);
    speed_textbox = create_textbox(textbox_layer, textbox3_bmp, 875, 50, 2, CenterAlign);
}

void arena_add_powerup(uint8_t powerup_num, uint16_t x, uint16_t y) {
    Bitmap* bitmap;
    switch (powerup_num) {
        case 0: bitmap = powerup0_bmp; break;
        case 1: bitmap = powerup1_bmp; break;
        case 2: bitmap = powerup2_bmp; break;
        case 3: bitmap = powerup3_bmp; break;
        case 4: bitmap = powerup4_bmp; break;
        case 5: bitmap = powerup5_bmp; break;
        case 6: bitmap = powerup6_bmp; break;
        case 7: bitmap = powerup7_bmp; break;        
        case 8: bitmap = powerup8_bmp; break;        
        case 9: bitmap = powerup9_bmp; break;        
        default: return;
    }
    // Check arena limits
    uint16_t width = bitmap->bitmapInfoHeader.width;
    uint16_t height = bitmap->bitmapInfoHeader.height;
    if (x + width > arena.xMax - 10 || y + height > arena.yMax - 10)
        return;
    make_hitbox(&powerups[num_powerups].hitbox, x - 5, x + bitmap->bitmapInfoHeader.width + 5, y, y + bitmap->bitmapInfoHeader.height + 5);
    // Check for obstacle collision
    for (int i = 0; i < num_obstacles; i++) {
        if (check_hitbox_collision(powerups[num_powerups].hitbox, obstacles[i].hitbox))
            return;
    }
    // Check for powerup collision
    for (int i = 0; i < num_powerups; i++) {
        if (check_hitbox_collision(powerups[num_powerups].hitbox, powerups[i].hitbox))
            return;
    }
    powerups[num_powerups].layer = create_layer(x, y, bitmap->bitmapInfoHeader.width, bitmap->bitmapInfoHeader.height);
    powerups[num_powerups].bmp = bitmap;
    powerups[num_powerups].type = powerup_num;
    if (powerups[num_powerups].type == ExtraHp)
        powerups[num_powerups].timeLeft = 600;
    else powerups[num_powerups].timeLeft = 300;
    layer_draw_image(powerups[num_powerups].layer, powerups[num_powerups].bmp, x, y);
    num_powerups++;
}

void arena_remove_powerup(int index) {
    destroy_layer(powerups[index].layer);
    num_powerups--;
    powerups[index] = powerups[num_powerups];
}

void arena_update_powerups() {
    timeLeftUntilHp--;
    for (int i = num_powerups - 1; i >= 0; i--) {
        powerups[i].timeLeft--;
        if (powerups[i].timeLeft <= 0)
            arena_remove_powerup(i);
    }
    if (num_powerups >= 50)
        return;
    if (timeLeftUntilHp <= 0) {
        arena_add_powerup(0, arena_get_random_x(), arena_get_random_y());
        timeLeftUntilHp = 110 + rand() % 20; // ~2 secs
    }
    if (rand() % 180 < snake.speed) 
        arena_add_powerup(rand() % (powerup_types - 1) + 1, arena_get_random_x(), arena_get_random_y());
}

void snake_consume_powerup(PowerupType type) {
    switch (type) {
        case ExtraHp: 
            snake.health +=  snake.max_health * 0.4;
            if (snake.health > snake.max_health) {
                snake.health = snake.max_health;
                if (snake.invincibilityLeft <= 0)
                    snake.invincibilityLeft = 30;
                else snake.invincibilityLeft += 30;
            }
            break;
        case Speedy: snake.speed *= 1.6; break;
        case Slowed: snake.speed *= 0.8; break;
        case Invincibility: snake.invincibilityLeft = 120; break;
        case Switcheroo: snake.switcherooLeft = 180; break;
        case Rotate: if (!snake.ricocheting) snake.angle += M_PI; break;
        case Blind: snake.blindLeft = 120; break;
        case Bleeding: snake.bleedingLeft = 90; break;
        case BonusPoints: snake.doubleLeft = 180; break;
        case Random: snake_consume_powerup(rand() % (powerup_types - 1)); break;
        default: printf("Invalid type: %d \n", type); break;
    }
}

uint16_t arena_get_random_x() {
    return rand() % (arena.xMax - arena.xMin) + arena.xMin;
}

uint16_t arena_get_random_y() {
    return rand() % (arena.yMax - arena.yMin) + arena.yMin;
}

void arena_create_hp_bar() {
    layer_draw_image(background,hpbar_bmp, 0, 95);
    hpbar.x1 = 5;
    hpbar.x2 = 95;
    hpbar.y1 = 100;
    hpbar.y2 = vg_get_vres() - 5;
}

void arena_create_obstacles() {
    obstacle_map = malloc(vg_get_hres() * vg_get_vres() * vg_get_bytes_pp());
    int rand_num = rand() % 5 + 8;
    for (int i = 0; i < rand_num; i++) {
        arena_add_obstacle(rand() % 4, arena_get_random_x(), arena_get_random_y());
    }
    memcpy(obstacle_map, vg_get_video_mem(), vg_get_hres() * vg_get_vres() * vg_get_bytes_pp());
}

void arena_update_obstacles() {
    for (int i = 0; i < num_obstacles; i++) {
        draw_bitmap(obstacles[i].bmp, obstacles[i].x, obstacles[i].y); 
    }    
}

void arena_add_obstacle(uint8_t obstacle_num, uint16_t x, uint16_t y) {
    Bitmap* bitmap;
    switch (obstacle_num) {
        case 0: bitmap = obstacle0_bmp; break;
        case 1: bitmap = obstacle1_bmp; break;
        case 2: bitmap = obstacle2_bmp; break;
        case 3: bitmap = obstacle3_bmp; break;
        default: return;
    }
    // Check arena limits
    uint16_t width = bitmap->bitmapInfoHeader.width;
    uint16_t height = bitmap->bitmapInfoHeader.height;
    if (x + width > arena.xMax - 10 || y + height > arena.yMax - 10)
        return;
    Hitbox hitbox;
    make_hitbox(&hitbox, x - 20, x + width + 20, y - 20,  y + height + 20);
    // Check for obstacle collision
    for (int i = 0; i < num_obstacles; i++) {
        if (check_hitbox_collision(hitbox, obstacles[i].hitbox))
            return;
    }
    obstacles[num_obstacles].hitbox = hitbox;
    obstacles[num_obstacles].x = x;
    obstacles[num_obstacles].y = y;
    obstacles[num_obstacles].bmp = bitmap;
    draw_bitmap(bitmap, x, y);     
    num_obstacles++;
}

bool is_obstacle(uint16_t x, uint16_t y) {
    uint32_t color = vg_retrieve(calc_address(obstacle_map, x, y, vg_get_hres())) & 0xFFFFFF;
    return color == 1 || color == 0xE0E0E0 || color == 0xC6C6C6;
}

void destroy_snake_arena() {
    while(num_powerups > 0)
        arena_remove_powerup(0);
    destroy_canvas();
    free(obstacle_map);
    free(speed_textbox);
    destroy_textbox(score_textbox);
    num_obstacles = 0;
    speed_textbox = NULL;
    score_textbox = NULL;
    textbox_layer = NULL;
    background = NULL;
}

void loadSnakeHighscore(char* path) {
    file_path = path;
    FILE *filePtr;
    filePtr = fopen(file_path, "rb");
    if (filePtr == NULL) {
        printf("snake file not found, highscore set to 0. File: %s \n", path);
        highscore = 0;
        return;
    }
    char temp[100];
    fscanf(filePtr, "%s %d", temp, &highscore);
    fclose(filePtr);
}

void saveSnakeHighscore() {
    FILE *filePtr;
    filePtr = fopen(file_path, "w");
    fprintf(filePtr, "Highscore: %d", highscore);
    fclose(filePtr);
}
