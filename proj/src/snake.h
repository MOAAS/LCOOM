#include <stdlib.h>
#include <math.h>

#include "bitmaps.h"
#include "canvas.h"
#include "textbox.h"
#include "sprite.h"

/** @defgroup snake snake
 * @{
 *
 * Functions used to create and update a rainbow snake game.
 */

/**
 * @brief Structure that represents the controlled object in the snake minigame.
 * 
 */
typedef struct {
    double x;                   ///< X position
    double y;                   ///< Y position
    double speed;               ///< Current speed
    double max_speed;           ///< Maximum snake speed
    double angle;               ///< Snake direction. 0 is east, 90 is south, 180 is west, 270 is north
    bool turning_left;          ///< Set to true iff the player is pressing the ARROW_LEFT key
    bool turning_right;         ///< Set to true iff the player is pressing the ARROW_RIGHT key
    uint32_t color;             ///< Snake color
    int health;                 ///< Snake health points
    int max_health;             ///< Initial (maximum) health points
    bool ricocheting;           ///< Set to true iff the snake has just bounced off a wall
    bool isDead;                ///< Set to true iff the snake's health dropped below 0. Cannot move if set to true.
    int timeSinceDeath;         ///< Ticks since snake death (0 if alive, 60 after 1 second dead)
    int obstacleImmunityLeft;   ///< Amount of time until obstacle immunity wears off (60 = 1 second)
    int invincibilityLeft;      ///< Amount of time until invincibility wears off (60 = 1 second)
    int switcherooLeft;         ///< Amount of time until control switch wears off (60 = 1 second)
    int blindLeft;              ///< Amount of time until blind debuff wears off (60 = 1 seconds)
    int doubleLeft;             ///< Amount of time until double points wears off (60 = 1 seconds)
    int bleedingLeft;           ///< Amount of time until bleeding debuff wears off (60 = 1 seconds)
    int score;                  ///< Score      
    double top_speed;           ///< Top reached speed (not maximum speed)
} Snake;

/**
 * @brief Enumerates the different types of snake Powerups.
 * 
 */
typedef enum {
    ExtraHp = 0,        ///< Increases HP by 40% of Max HP (maximum of 100%)
    Speedy = 1,         ///< Multiplies speed by 1.6
    Slowed = 2,         ///< Multiplies speed by 0.8
    Invincibility = 3,  ///< Grands 2 seconds of invincibility
    Rotate = 4,         ///< Turns the snake around
    Switcheroo = 5,     ///< Switchs the controls for 3 seconds
    Blind = 6,          ///< Does not allow the user to see the HP bar for 2 seconds
    BonusPoints = 7,    ///< Grants 5 times as many points for 3 seconds
    Bleeding = 8,       ///< Player loses health twice as fast for 1.5 seconds
    Random = 9          ///< Grants a random powerup between the previous 9
} PowerupType;

/**
 * @brief Represents a snake powerup.
 * 
 */
typedef struct {
    Hitbox hitbox;      ///< Powerup hitbox, will be checked for collision
    Layer* layer;       ///< Layer where the powerup will be drawn on
    Bitmap* bmp;        ///< Powerup bitmap
    PowerupType type;   ///< Powerup type (should match the bitmap)
    int timeLeft;       ///< No. of ticks left for the powerup to vanish (60 ticks / sec)
} Powerup;

/**
 * @brief Represent a snake decoy, used for when the snake multiplies.
 * 
 */
typedef struct {
    double x;       ///< X position
    double y;       ///< Y position
    double speed;   ///< Current speed
    double angle;   ///< Snake direction. 0 is east, 90 is south, 180 is west, 270 is north
    bool isDead;    ///< Set to true iff the decoy hit the wall or an obstacle
    uint32_t color; ///< Decoy color
} SnakeDecoy;

/**
 * @brief Represents a snake obstacle.
 * 
 */
typedef struct {
    Hitbox hitbox;  ///< Obstacle hitbox, will be checked for collision
    int x, y;       ///< Coordinates of the top left corner of the bitmap
    Bitmap* bmp;    ///< Obstacle bitmap
} Obstacle;

/**
 * @brief Enumerates possible snake directions.
 * 
 */
enum Direction {
    LeftDir,    ///< Turn snake left
    RightDir    ///< Turn snake right
};

/**
 * @brief reads the highscore .txt file for the snake minigame. Sets highscore to 0 if the file does not exist.
 * 
 * @param path Full path for the .txt file
 */
void loadSnakeHighscore(char* path);

/**
 * @brief Saves the highscore .txt file for the snake minigame.
 * Assumes loadSnakeHighscore() was previously called.
 * 
 */
void saveSnakeHighscore();

/**
 * @brief Creates a snake, on a random position in the map that has no obstacle, starting with a random angle.
 * Also starts with 1 second of obstacle immunity
 * 
 * @param speed 
 * @param max_health 
 */
void create_snake(double speed, int max_health);

/**
 * @brief Displays on the screen the highscore, score and top speed of the run.
 * 
 */
void snake_showstats();


/**
 * @brief Multiplies the snake into several decoys (visual effect)
 * 
 * @param num Number of decoys to create.
 */
void snake_multiply(int num);

/**
 * @brief Moves the game forward. Moves the snake and decoys, redraws the obstacles and increases the snake's speed.
 * Reduces the immunities and debuffs by 1 tick and decreases health by 1 point (unless it's invincible)
 * May add a powerup or more. Updates the scoreboard and HP bar on the left.
 * 
 */
void snake_game_tick();

/**
 * @brief Increases the snake speed by 0.01. Increases the snake score by speed points.
 * 
 */
void snake_increase_speed();

/**
 * @brief Draws the snake score and speed on the top.
 * 
 */
void snake_update_scoreboard();

/**
 * @brief Moves the snake, if it's not dead.
 * Checks if there was obstacle collision (loses health and multiplies in that case).
 * Checks if there was powerup collision (consumes it)
 * Checks if there was wall bouncing (bounces off)
 * 
 */
void snake_move();

/**
 * @brief Moves all the living snake decoys
 * Checks if there was obstacle collision (stops)
 * Checks if there was powerup collision (destroys it)
 * Checks if there was wall bouncing (stops)
 * 
 */
void snake_move_decoys();

/**
 * @brief Redraws the UI HP bar
 * 
 */
void snake_update_hp_bar();

/**
 * @brief Sets a direction of the snake.
 * 
 * @param Direction LeftDir or RightDir, direction to set
 * @param value True or false, value to put in
 */
void snake_set_turning(enum Direction dir, bool value);

/**
 * @brief Changes the angle of the snake, depending on the snake's speed. If switcherooLeft > 0, it's sent on the opposite direction.
 * 
 */
void snake_turn_left();

/**
 * @brief Changes the angle of the snake, depending on the snake's speed. If switcherooLeft > 0, it's sent on the opposite direction.
 * 
 */
void snake_turn_right();

/**
 * @brief Checks if the game is over
 * 
 * @return true More than 2 seconds have passed since the snake died
 * @return false 2 seconds have not passed since the snake died
 */
bool snake_isGameOver();

/**
 * @brief Represents an arena where the rainbow snake can move on.
 * 
 */
typedef struct {
    uint16_t xMin; ///< X position of the Upper left corner 
    uint16_t yMin; ///< Y position of the Upper left corner 
    uint16_t xMax; ///< X position of the Lower right corner 
    uint16_t yMax; ///< Y position of the Lower right corner 
} SnakeArena;

/**
 * @brief Creates a snake arena and UI. Adds obstacles and creates the HP bar. Also adds the scoreboard on top.
 * 
 * @param background Layer to draw the arena on.
 */
void create_snake_arena(Layer* background);

/**
 * @return uint16_t Returns a random X position on the arena
 */
uint16_t arena_get_random_x();

/**
 * @return uint16_t Returns a random Y position on the arena
 */
uint16_t arena_get_random_y();

/**
 * @brief Draws all the obstacles on the screen.
 * 
 */
void arena_update_obstacles();

/**
 * @brief Draws the HP Bar on the left
 * 
 */
void arena_create_hp_bar();

/**
 * @brief Creates between 1 and 12 random obstacles, and adds them to the arena.
 * 
 */
void arena_create_obstacles();

/**
 * @brief Adds an obstacle to the arena
 * 
 * @param obstacle_num Obstacle number / type
 * @param x X position of the upper left corner
 * @param y Y position of the upper left corner
 */
void arena_add_obstacle(uint8_t obstacle_num, uint16_t x, uint16_t y);

/**
 * @brief Adds a powerup to the arena
 * 
 * @param powerup_num Powerup number / type
 * @param x X position of the upper left corner
 * @param y Y position of the upper left corner
 */
void arena_add_powerup(uint8_t powerup_num, uint16_t x, uint16_t y);

/**
 * @brief Removes a powerup from the screen.
 * 
 * @param index Index of the powerup array.
 */
void arena_remove_powerup(int index);

/**
 * @brief Updates the arena powerups. If any of them expired, removes it.
 * Randomly adds HP (time-based) and other (speed-based) powerups.
 * 
 */
void arena_update_powerups();

/**
 * @brief Consumes a powerup. 
 * 
 * @param type Type of powerup to consume.
 */
void snake_consume_powerup(PowerupType type);

/**
 * @brief Looks at the pixel at (x,y) position and checks if it belongs to an obstacle
 * 
 * @param x X position
 * @param y Y position
 * @return true There's an obstacle
 * @return false There's no obstacle
 */
bool is_obstacle(uint16_t x, uint16_t y);

/**
 * @brief Removes all powerups and obstacles and destroys all of the UI.
 * 
 */
void destroy_snake_arena();
   
