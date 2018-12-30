#pragma once

#include <math.h>
#include "video.h"
#include "sprite.h"
#include "canvas.h"
#include "bitmap.h"
#include "textbox.h"

/**
 * @brief Structure that represents the controlled object in the minigame.
 * 
 */
typedef struct {
    double y;           ///< Current Y position (higher as it goes down)
    double y_prev;      ///< Previous Y position
    int min_y;          ///< Upper limit for the Y position
    int max_y;          ///< Lower limit for the Y position
    int x;              ///< X position (constant)    
    double velocity;    ///< Y velocity
    double acceleration;///< Y acceleration
    bool goingUp;       ///< Set to true iff space bar is pressed
    bool isDead;        ///< Set to true iff the object is no longer playable (dead)
    int score;          ///< Player score
    uint8_t radius;     ///< Radius of the circle to be drawn on the bird's position 
    uint32_t color;     ///< Color to be drawn on the current position update 
    Hitbox hitbox;      ///< Player hitbox
} Bird;

/**
 * @brief Represents an obstacle in the minigame.
 * 
 */
typedef struct {
    Bitmap* bmp;        ///< Tube bitmap
    Hitbox hitbox;      ///< Tube hitbox
    bool passed;        ///< Set to true iff the bird already went past the tube
} FlappyTube;

/**
 * @brief reads the highscore .txt file for the flappy minigame. Sets highscore to 0 if the file does not exist.
 * 
 * @param path Full path for the .txt file
 */
void loadFlappyHighscore(char* path);

/**
 * @brief Saves the highscore .txt file for the flappy minigame.
 * Assumes loadFlappyHighscore() was previously called.
 * 
 */
void saveFlappyHighscore();

/**
 * @brief Displays, on the screen, that the game is over and the score as well 
 * 
 */
void flappy_gameover();

/**
 * @brief Alters the bird's position, based on its acceleration and velocity.
 * 
 */
void flappy_move();

/**
 * @brief Checks the x and y position, and updates the bird's hitbox accordingly.
 * 
 */
void flappy_update_hitbox();

/**
 * @brief Increases the score by 1. Updates the radius depending on its score, for a minimum of 2 and maximum of 8.
 * Formula: radius = log2(score);
 * 
 */
void flappy_increase_score();

/**
 * @brief Updates the score area, checking if any new obstacle was passed.
 * 
 */
void flappy_update_score();

/**
 * @brief Updates the screen, by moving the game area and obstacles, and redrawing the bird.
 * 
 */
void flappy_update_screen();

/**
 * @brief Checks if the bird is on the hitbox of any obstacle.
 * 
 * @return true The bird is on an obstacle hitbox.
 * @return false The bird is not on an obstacle hitbox.
 */
bool flappy_collided();

/**
 * @brief Moves the obstacles hitbox and redraws them
 * 
 */
void flappy_move_tubes();

/**
 * @brief Frees the memory used by the offscreen tubes.
 * 
 */
void flappy_clear_dead_tubes();

/**
 * @brief Adds an obstacle on the screen. 
 * 
 * @param tube_no Type of obstacle. The higher the more difficult.
 */
void flappy_add_tube(uint16_t tube_no);

/**
 * @brief Removes an obstacle from the array. The array order may change.
 * 
 * @param index Index of the obstacle array.
 */
void flappy_remove_tube(int index);


/**
 * @brief Creates a bird object and starts the game.
 * 
 * @param background Layer where the score will be displayed.
 */
void start_flappy(Layer* background);

/**
 * @brief Ends the game and destroys the bird object, and all of the obstacles.
 * 
 */
void end_flappy();

/**
 * @brief Moves the game forward.
 * Moves the bird vertically, moves the screen to the left, updates the score and checks if the bird has collided with an obstacle.
 * May also add new obstacles after a random amount of time.
 * 
 */
void flappy_tick();

/**
 * @brief Checks if the bird is dead (attribute isDead)
 * 
 * @return true The bird is dead
 * @return false The bird is not dead
 */
bool flappy_dead();

/**
 * @brief Sets the bird velocity to 3.5, upwards. Does nothing if isDead or goingUp attributes are set to true.
 * 
 */
void flappy_jump();

/**
 * @brief Sets the goingUp attribute to false.
 * 
 */
void flappy_endjump();

/**
 * @brief Displays, on the screen, the game highscore
 * 
 */
void flappy_showstats();
