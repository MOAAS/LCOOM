#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "projeto.h"


static GameState gameState = MainMenu;
static DrawingState* pencil;

static Sprite* cursor;

static Bitmap* cursor_normal;
static Bitmap* cursor_brush;
static Bitmap* cursor_rainbow;
static Bitmap* cursor_balde;
static Bitmap* cursor_picker;
static Bitmap* cursor_rubber;


static Layer* background;
static Layer* textbox_layer;
static Layer* color_picker_layer;

static Bitmap* ULTRA_BACKGROUND;

static Bitmap* mainmenu_button1;
static Bitmap* mainmenu_button1_hl;
static Bitmap* mainmenu_button2;
static Bitmap* mainmenu_button2_hl;
static Bitmap* mainmenu_button3;
static Bitmap* mainmenu_button3_hl;

static Bitmap* rainbow_button;
static Bitmap* rainbow_hl;
static Bitmap* balde_button;
static Bitmap* balde_hl;
static Bitmap* picker_button;
static Bitmap* picker_hl;
static Bitmap* brush_button;
static Bitmap* brush_hl;
static Bitmap* trash_button;
static Bitmap* trash_hl;
static Bitmap* rubber_button;
static Bitmap* rubber_hl;
static Bitmap* color_button;
static Bitmap* color_hl;

static Bitmap* textbox_bmp;
static Bitmap* colorpicker_bmp;
static Bitmap* clock_bmp;
static Bitmap* paint_tube_1_bmp;
static Bitmap* paint_tube_2_bmp;

static Bitmap* thumbsup_bmp;
static Bitmap* thumbsdown_bmp;

static Event_t event;

static NNY* paint_tube_1;
static NNY* paint_tube_2;
static int num_paint_buttons = 7;
static int num_menu_buttons = 3;
static Button* paint_buttons[7];
static Button* menu_buttons[3];

void loadBitmaps() {
    // Background
    ULTRA_BACKGROUND = loadBitmap("home/lcom/labs/proj/bitmaps/cool_dude.bmp");
    // Buttons
    mainmenu_button1 = loadBitmap("home/lcom/labs/proj/bitmaps/mainmenu_button1.bmp");
    mainmenu_button1_hl = loadBitmap("home/lcom/labs/proj/bitmaps/mainmenu_button1_hl.bmp");
    mainmenu_button2 = loadBitmap("home/lcom/labs/proj/bitmaps/mainmenu_button2.bmp");
    mainmenu_button2_hl = loadBitmap("home/lcom/labs/proj/bitmaps/mainmenu_button2_hl.bmp");
    mainmenu_button3 = loadBitmap("home/lcom/labs/proj/bitmaps/mainmenu_button3.bmp");
    mainmenu_button3_hl = loadBitmap("home/lcom/labs/proj/bitmaps/mainmenu_button3_hl.bmp");

    rainbow_button = loadBitmap("home/lcom/labs/proj/bitmaps/rainbow.bmp");
    rainbow_hl = loadBitmap("home/lcom/labs/proj/bitmaps/rainbow_hl.bmp");
    
    balde_button = loadBitmap("home/lcom/labs/proj/bitmaps/balde.bmp");
    balde_hl = loadBitmap("home/lcom/labs/proj/bitmaps/balde_hl.bmp");
    
    picker_button = loadBitmap("home/lcom/labs/proj/bitmaps/picker.bmp");
    picker_hl = loadBitmap("home/lcom/labs/proj/bitmaps/picker_hl.bmp");
    
    brush_button = loadBitmap("home/lcom/labs/proj/bitmaps/brush.bmp");
    brush_hl = loadBitmap("home/lcom/labs/proj/bitmaps/brush_hl.bmp");

    trash_button = loadBitmap("home/lcom/labs/proj/bitmaps/trash.bmp");
    trash_hl = loadBitmap("home/lcom/labs/proj/bitmaps/trash_hl.bmp");

    rubber_button = loadBitmap("home/lcom/labs/proj/bitmaps/rubber.bmp");
    rubber_hl = loadBitmap("home/lcom/labs/proj/bitmaps/rubber_hl.bmp");

    color_button = loadBitmap("home/lcom/labs/proj/bitmaps/color.bmp");
    color_hl = loadBitmap("home/lcom/labs/proj/bitmaps/color_hl.bmp");

    thumbsup_bmp =  loadBitmap("home/lcom/labs/proj/bitmaps/thumbsup.bmp");
    thumbsdown_bmp = loadBitmap("home/lcom/labs/proj/bitmaps/thumbsdown.bmp");

    textbox_bmp = loadBitmap("home/lcom/labs/proj/bitmaps/textbox.bmp");
    colorpicker_bmp = loadBitmap("home/lcom/labs/proj/bitmaps/colorpicker.bmp");

    clock_bmp = loadBitmap("home/lcom/labs/proj/bitmaps/clock.bmp");
    
    cursor_normal = loadBitmap("home/lcom/labs/proj/bitmaps/cool_cursor.bmp");
    cursor_brush = loadBitmap("home/lcom/labs/proj/bitmaps/cursor_brush.bmp");
    cursor_balde = loadBitmap("home/lcom/labs/proj/bitmaps/cursor_balde.bmp");
    cursor_picker = loadBitmap("home/lcom/labs/proj/bitmaps/cursor_picker.bmp");
    cursor_rubber = loadBitmap("home/lcom/labs/proj/bitmaps/cursor_rubber.bmp");
    cursor_rainbow = loadBitmap("home/lcom/labs/proj/bitmaps/cursor_rainbow.bmp");

    paint_tube_1_bmp = loadBitmap("home/lcom/labs/proj/bitmaps/painttube_1.bmp");
    paint_tube_2_bmp = loadBitmap("home/lcom/labs/proj/bitmaps/painttube_2.bmp");

    // Sprites
    cursor = create_sprite(cursor_normal, 0, 0);
    pencil = create_pencil(cursor);
}

void loadButtons() {
    paint_buttons[0] = create_button(10, 470, background, brush_button, brush_hl); // normal
    paint_buttons[1] = create_button(10, 550, background, balde_button, balde_hl); // bucket
    paint_buttons[2] = create_button(10, 630, background, picker_button, picker_hl); // color coiso
    paint_buttons[3] = create_button(80, 470, background, rainbow_button, rainbow_hl); // rainbow
    paint_buttons[4] = create_button(80, 550, background, rubber_button, rubber_hl); // borracha
    paint_buttons[5] = create_button(80, 630, background, trash_button, trash_hl); // trash
    paint_buttons[6] = create_button(45, 360, background, color_button, color_hl); //color picker
    paint_buttons[5]->singleState = true; // trash so tem um estado
    paint_buttons[6]->singleState = true; // trash so tem um estado

    menu_buttons[0] = create_button(300, 150, background, mainmenu_button1, mainmenu_button1_hl);
    menu_buttons[1] = create_button(300, 350, background, mainmenu_button2, mainmenu_button2_hl);
    menu_buttons[2] = create_button(300, 550, background, mainmenu_button3, mainmenu_button3_hl);
    menu_buttons[0]->singleState = true; // so tem um estado
    menu_buttons[1]->singleState = true; // so tem um estado
    menu_buttons[2]->singleState = true; // so tem um estado
}

void loadBackground() {
    background = create_layer(0, 0, vg_get_hres(), vg_get_vres());
    layer_draw_image(background, ULTRA_BACKGROUND, 0, 0);
}

void projeto() {
    loadBitmaps();
    loadLetterMap();    
    loadDictionary();
    video_init(0x118);
    loadBackground();
    loadButtons();
    subscribe_device(SerialPort);
    subscribe_device(Mouse);
    subscribe_device(Keyboard);
    subscribe_device(Timer);
    draw_sprite_color(cursor);
    while(gameState != ExitGame) {
        switch(gameState) {
            case MainMenu: main_menu(); break;
            case Drawing: draw(); break;
            case Guessing: guess(); break;
            case TypingGuess: type_guess(); break;
            case Saving: save(); break;
            case EndGameWin: end_game(); break;
            case EndGameLoss: end_game(); break;
            case ExitGame: break;
        }
    }
    vg_exit();
    unsubscribe_device(Timer);
    unsubscribe_device(Keyboard);
    unsubscribe_device(Mouse);
    unsubscribe_device(SerialPort);
}


void main_menu() {
    layer_draw_image(background, ULTRA_BACKGROUND, 0, 0);
    draw_buttons(menu_buttons, num_menu_buttons);
    while (1) {
        event = GetEvent();
        // isto e temporario caso algo crashe
        if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS) {
            gameState = ExitGame;
            return;
        }
        switch (checkButtonPress(event, cursor, menu_buttons, num_menu_buttons)) {
            case 0: 
                gameState = Drawing;  
                start_game(); 
                disable_buttons(menu_buttons, num_menu_buttons);
                return;
            case 1: 
                gameState = Guessing; 
                start_game(); 
                disable_buttons(menu_buttons, num_menu_buttons);
                return;
            case 2: gameState = ExitGame; return;
            case -1: break;
        }
        update_cursor(cursor, event);
    }
}

void start_game() {
    create_clock(clock_bmp, 90);
    create_canvas(background, 150, 150, 1000, 750, WHITE);
    char* solution = get_random_word();
    if (gameState == Drawing) {      
        setupDrawing();
        word_pick_start(background, solution, true);
    }
    else word_pick_start(background, solution, false);
}

void end_game() {
    word_pick_end();
    destroy_canvas();
    destroy_clock();
    Bitmap* wordbox_bmp = loadBitmap("home/lcom/labs/proj/bitmaps/wordbox.bmp");
    layer_draw_image(background, ULTRA_BACKGROUND, 0, 0);
    layer_draw_image(background, wordbox_bmp, 100, 200); // 860x100 image
    if (gameState == EndGameWin) {
        draw_word(background, "You win!", 530, 225, 3, 2, Center);
        layer_draw_image(background, thumbsup_bmp, 300, 300);
    }
    else {
        draw_word(background, "You lose!", 530, 225, 3, 2, Center);
        layer_draw_image(background, thumbsdown_bmp, 300, 300);
    }
    timer_reset_counter();
    change_tool(pencil, None);
    while(1) {
        event = GetEvent();
        if (event.isTimerEvent && event.timerEvent.seconds_passed == 3)
            break;
        update_cursor(cursor, event);
    }
    gameState = MainMenu;
    
}

//////////////
// GUESSING //
//////////////

void guess() {
    while(gameState == Guessing) {
        event = GetEvent();
        update_clock(event);
        update_cursor(cursor, event);
        if (clock_get_time_left() == 0)
            gameState = EndGameLoss;
        if (event.isTimerEvent && event.timerEvent.has_second_passed && event.timerEvent.seconds_passed % 12 == 0)
            reveal_letter();
        if (event.isUARTEvent) {
            for (int i = 0; i < event.num_uart_messages; i++) {
                uart_receive_draw_line1(&event.uart_messages[i]);
            }
        }
        if (event.isKeyboardEvent) {
            switch(event.keyboardEvent.type) {
                case ESC_PRESS: gameState = ExitGame; break;
                case ENTER_PRESS: gameState = TypingGuess; break;
                default: break;
            }
        }
    }
}

void type_guess() {
    textbox_layer = create_layer(300, 300, 400, 200);
    TextBox* textbox = create_textbox(textbox_layer, textbox_bmp, 300, 300, 10, 5, 1);
    textbox_write(textbox, "Insert text here...");
    while(gameState == TypingGuess) {
        event = GetEvent();
        update_clock(event);
        update_cursor(cursor, event);
        if (clock_get_time_left() == 0)
            gameState = EndGameLoss;
        if (event.isTimerEvent && event.timerEvent.has_second_passed && event.timerEvent.seconds_passed % 1 == 0)
            reveal_letter();
        useTextbox(textbox, event, Guessing, Guessing);
    }
    if (verify_guess(textbox->text)) // if guess is correct
        gameState = EndGameWin;
    destroy_textbox(textbox);
}

void useTextbox(TextBox* textbox, Event_t event, GameState previous_state, GameState next_state) {
    if (!event.isKeyboardEvent)
        return;
    switch(event.keyboardEvent.type) {
        case CHARACTER_PRESS: 
            if (textbox->isEmpty) {
                textbox->isEmpty = false;
                textbox_clear(textbox);
            }
            textbox_put(textbox, event.keyboardEvent.character); 
            return;
        case BACKSPACE_PRESS: 
            if (textbox->isEmpty) {
                textbox->isEmpty = false;
                textbox_clear(textbox);
            }
            else textbox_backspace(textbox);
            return;
        case ESC_PRESS:
            textbox_clear(textbox); 
            gameState = previous_state; 
            return;
        case ENTER_PRESS: gameState = next_state; return;
        default: return;
    }
}

/////////////
// DRAWING //
/////////////

DrawingState* create_pencil(Sprite* cursor) {
    DrawingState* pencil = malloc(sizeof(DrawingState));
    pencil->color1 = RED;
    pencil->color2 = WHITE;
    pencil->tool = None;
    pencil->thickness = 4;
    pencil->rainbowColor= RED;
    pencil->cursor = cursor;
    return pencil;
}

void setupDrawing() {
    paint_tube_1 = create_NNY(45, 280, background, paint_tube_1_bmp);
    paint_tube_2 = create_NNY(79, 283, background, paint_tube_2_bmp);
    nny_change_color(paint_tube_1, pencil->color1);
    nny_change_color(paint_tube_2, pencil->color2);
    draw_buttons(paint_buttons, num_paint_buttons);
    change_tool(pencil, Brush);
    press_button(paint_buttons[0]); // Brush
}

void color_picker() {    
    color_picker_layer = create_layer(45, 440, 538, 323);
    layer_draw_image(color_picker_layer, colorpicker_bmp, 45, 440);
    while(1) {
        event = GetEvent();
        update_clock(event);
        update_cursor(cursor, event);
        if(event.isMouseEvent && (event.mouseEvent.type == LB_PRESS || event.mouseEvent.type == RB_PRESS)) {
            if (is_within_bounds(color_picker_layer, cursor->x, cursor->y)) {
                if (event.mouseEvent.type == LB_PRESS)
                    pencil->color1 = layer_get_pixel(color_picker_layer, cursor->x, cursor->y);
                else pencil->color2 = layer_get_pixel(color_picker_layer, cursor->x, cursor->y);  
            }
            break;
        }
    }
    change_tool(pencil, pencil->tool); // atualiza os paint tubes
    destroy_layer(color_picker_layer);
    unpress_button(paint_buttons[6]);
}


void draw() {
    while (1) {
        event = GetEvent();
        switch (checkButtonPress(event, cursor, paint_buttons, num_paint_buttons)) {
            case 0: change_tool(pencil, Brush); break;
            case 1: change_tool(pencil, Bucket);break;
            case 2: change_tool(pencil, ColorPicker); break;
            case 3: change_tool(pencil, Rainbow); break;
            case 4: change_tool(pencil, Rubber); break;
            case 5: canvas_set_color(WHITE); break;
            case 6: color_picker(); break;
            case -1: usePencil(pencil, event); break; // Se um botao for carregado o pincel nao e usado (rimou) xd
        }
        update_clock(event);
        update_cursor(cursor, event);
        if (event.isKeyboardEvent) {
            switch(event.keyboardEvent.type) {
                case ESC_PRESS: gameState = ExitGame; return;
                case RSHIFT_PRESS: increaseThickness(pencil); break;
                case LSHIFT_PRESS:  decreaseThickness(pencil); break;
                default: break;
            }
        }
    }
}


void usePencil(DrawingState* pencil, Event_t event) {
    if (!event.isMouseEvent)
        return;
    int xf = cursor_get_xf(cursor, event.mouseEvent.delta_x);
    int yf = cursor_get_yf(cursor, -event.mouseEvent.delta_y);
    uint32_t color_under;
    switch (pencil->tool) {
        case Brush:
            if (event.isLBPressed) {
                canvas_draw_line1(cursor->x, cursor->y, xf, yf, pencil->color1, pencil->thickness);
                uart_send_draw_line1(cursor->x, cursor->y, xf, yf, pencil->color1, pencil->thickness);
            }
            else if (event.isRBPressed)
                canvas_draw_line1(cursor->x, cursor->y, xf, yf, pencil->color2, pencil->thickness);
            break;
        case Rainbow: 
            if (!event.isLBPressed && !event.isRBPressed)
                break;
            rainbow_draw(pencil, cursor->x, cursor->y, xf, yf);
            sprite_set_color(cursor, pencil->rainbowColor);
            break;
        case ColorPicker:
            if (event.mouseEvent.type == LB_PRESS || event.mouseEvent.type == RB_PRESS) {
                if (event.mouseEvent.type == LB_PRESS)
                    pencil->color1 = layer_get_pixel(background, cursor->x, cursor->y);
                else pencil->color2 = layer_get_pixel(background, cursor->x, cursor->y);
                pencil->tool = Brush;
                change_tool(pencil, Brush);
                press_button(paint_buttons[0]); // Brush
                unpress_button(paint_buttons[2]);
            }
            break;
        case Bucket:
            color_under = layer_get_pixel(background, cursor->x, cursor->y);
			if (event.mouseEvent.type == LB_PRESS)
				bucket_tool(cursor->x, cursor->y, color_under, pencil->color1);
            else if (event.mouseEvent.type == RB_PRESS)
                bucket_tool(cursor->x, cursor->y, color_under, pencil->color2);
            break;
        case Rubber:
            if (event.isLBPressed)
                canvas_draw_line2(cursor->x, cursor->y, xf, yf, pencil->color2, pencil->thickness + 2);
            else if (event.isRBPressed)
                canvas_draw_line2(cursor->x, cursor->y, xf, yf, pencil->color1, pencil->thickness + 2);
            break;

        default: break;
    }
}

void rainbow_draw(DrawingState* pencil, uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf) {
    pencil->rainbowColor = rainbow();
    nny_change_color(paint_tube_1, pencil->rainbowColor);
    nny_change_color(paint_tube_2, pencil->rainbowColor);
    canvas_draw_line1(x0, y0, xf, yf, pencil->rainbowColor, pencil->thickness);
}

void change_tool(DrawingState* pencil, Tool tool) {
    switch (tool) {
        case Brush: cursor_change_bmp(cursor,cursor_brush);  break;
        case Bucket: cursor_change_bmp(cursor,cursor_balde);break;
        case ColorPicker: cursor_change_bmp(cursor,cursor_picker); break;
        case Rainbow: cursor_change_bmp(cursor, cursor_rainbow); break;
        case Rubber: cursor_change_bmp(cursor, cursor_rubber); break;
        case None: cursor_change_bmp(cursor, cursor_normal); break;
    } 
    if (tool == Rainbow) {
        nny_change_color(paint_tube_1, pencil->rainbowColor);
        nny_change_color(paint_tube_2, pencil->rainbowColor);  
        sprite_set_color(cursor, pencil->rainbowColor);
    }
    else if (tool != None) {
        nny_change_color(paint_tube_1, pencil->color1);
        nny_change_color(paint_tube_2, pencil->color2);
        sprite_set_color(cursor, pencil->color1);
    }
    draw_sprite_color(cursor);
    pencil->tool = tool;
}

void increaseThickness(DrawingState* pencil) {
    if (pencil->thickness < 10)
        pencil->thickness++;
}

void decreaseThickness(DrawingState* pencil) {
    if (pencil->thickness > 1)
        pencil->thickness--;
}

////////////
// Saving //
////////////

void save() {
    textbox_layer = create_layer(300, 300, 400, 200);
    TextBox* textbox = create_textbox(textbox_layer, textbox_bmp, 300, 300, 10, 5, 1);
    textbox_write(textbox, "Insert file name...");
    while(gameState == Saving) {
        event = GetEvent();
        update_cursor(cursor, event);
        useTextbox(textbox, event, Drawing, Drawing);
    }
    if (strcmp(textbox->text, "") != 0) {
        char path[500] = "home/lcom/labs/proj/";
        strcat(path, textbox->text);
        strcat(path, ".bmp");
        char* canvas_map = canvas_get_map();
        saveBitmap(path, canvas_get_width(), canvas_get_height(), canvas_map);    
        free(canvas_map);    
    }
    destroy_textbox(textbox);

}
