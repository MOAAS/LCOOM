#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>

#include "bitmaps.h"

Bitmap* cursor_normal;
Bitmap* cursor_brush;
Bitmap* cursor_rainbow;
Bitmap* cursor_balde;
Bitmap* cursor_picker;
Bitmap* cursor_rubber;

//Bitmap* ULTRA_BACKGROUND;
Bitmap* welcome_screen;
Bitmap* background_menu;

Bitmap* mainmenu_button1;
Bitmap* mainmenu_button2;
Bitmap* mainmenu_button3;
Bitmap* mainmenu_button4;

Bitmap* mainmenu_button1_hl;
Bitmap* mainmenu_button2_hl;
Bitmap* mainmenu_button3_hl;
Bitmap* mainmenu_button4_hl;

Bitmap* snake_button;
Bitmap* snake_button_hl;
Bitmap* bird_button;
Bitmap* bird_button_hl;

Bitmap* rainbow_button;
Bitmap* rainbow_hl;
Bitmap* balde_button;
Bitmap* balde_hl;
Bitmap* picker_button;
Bitmap* picker_hl;
Bitmap* brush_button;
Bitmap* brush_hl;
Bitmap* trash_button;
Bitmap* trash_hl;
Bitmap* rubber_button;
Bitmap* rubber_hl;
Bitmap* palette_button;
Bitmap* palette_hl;
Bitmap* slider_bmp;
Bitmap* emote_icon;
Bitmap* emote_icon_hl;
Bitmap* save_button;
Bitmap* save_hl;
Bitmap* load_button;
Bitmap* load_hl;

Bitmap* letters_bmp;
Bitmap* textbox1_bmp;
Bitmap* textbox2_bmp;
Bitmap* textbox3_bmp;
Bitmap* textbox4_bmp;
Bitmap* textbox5_bmp;
Bitmap* textbox6_bmp;

Bitmap* colorpicker_bmp;
Bitmap* clock_bmp;
Bitmap* clockbig_bmp;

Bitmap* paint_tube_1_bmp;
Bitmap* paint_tube_2_bmp;

Bitmap* emote0_bmp;
Bitmap* emote1_bmp;
Bitmap* emote2_bmp;
Bitmap* emote3_bmp;
Bitmap* emote4_bmp;

Bitmap* emote0_hl_bmp;
Bitmap* emote1_hl_bmp;
Bitmap* emote2_hl_bmp;
Bitmap* emote3_hl_bmp;
Bitmap* emote4_hl_bmp;
Bitmap* emote_wheel_bmp;

Bitmap* obstacle0_bmp;
Bitmap* obstacle1_bmp;
Bitmap* obstacle2_bmp;
Bitmap* obstacle3_bmp;

Bitmap* hpbar_bmp;

Bitmap* powerup0_bmp;
Bitmap* powerup1_bmp;
Bitmap* powerup2_bmp;
Bitmap* powerup3_bmp;
Bitmap* powerup4_bmp;
Bitmap* powerup5_bmp;
Bitmap* powerup6_bmp;

Bitmap* flappy_tube0_bmp;
Bitmap* flappy_tube1_bmp;
Bitmap* flappy_tube2_bmp;
Bitmap* flappy_tube3_bmp;

void loadBitmaps(const char* folderPath) {
    // Background
    background_menu = loadBitmap(folderPath, "background_menu.bmp");
    welcome_screen = loadBitmap(folderPath, "background_menu_start.bmp");
    // Menu Buttons
    mainmenu_button1 = loadBitmap(folderPath, "mainmenu_button1.bmp");

    mainmenu_button2 = loadBitmap(folderPath, "mainmenu_button2.bmp");
    mainmenu_button3 = loadBitmap(folderPath, "mainmenu_button3.bmp");
    mainmenu_button4 = loadBitmap(folderPath, "mainmenu_button4.bmp");
    mainmenu_button1_hl = loadBitmap(folderPath, "mainmenu_button1_hl.bmp");
    mainmenu_button2_hl = loadBitmap(folderPath, "mainmenu_button2_hl.bmp");
    mainmenu_button3_hl = loadBitmap(folderPath, "mainmenu_button3_hl.bmp");
    mainmenu_button4_hl = loadBitmap(folderPath, "mainmenu_button4_hl.bmp");

    snake_button = loadBitmap(folderPath, "snake_button.bmp");
    snake_button_hl = loadBitmap(folderPath, "snake_button_hl.bmp");
    bird_button = loadBitmap(folderPath, "bird_button.bmp");
    bird_button_hl = loadBitmap(folderPath, "bird_button_hl.bmp");

    // Drawing mode buttons
    rainbow_button = loadBitmap(folderPath, "rainbow.bmp");
    rainbow_hl = loadBitmap(folderPath, "rainbow_hl.bmp");
    
    balde_button = loadBitmap(folderPath, "balde.bmp");
    balde_hl = loadBitmap(folderPath, "balde_hl.bmp");
    
    picker_button = loadBitmap(folderPath, "picker.bmp");
    picker_hl = loadBitmap(folderPath, "picker_hl.bmp");
    
    brush_button = loadBitmap(folderPath, "brush.bmp");
    brush_hl = loadBitmap(folderPath, "brush_hl.bmp");

    trash_button = loadBitmap(folderPath, "trash.bmp");
    trash_hl = loadBitmap(folderPath, "trash_hl.bmp");

    rubber_button = loadBitmap(folderPath, "rubber.bmp");
    rubber_hl = loadBitmap(folderPath, "rubber_hl.bmp");

    palette_button = loadBitmap(folderPath, "palette.bmp");
    palette_hl = loadBitmap(folderPath, "palette_hl.bmp");

    slider_bmp = loadBitmap(folderPath, "slider.bmp");

    emote_icon = loadBitmap(folderPath, "emote_wheel_icon.bmp");
    emote_icon_hl = loadBitmap(folderPath, "emote_wheel_icon_hl.bmp");

    // Training mode buttons

    save_button = loadBitmap(folderPath, "save.bmp");
    save_hl = loadBitmap(folderPath, "save_hl.bmp");
    load_button = loadBitmap(folderPath, "load.bmp");
    load_hl = loadBitmap(folderPath, "load_hl.bmp");

    // coisas da interface
    letters_bmp = loadBitmap(folderPath, "letras.bmp");
    textbox1_bmp = loadBitmap(folderPath, "wordbox1.bmp");
    textbox2_bmp = loadBitmap(folderPath, "wordbox2.bmp");
    textbox3_bmp = loadBitmap(folderPath, "wordbox3.bmp");
    textbox4_bmp = loadBitmap(folderPath, "wordbox4.bmp");
    textbox5_bmp = loadBitmap(folderPath, "wordbox5.bmp");
    textbox6_bmp = loadBitmap(folderPath, "wordbox6.bmp");

    colorpicker_bmp = loadBitmap(folderPath, "colorpicker.bmp");
    clock_bmp = loadBitmap(folderPath, "clock.bmp");
    clockbig_bmp = loadBitmap(folderPath, "clockbig.bmp");
    
    // Cursor
    cursor_normal = loadBitmap(folderPath, "cool_cursor.bmp");
    cursor_brush = loadBitmap(folderPath, "cursor_brush.bmp");
    cursor_balde = loadBitmap(folderPath, "cursor_balde.bmp");
    cursor_picker = loadBitmap(folderPath, "cursor_picker.bmp");
    cursor_rubber = loadBitmap(folderPath, "cursor_rubber.bmp");
    cursor_rainbow = loadBitmap(folderPath, "cursor_rainbow.bmp");

    // NNY
    paint_tube_1_bmp = loadBitmap(folderPath, "painttube_1.bmp");
    paint_tube_2_bmp = loadBitmap(folderPath, "painttube_2.bmp");

    emote0_bmp = loadBitmap(folderPath, "emote0.bmp");
    emote1_bmp = loadBitmap(folderPath, "emote1.bmp");
    emote2_bmp = loadBitmap(folderPath, "emote2.bmp");
    emote3_bmp = loadBitmap(folderPath, "emote3.bmp");
    emote4_bmp = loadBitmap(folderPath, "emote4.bmp");

    emote0_hl_bmp = loadBitmap(folderPath, "emote0_hl.bmp");
    emote1_hl_bmp = loadBitmap(folderPath, "emote1_hl.bmp");
    emote2_hl_bmp = loadBitmap(folderPath, "emote2_hl.bmp");
    emote3_hl_bmp = loadBitmap(folderPath, "emote3_hl.bmp");
    emote4_hl_bmp = loadBitmap(folderPath, "emote4_hl.bmp");



    
    emote_wheel_bmp = loadBitmap(folderPath, "emote_wheel.bmp");

    // SNAAKE
    obstacle0_bmp = loadBitmap(folderPath, "obstacle0.bmp");
    obstacle1_bmp = loadBitmap(folderPath, "obstacle1.bmp");
    obstacle2_bmp = loadBitmap(folderPath, "obstacle2.bmp");
    obstacle3_bmp = loadBitmap(folderPath, "obstacle3.bmp");

    powerup0_bmp = loadBitmap(folderPath, "powerup0.bmp");
    powerup1_bmp = loadBitmap(folderPath, "powerup1.bmp");
    powerup2_bmp = loadBitmap(folderPath, "powerup2.bmp");
    powerup3_bmp = loadBitmap(folderPath, "powerup3.bmp");
    powerup4_bmp = loadBitmap(folderPath, "powerup4.bmp");
    powerup5_bmp = loadBitmap(folderPath, "powerup5.bmp");
    powerup6_bmp = loadBitmap(folderPath, "powerup6.bmp");

    hpbar_bmp = loadBitmap(folderPath, "hp_bar.bmp");

    // FLAPPY
    flappy_tube0_bmp = loadBitmap(folderPath, "flappy_tube0.bmp");
    flappy_tube1_bmp = loadBitmap(folderPath, "flappy_tube1.bmp");
    flappy_tube2_bmp = loadBitmap(folderPath, "flappy_tube2.bmp");
    flappy_tube3_bmp = loadBitmap(folderPath, "flappy_tube3.bmp");
}

