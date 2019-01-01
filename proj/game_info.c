#include <lcom/lcf.h>

#include "game_info.h"

extern Bitmap* instructions_snake;
extern Bitmap* instructions_flappy;
extern Bitmap* instructions_draw;
extern Bitmap* instructions_wordgame;

static Layer* instructions_layer = NULL;

static bool wordgame_info_on = false;
static bool drawtool_info_on = false;
static bool flappy_info_on = false;
static bool snake_info_on = false;

void toggle_instructions(InfoType type) {
    switch (type) {    
        case WordGameInfo: 
            if (wordgame_info_on)
                clear_instructions();
            else display_instructions(WordGameInfo);
            break;
        case DrawToolInfo:
            if (drawtool_info_on)
                clear_instructions();
            else display_instructions(DrawToolInfo);
            break;
        case SnakeInfo:
            if (snake_info_on)
                clear_instructions();
            else display_instructions(SnakeInfo);
            break;
        case FlappyInfo:
            if (flappy_info_on)
                clear_instructions();
            else display_instructions(FlappyInfo);
            break;
        default: break;
    }
}

void display_instructions(InfoType type) {
    clear_instructions();
    switch (type) {    
        case WordGameInfo: 
            wordgame_info_on = true;
            instructions_layer = create_layer(200, 100, 800, 600);
            layer_draw_image(instructions_layer, instructions_wordgame, 200, 100);
            break;
        case DrawToolInfo:
            drawtool_info_on = true;
            instructions_layer = create_layer(200, 100, 800, 600);
            layer_draw_image(instructions_layer, instructions_draw, 200, 100);
            break;
        case SnakeInfo:
            snake_info_on = true;
            instructions_layer = create_layer(20, 50, 800, 500);
            layer_draw_image(instructions_layer, instructions_snake, 20, 50);
            break;
        case FlappyInfo:
            flappy_info_on = true;
            instructions_layer = create_layer(215, 275, 525, 200);
            layer_draw_image(instructions_layer, instructions_flappy, 215, 275);
            break;
        default: break;
    }
}

void clear_instructions() {
    if (instructions_layer != NULL) {
        destroy_layer(instructions_layer);
        instructions_layer = NULL;
    }
    wordgame_info_on = drawtool_info_on = flappy_info_on = snake_info_on = false;
}
