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
Bitmap* save_button;
Bitmap* save_hl;
Bitmap* load_button;
Bitmap* load_hl;

Bitmap* textbox_bmp;
Bitmap* wide_textbox_bmp;
Bitmap* ultrawide_textbox_bmp;
Bitmap* megalarge_textbox_bmp;
Bitmap* ultrawide_textbox2_bmp;

Bitmap* colorpicker_bmp;
Bitmap* clock_bmp;
Bitmap* paint_tube_1_bmp;
Bitmap* paint_tube_2_bmp;

Bitmap* cool_dude_happy;
Bitmap* cool_dude_angry;
Bitmap* cool_dude_happy_big;
Bitmap* cool_dude_angry_big;

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

Bitmap* flappy_tube0_bmp;
Bitmap* flappy_tube1_bmp;
Bitmap* flappy_tube2_bmp;
Bitmap* flappy_tube3_bmp;

void loadBitmaps() {
    // Background
    //ULTRA_BACKGROUND = loadBitmap("cool_dude.bmp");
    background_menu = loadBitmap("background_menu.bmp");
    welcome_screen = loadBitmap("background_menu_start.bmp");
    // Menu Buttons
    mainmenu_button1 = loadBitmap("mainmenu_button1.bmp");

    mainmenu_button2 = loadBitmap("mainmenu_button2.bmp");
    mainmenu_button3 = loadBitmap("mainmenu_button3.bmp");
    mainmenu_button4 = loadBitmap("mainmenu_button4.bmp");
    mainmenu_button1_hl = loadBitmap("mainmenu_button1_hl.bmp");
    mainmenu_button2_hl = loadBitmap("mainmenu_button2_hl.bmp");
    mainmenu_button3_hl = loadBitmap("mainmenu_button3_hl.bmp");
    mainmenu_button4_hl = loadBitmap("mainmenu_button4_hl.bmp");

    snake_button = loadBitmap("snake_button.bmp");
    snake_button_hl = loadBitmap("snake_button_hl.bmp");

    // Drawing mode buttons
    rainbow_button = loadBitmap("rainbow.bmp");
    rainbow_hl = loadBitmap("rainbow_hl.bmp");
    
    balde_button = loadBitmap("balde.bmp");
    balde_hl = loadBitmap("balde_hl.bmp");
    
    picker_button = loadBitmap("picker.bmp");
    picker_hl = loadBitmap("picker_hl.bmp");
    
    brush_button = loadBitmap("brush.bmp");
    brush_hl = loadBitmap("brush_hl.bmp");

    trash_button = loadBitmap("trash.bmp");
    trash_hl = loadBitmap("trash_hl.bmp");

    rubber_button = loadBitmap("rubber.bmp");
    rubber_hl = loadBitmap("rubber_hl.bmp");

    palette_button = loadBitmap("palette.bmp");
    palette_hl = loadBitmap("palette_hl.bmp");

    // Training mode buttons

    save_button = loadBitmap("save.bmp");
    save_hl = loadBitmap("save_hl.bmp");
    load_button = loadBitmap("load.bmp");
    load_hl = loadBitmap("load_hl.bmp");

    // Cool dude!

    cool_dude_happy = loadBitmap("cool_dude_happy.bmp");
    cool_dude_angry = loadBitmap("cool_dude_angry.bmp");

    cool_dude_happy_big = resizeBitmap(cool_dude_happy, 2);
    cool_dude_angry_big = resizeBitmap(cool_dude_angry, 2);

    // coisas da interface
    textbox_bmp = loadBitmap("wordbox1.bmp");
    wide_textbox_bmp = loadBitmap("wordbox2.bmp");
    ultrawide_textbox_bmp = loadBitmap("wordbox3.bmp");
    megalarge_textbox_bmp = loadBitmap("wordbox4.bmp");
    ultrawide_textbox2_bmp = loadBitmap("wordbox5.bmp");

    colorpicker_bmp = loadBitmap("colorpicker.bmp");
    clock_bmp = loadBitmap("clock.bmp");
    
    // Cursor
    cursor_normal = loadBitmap("cool_cursor.bmp");
    cursor_brush = loadBitmap("cursor_brush.bmp");
    cursor_balde = loadBitmap("cursor_balde.bmp");
    cursor_picker = loadBitmap("cursor_picker.bmp");
    cursor_rubber = loadBitmap("cursor_rubber.bmp");
    cursor_rainbow = loadBitmap("cursor_rainbow.bmp");

    // NNY
    paint_tube_1_bmp = loadBitmap("painttube_1.bmp");
    paint_tube_2_bmp = loadBitmap("painttube_2.bmp");

    // SNAAKE
    obstacle0_bmp = loadBitmap("obstacle0.bmp");
    obstacle1_bmp = loadBitmap("obstacle1.bmp");
    obstacle2_bmp = loadBitmap("obstacle2.bmp");
    obstacle3_bmp = loadBitmap("obstacle3.bmp");

    powerup0_bmp = loadBitmap("powerup0.bmp");
    powerup1_bmp = loadBitmap("powerup1.bmp");
    powerup2_bmp = loadBitmap("powerup2.bmp");
    powerup3_bmp = loadBitmap("powerup3.bmp");
    powerup4_bmp = loadBitmap("powerup4.bmp");

    hpbar_bmp = loadBitmap("hp_bar.bmp");

    // FLAPPY
    flappy_tube0_bmp = loadBitmap("flappy_tube0.bmp");
    flappy_tube1_bmp = loadBitmap("flappy_tube1.bmp");
    flappy_tube2_bmp = loadBitmap("flappy_tube2.bmp");
    flappy_tube3_bmp = loadBitmap("flappy_tube3.bmp");
}

