#include <stdlib.h>
#include <math.h>

#include "bitmaps.h"
#include "canvas.h"
#include "textbox.h"
#include "sprite.h"

typedef struct {
    double x;
    double y;
    double speed;
    double max_speed;
    double angle;
    bool turning_left;
    bool turning_right;
    uint32_t color;
    int health;
    int max_health;
    bool isDead;
    int timeSinceDeath;
    int gracePeriodLeft;

    // powerup coisas
    int invincibilityLeft;
    int switcherooLeft;
    int score;
    double top_speed;
} Snake;

typedef enum {
    ExtraHp = 0,
    Speedy = 1,
    Slowed = 2,
    Invincibility = 3,
    Switcheroo = 4
} PowerupType;

typedef struct {
    Hitbox hitbox;
    Layer* layer;
    Bitmap* bmp;
    PowerupType type;
    int timeLeft;
} Powerup;

typedef struct {
    double x;
    double y;
    double speed;
    double angle;
    bool isDead;
    uint32_t color;
} SnakeDecoy;

typedef struct {
    Hitbox hitbox;
    int x, y;
    Bitmap* bmp;
} Obstacle;

enum Direction {
    Left,
    Right
};

void loadSnakeHighscore(char* path);
void saveSnakeHighscore();


void create_snake(uint16_t x, uint16_t y, double speed, int max_health);// double max_speed, double angle);
void snake_showstats();

void snake_multiply(int num);

void snake_game_tick();
void snake_increase_speed();
void snake_update_scoreboard();
void snake_move();
void snake_move_decoys();
void snake_update_hp_bar();
void snake_set_turning(enum Direction dir, bool value);
void snake_turn_left();
void snake_turn_right();
bool snake_isGameOver();

typedef struct {
    uint16_t xMin;
    uint16_t xMax;
    uint16_t yMin;
    uint16_t yMax;    
} SnakeArena;

void create_snake_arena(Layer* background);
uint16_t arena_get_random_x();
uint16_t arena_get_random_y();
void arena_update_obstacles();
void arena_create_hp_bar();
void arena_create_obstacles();
void arena_add_obstacle(uint8_t obstacle_num, uint16_t x, uint16_t y);
void arena_add_powerup(uint8_t powerup_num, uint16_t x, uint16_t y);
void arena_remove_powerup(int index);
void arena_update_powerups();
void snake_consume_powerup(PowerupType type);


bool is_obstacle(uint16_t x, uint16_t y);

void destroy_snake_arena();
   
