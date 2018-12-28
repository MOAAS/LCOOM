#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "projeto.h"

/////////////
// Bitmaps //
/////////////

extern Bitmap* cursor_normal;
extern Bitmap* cursor_brush;
extern Bitmap* cursor_rainbow;
extern Bitmap* cursor_balde;
extern Bitmap* cursor_picker;
extern Bitmap* cursor_rubber;

//extern Bitmap* ULTRA_BACKGROUND;
extern Bitmap* welcome_screen;
extern Bitmap* background_menu;

extern Bitmap* mainmenu_button1;
extern Bitmap* mainmenu_button2;
extern Bitmap* mainmenu_button3;
extern Bitmap* mainmenu_button4;

extern Bitmap* mainmenu_button1_hl;
extern Bitmap* mainmenu_button2_hl;
extern Bitmap* mainmenu_button3_hl;
extern Bitmap* mainmenu_button4_hl;


extern Bitmap* snake_button;
extern Bitmap* snake_button_hl;
extern Bitmap* bird_button;
extern Bitmap* bird_button_hl;

extern Bitmap* rainbow_button;
extern Bitmap* rainbow_hl;
extern Bitmap* balde_button;
extern Bitmap* balde_hl;
extern Bitmap* picker_button;
extern Bitmap* picker_hl;
extern Bitmap* brush_button;
extern Bitmap* brush_hl;
extern Bitmap* trash_button;
extern Bitmap* trash_hl;
extern Bitmap* rubber_button;
extern Bitmap* rubber_hl;
extern Bitmap* palette_button;
extern Bitmap* palette_hl;

extern Bitmap* paint_tube_1_bmp;
extern Bitmap* paint_tube_2_bmp;

extern Bitmap* save_button;
extern Bitmap* save_hl;
extern Bitmap* load_button;
extern Bitmap* load_hl;

extern Bitmap* textbox_bmp;
extern Bitmap* wide_textbox_bmp;
extern Bitmap* ultrawide_textbox_bmp;
extern Bitmap* megalarge_textbox_bmp;
extern Bitmap* colorpicker_bmp;
extern Bitmap* clock_bmp;
extern Bitmap* clockbig_bmp;


extern Bitmap* cool_dude_happy;
extern Bitmap* cool_dude_angry;
extern Bitmap* cool_dude_happy_big;
extern Bitmap* cool_dude_angry_big;

static IdleSprite* paint_tube_1;
static IdleSprite* paint_tube_2;

static int num_menu_buttons = 6;
static int num_paint_buttons = 7;
static int num_training_buttons = 2;
static int num_wordgame_buttons = 1;

static Button* paint_buttons[7];
static Button* menu_buttons[10];
static Button* training_buttons[5];
static Button* wordgame_buttons[5];

static Slider* thickness_slider;

/////////////////
// Buttons end //
/////////////////

static GameState gameState = Start;
static GameState lastGameState = Start;

static DrawingState* pencil;

static Sprite* cursor;

static Layer* background;
static Layer* textbox_layer;
static Layer* color_picker_layer;

static TextBox* big_ben1;
static TextBox* big_ben2;

static Event_t event;

char* words_path = "home/lcom/labs/proj/palavras/palavras.txt";
char* snake_hs_path = "home/lcom/labs/proj/highscores/snake.txt";
char* flappy_hs_path = "home/lcom/labs/proj/highscores/flappy.txt";
char* wordgame_hs_path = "home/lcom/labs/proj/highscores/wordgame.txt";
char* bitmap_folder = "home/lcom/labs/proj/bitmaps/";
char* saved_img_folder = "home/lcom/labs/proj/saved_images/";


void loadCursor() {
    cursor = create_sprite(cursor_normal, 0, 0);
    pencil = create_pencil(cursor);
}

void loadButtons() {
    paint_buttons[0] = create_button(5, 470, background, brush_button, brush_hl); // normal
    paint_buttons[1] = create_button(5, 550, background, balde_button, balde_hl); // bucket
    paint_buttons[2] = create_button(5, 630, background, picker_button, picker_hl); // color coiso
    paint_buttons[3] = create_button(75, 470, background, rainbow_button, rainbow_hl); // rainbow
    paint_buttons[4] = create_button(75, 550, background, rubber_button, rubber_hl); // borracha
    paint_buttons[5] = create_button(75, 630, background, trash_button, trash_hl); // trash
    paint_buttons[6] = create_button(45, 400, background, palette_button, palette_hl); //color picker
    paint_buttons[5]->singleState = true; // trash
    paint_buttons[6]->singleState = true; // color picker

    menu_buttons[0] = create_button(200, 266, background, mainmenu_button1, mainmenu_button1_hl);
    menu_buttons[1] = create_button(305, 499, background, mainmenu_button2, mainmenu_button2_hl);
    menu_buttons[2] = create_button(540, 330, background, mainmenu_button3, mainmenu_button3_hl);
    menu_buttons[3] = create_button(40, 650, background, mainmenu_button4, mainmenu_button4_hl);
    menu_buttons[4] = create_button(703, 462, background, snake_button, snake_button_hl);
    menu_buttons[5] = create_button(758, 405, background, bird_button, bird_button_hl);
    menu_buttons[0]->singleState = true; 
    menu_buttons[1]->singleState = true; 
    menu_buttons[2]->singleState = true; 
    menu_buttons[3]->singleState = true; 
    menu_buttons[4]->singleState = true; 
    menu_buttons[5]->singleState = true; 

    training_buttons[0] = create_button(10, 100, background, save_button, save_hl); // save
    training_buttons[1] = create_button(77, 100, background, load_button, load_hl); // save
    training_buttons[0]->singleState = true; // save
    training_buttons[1]->singleState = true; // save

    wordgame_buttons[0] = create_button(10, 10, background, load_button, load_button);

    thickness_slider = create_slider(5, 700, 0.4, background, 0xBEC7ED, RED);
}

void loadBackground() {
    background = create_layer(0, 0, vg_get_hres(), vg_get_vres());
    paint_tube_1 = create_idle_sprite(45, 280, background, paint_tube_1_bmp, pencil->color1);
    paint_tube_2 = create_idle_sprite(79, 283, background, paint_tube_2_bmp, pencil->color2);
    layer_draw_image(background, welcome_screen, 0, 0);
}


void reloadBackground() {
    layer_draw_image(background, background_menu, 0, 0);
}

void changeState(GameState newState) {
    lastGameState = gameState;
    gameState = newState;
}

void projeto() {
    srand(time(NULL));
    loadBitmaps(bitmap_folder);
    loadCursor();
    loadLetterMap();    
    loadDictionary(words_path);
    wordgame_set_rounds(4);
    loadWordGameHighscore(wordgame_hs_path);
    loadSnakeHighscore(snake_hs_path);
    loadFlappyHighscore(flappy_hs_path);
    video_init(0x118);
    loadBackground();
    loadButtons();
    subscribe_device(RTC);
    subscribe_device(SerialPort);
    subscribe_device(Mouse);
    subscribe_device(Keyboard);
    subscribe_device(Timer);
    draw_sprite_color(cursor);
    while(gameState != ExitGame) {
        switch(gameState) {
            case Start: start_screen(); break;
            case MainMenu: main_menu(); break;
            case WaitDrawing: wait_for_guesser(); break;
            case WaitGuessing: wait_for_drawer(); break;
            case Drawing: draw(); break;
            case PickingColor: color_picker(); break;
            case Guessing: guess(); break;
            case TypingGuess: type_guess(); break;
            case Training: training(); break;
            case Saving: save(); break;
            case Loading: load(); break;
            case EndRoundWin: end_round(); break;
            case EndRoundLoss: end_round(); break;
            case EndGame: end_game(); break;
            case PlayingSnake: play_snake(); break;
            case PlayingFlappy: play_flappy(); break;
            case ExitGame: break;
        }
    }
    vg_exit();
    unsubscribe_device(Timer);
    unsubscribe_device(Keyboard);
    unsubscribe_device(Mouse);
    unsubscribe_device(SerialPort);
    unsubscribe_device(RTC);

}

void start_screen() {
    layer_draw_image(background, welcome_screen, 0, 0);
    create_big_ben();
    draw_word(background, "WeLCOM user!", vg_get_hres() / 2, 600, 4, 0, CenterAlign);
    draw_word(background, "Press enter to start!", vg_get_hres() / 2, 670, 2, 0, CenterAlign);
    while (gameState == Start) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isKeyboardEvent && event.keyboardEvent.type == ENTER_PRESS)
            changeState(MainMenu); 
        if (event.isRtcEvent) {
            update_big_ben(false);
        }
    }
    destroy_big_ben();
}


void main_menu() {
    reloadBackground();
    draw_buttons(cursor, menu_buttons, num_menu_buttons);
    reset_pencil(pencil); 
    while (gameState == MainMenu) {
        event = GetEvent();
        switch (checkButtonPress(event, cursor, menu_buttons, num_menu_buttons)) {
            case 0: changeState(WaitDrawing); break;
            case 1: changeState(WaitGuessing); break;
            case 2: 
                changeState(Training); 
                setup_training();
                break;
            case 3: changeState(ExitGame); break;
            case 4: changeState(PlayingSnake); break;
            case 5: changeState(PlayingFlappy); break;
            case -1: break;
        }
        update_cursor(cursor, event);
    }
}

// Big Ben //

void create_big_ben() {
    textbox_layer = create_layer(10, 10, 181, 181);
    big_ben1 = create_textbox(textbox_layer, clockbig_bmp, 36, 75, 1, CenterAlign);
    big_ben2 = create_textbox(textbox_layer, clockbig_bmp, 36, 106, 1, CenterAlign);
    update_big_ben(true);    
}

void destroy_big_ben() {
    free(big_ben2);
    destroy_textbox(big_ben1);
}

void update_big_ben(bool force) {
    char time1_str[100];
    char time2_str[100];
    Date d1 = rtc_get_date();
    if (force || d1.sec == 0) {
        sprintf(time1_str, "%02x/%02x/%02x", d1.day, d1.mth, d1.yer);
        sprintf(time2_str, "%02x:%02x:%02x", d1.hor, d1.min, d1.sec);
        textbox_clear(big_ben1);
        textbox_write(big_ben1, time1_str);
        textbox_clear(big_ben2);
        textbox_write(big_ben2, time2_str);
    }
    else {
        textbox_backspace(big_ben2);
        textbox_backspace(big_ben2);
        sprintf(time2_str, "%02x", d1.sec);
        textbox_write(big_ben2, time2_str);
    }
}

void start_round(char* solution) {
    if (gameState == WaitDrawing) {
        changeState(Drawing);
        setupDrawing();
        wordgame_start_round(background, solution, true);
    }
    else if (gameState == WaitGuessing) {
        changeState(Guessing);
        setupGuessing();
        wordgame_start_round(background, solution, false);
    }
    else changeState(MainMenu);
}

void end_round() {
    int points = 100 + wordgame_get_time_left() * 5 - 100 * (gameState == EndRoundLoss);
    destroy_canvas();
    destroy_emote();
    reset_pencil(pencil);
    reloadBackground();
    layer_draw_image(background, megalarge_textbox_bmp, 60, 250); // 860x100 image
    if (gameState == EndRoundWin) {
        if (lastGameState == TypingGuess || lastGameState == Guessing) {
            changeState(WaitDrawing);
            draw_word(background, "You guessed it!", 500, 300, 2, 0, CenterAlign);
        }
        else if (lastGameState == Drawing || lastGameState == PickingColor) {
            changeState(WaitGuessing);
            draw_word(background, "Your teammate guessed it!", 500, 300, 2, 0, CenterAlign);
        }
        else changeState(MainMenu);
        //layer_draw_image(background, cool_dude_happy_big, 500, 325);
    }
    else if (lastGameState == TypingGuess || lastGameState == Guessing) {
        changeState(WaitDrawing);
        draw_word(background, "You failed!", 500, 300, 2, 0, CenterAlign);
        //layer_draw_image(background, cool_dude_angry_big, 500, 325);
    }
    else if (lastGameState == Drawing || lastGameState == PickingColor) {
        changeState(WaitGuessing);
        draw_word(background, "Your teammate failed!", 500, 300, 2, 0, CenterAlign);
        //layer_draw_image(background, cool_dude_angry_big, 500, 325);
    }
    else changeState(MainMenu);
    char reveal[100], scoring[100];
    sprintf(reveal, "The word was: %s", get_solution());
    sprintf(scoring, "You got %d points!", points);
    draw_word(background, reveal, 500, 400, 2, 0, CenterAlign);
    draw_word(background, scoring, 500, 500, 2, 0, CenterAlign);
    wordgame_end_round(points);
    wait_ms(2000);
    if (is_wordgame_over())
        changeState(EndGame);
}

void end_game() {
    reloadBackground();
    wordgame_showstats();
    while(gameState == EndGame) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isKeyboardEvent && event.keyboardEvent.type == ENTER_PRESS)
            changeState(MainMenu); 
    }
    reset_wordgame();
}

void wait_for_guesser() {
    char* solution = get_random_word();
    layer_draw_image(background, wide_textbox_bmp, 85, 650); // 860x100 image
    draw_word(background, "Waiting for guesser...", 515, 700, 2, 0, CenterAlign);
    while (1) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isUARTEvent && get_msg_by_id(MSG_GUESSER_READY, event.uart_messages, event.num_uart_messages))
            break;
        if (is_wordgame_over() && event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS) {
            changeState(MainMenu); // Verifica DEPOIS de (nao) receber
            return;
        }
    }
    uart_send_drawer_ready(solution);
    start_round(solution);
}

void wait_for_drawer() {
    UARTMessage* msg; char* solution = NULL;
    layer_draw_image(background, wide_textbox_bmp, 85, 650); // 860x100 image
    draw_word(background, "Waiting for drawer...", 515, 700, 2, 0, CenterAlign);
    while (1) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (is_wordgame_over() && event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS) {
            changeState(MainMenu); // Verifica antes de receber
            return;
        }
        if (event.isTimerEvent && event.timerEvent.timer_counter % 30 == 0)
            uart_send_guesser_ready(); // manda de meio em meio segundo
        if (event.isUARTEvent && (msg = get_msg_by_id(MSG_DRAWER_READY, event.uart_messages, event.num_uart_messages))) {
            solution = malloc(strlen((char*)msg->bytes + 1) * sizeof(char));
            strcpy(solution, (char*)msg->bytes);
            break;
        }
    }
    start_round(solution);
    free(solution);
}


//////////////
// GUESSING //
//////////////

void setupGuessing() {
    reloadBackground();
    create_canvas(background, 150, 150, 1000, 750, WHITE);
}

void guess() {
    while(gameState == Guessing) {
        event = GetEvent();
        if (event.isTimerEvent && event.timerEvent.has_second_passed && event.timerEvent.seconds_passed % 12 == 0)
            reveal_letter();
        if (event.isKeyboardEvent && event.keyboardEvent.type == ENTER_PRESS)
            changeState(TypingGuess);
        if (event.isUARTEvent)
            uart_process_msgs(event.uart_messages, event.num_uart_messages);
        if (wordgame_time_up()) {
            changeState(EndRoundLoss);
            uart_send_game_loss();
            break;
        }
        update_cursor(cursor, event);
    }
}

void type_guess() {
    textbox_layer = create_layer(400, 300, 334, 135);
    TextBox* textbox = create_textbox(textbox_layer, textbox_bmp, 10, 10, 1, LeftAlign);
    textbox_write(textbox, "Insert guess...");
    while(gameState == TypingGuess) {
        event = GetEvent();
        if (event.isTimerEvent && event.timerEvent.has_second_passed && event.timerEvent.seconds_passed % 12 == 0)
            reveal_letter();
        if (event.isUARTEvent)
            uart_process_msgs(event.uart_messages, event.num_uart_messages);
        if (wordgame_time_up()) {
            changeState(EndRoundLoss);
            uart_send_game_loss();
            break;
        }
        useTextbox(textbox, event, Guessing, Guessing);
        update_cursor(cursor, event);
    }
    if (verify_guess(textbox->text) && !wordgame_time_up()) { // if guess is correct 
        changeState(EndRoundWin);
        uart_send_game_win();
    }
    destroy_textbox(textbox);
}

void useTextbox(TextBox* textbox, Event_t event, GameState previous_state, GameState next_state) {
    if (!event.isKeyboardEvent)
        return;
    if (textbox->isEmpty && event.keyboardEvent.type != ENTER_RELEASE) {
        textbox->isEmpty = false;
        textbox_clear(textbox);
    }
    switch(event.keyboardEvent.type) {
        case CHARACTER_PRESS: 
            textbox_put(textbox, event.keyboardEvent.character); 
            return;
        case BACKSPACE_PRESS: 
            textbox_backspace(textbox);
            return;
        case ESC_PRESS:
            changeState(previous_state); 
            return;
        case ENTER_PRESS: changeState(next_state); return;
        default: return;
    }
}

/////////////
// DRAWING //
/////////////

DrawingState* create_pencil(Sprite* cursor) {
    DrawingState* pencil = malloc(sizeof(DrawingState));
    pencil->color1 = BLACK;
    pencil->color2 = WHITE;
    pencil->tool = None;
    pencil->thickness = 4;
    pencil->rainbowColor= RED;
    pencil->cursor = cursor;
    return pencil;
}

void reset_pencil(DrawingState* pencil) {
    pencil->color1 = BLACK;
    pencil->color2 = WHITE;
    pencil->thickness = 4;
    thickness_slider->handle_pos = thickness_slider->x + + 0.4 * thickness_slider->width;
    pencil->rainbowColor= RED;
    change_tool(pencil, None);
}

void setupDrawing() {
    reloadBackground();
    create_canvas(background, 150, 150, 1000, 750, WHITE);
    draw_idle_sprite(paint_tube_1);
    draw_idle_sprite(paint_tube_2);
    draw_slider(thickness_slider);
    disable_buttons(paint_buttons, num_paint_buttons);
    draw_buttons(cursor, paint_buttons, num_paint_buttons);
    change_tool(pencil, Brush);
    press_button(paint_buttons[0]); // Brush
}

void color_picker() {    
    color_picker_layer = create_layer(45, 440, 538, 323);
    layer_draw_image(color_picker_layer, colorpicker_bmp, 45, 440);
    while(gameState == PickingColor) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isRtcEvent) {
            wordgame_tick_clock();
            uart_send_tick_clock();
        }
        if(event.isMouseEvent && (event.mouseEvent.type == LB_PRESS || event.mouseEvent.type == RB_PRESS)) {
            if (is_within_bounds(color_picker_layer, cursor->x, cursor->y)) {
                if (event.mouseEvent.type == LB_PRESS)
                    pencil->color1 = layer_get_pixel(color_picker_layer, cursor->x, cursor->y);
                else pencil->color2 = layer_get_pixel(color_picker_layer, cursor->x, cursor->y);  
            }
            changeState(lastGameState);
        }
        if (event.isUARTEvent) {
            if (get_msg_by_id(MSG_GAME_WON, event.uart_messages, event.num_uart_messages))
                changeState(EndRoundWin);
            else if (get_msg_by_id(MSG_GAME_LOST, event.uart_messages, event.num_uart_messages))
                changeState(EndRoundLoss);
        }
    }
    change_tool(pencil, pencil->tool); // atualiza os paint tubes
    destroy_layer(color_picker_layer);
    unpress_button(paint_buttons[6]);
}

void draw() {
    while (gameState == Drawing) {
        event = GetEvent();
        switch (checkButtonPress(event, cursor, paint_buttons, num_paint_buttons)) {
            case 0: change_tool(pencil, Brush); break;
            case 1: change_tool(pencil, Bucket);break;
            case 2: change_tool(pencil, ColorPicker); break;
            case 3: change_tool(pencil, Rainbow); break;
            case 4: change_tool(pencil, Rubber); break;
            case 5: canvas_set_color(WHITE); uart_send_trash(); break;
            case 6: changeState(PickingColor); break;
            case -1: 
                if (!wordgame_time_up()) 
                    usePencil(pencil, event); 
                break; // Se um botao for carregado o pincel nao e usado (rimou) xd
        }
        if (update_slider(event, cursor, thickness_slider)) {
            uint8_t new_thickness = round(slider_get_perc(thickness_slider) * 10) + 1;
            changeThickness(pencil, new_thickness);
        }
        update_cursor(cursor, event);
        if (event.isRtcEvent) {
            wordgame_tick_clock();
            uart_send_tick_clock();
        }
        if (event.isUARTEvent) {
            if (get_msg_by_id(MSG_GAME_WON, event.uart_messages, event.num_uart_messages))
                changeState(EndRoundWin);
            else if (get_msg_by_id(MSG_GAME_LOST, event.uart_messages, event.num_uart_messages))
                changeState(EndRoundLoss);
        }
    }
}


void usePencil(DrawingState* pencil, Event_t event) {
    if (!event.isMouseEvent)
        return;
    int xf = cursor_get_xf(cursor, event.mouseEvent.delta_x);
    int yf = cursor_get_yf(cursor, -event.mouseEvent.delta_y);
    uint32_t color, colorRubber;
    if (event.isLBPressed) {
        color = pencil->color1;
        colorRubber = pencil->color2;
    }
    else if (event.isRBPressed) {
        color = pencil->color2;
        colorRubber = pencil->color1;
    }
    else return;
    switch (pencil->tool) {
        case Brush:
            canvas_draw_line(cursor->x, cursor->y, xf, yf, color, pencil->thickness);
            if (gameState != Training)
                uart_send_draw_line(cursor->x, cursor->y, xf, yf, color, pencil->thickness);
            break;
        case Rainbow:
            rainbow_draw(pencil, cursor->x, cursor->y, xf, yf);
            sprite_set_color(cursor, pencil->rainbowColor);
            if (gameState != Training)
                uart_send_draw_line(cursor->x, cursor->y, xf, yf, pencil->rainbowColor, pencil->thickness);
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
            if (event.mouseEvent.type != LB_PRESS && event.mouseEvent.type != RB_PRESS)
                break;       
            bucket_tool(cursor->x, cursor->y, color);
            if (gameState != Training)
                uart_send_bucket(cursor->x, cursor->y, color);
            break;
        case Rubber:
            canvas_draw_line2(cursor->x, cursor->y, xf, yf, colorRubber, pencil->thickness + 2);
            if (gameState != Training)
                uart_send_draw_line2(cursor->x, cursor->y, xf, yf, colorRubber, pencil->thickness + 2);
            break;
        default: break;
    }
}

void rainbow_draw(DrawingState* pencil, uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf) {
    pencil->rainbowColor = rainbow(pencil->rainbowColor);
    idle_sprite_change_color(paint_tube_1, pencil->rainbowColor);
    idle_sprite_change_color(paint_tube_2, pencil->rainbowColor);
    canvas_draw_line(x0, y0, xf, yf, pencil->rainbowColor, pencil->thickness);
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
        idle_sprite_change_color(paint_tube_1, pencil->rainbowColor);
        idle_sprite_change_color(paint_tube_2, pencil->rainbowColor);  
        sprite_set_color(cursor, pencil->rainbowColor);
    }
    else if (tool != None) {
        idle_sprite_change_color(paint_tube_1, pencil->color1);
        idle_sprite_change_color(paint_tube_2, pencil->color2);
        sprite_set_color(cursor, pencil->color1);
    }
    else sprite_set_color(cursor, WHITE);
    draw_sprite_color(cursor);
    pencil->tool = tool;
}

void changeThickness(DrawingState* pencil, uint8_t thickness) {
    if (thickness > 10)
        pencil->thickness = 10;
    else if (thickness == 0)
        pencil->thickness = 1;
    else pencil->thickness = thickness;
}

//////////////
// Training //
//////////////

void setup_training() {
    reloadBackground();
    create_canvas(background, 150, 75, 1000, 750, WHITE);
    draw_idle_sprite(paint_tube_1);
    draw_idle_sprite(paint_tube_2);
    draw_slider(thickness_slider);
    disable_buttons(paint_buttons, num_paint_buttons);
    disable_buttons(training_buttons, num_training_buttons);
    draw_buttons(cursor, paint_buttons, num_paint_buttons);
    draw_buttons(cursor, training_buttons, num_training_buttons);
    draw_buttons(cursor, wordgame_buttons, num_wordgame_buttons);
    change_tool(pencil, Brush);
    press_button(paint_buttons[0]); // Brush
}

void training() {
    while (gameState == Training) {
        event = GetEvent();
        switch (checkButtonPress(event, cursor, wordgame_buttons, num_wordgame_buttons)) {
            case 0: toggle_emote_wheel(cursor); break;
            case -1: break; 
        }
        switch (checkButtonPress(event, cursor, training_buttons, num_training_buttons)) {
            case 0: changeState(Saving); return;
            case 1: changeState(Loading); return;
            case -1: break; 
        }
        switch (checkButtonPress(event, cursor, paint_buttons, num_paint_buttons)) {
            case 0: change_tool(pencil, Brush); break;
            case 1: change_tool(pencil, Bucket); break;
            case 2: change_tool(pencil, ColorPicker); break;
            case 3: change_tool(pencil, Rainbow); break;
            case 4: change_tool(pencil, Rubber); break;
            case 5: canvas_set_color(WHITE); break;
            case 6: changeState(PickingColor); break;
            case -1: usePencil(pencil, event); break; // Se um botao for carregado o pincel nao e usado (rimou) xd
        }
        if (update_slider(event, cursor, thickness_slider)) {
            uint8_t new_thickness = round(slider_get_perc(thickness_slider) * 10) + 1;
            changeThickness(pencil, new_thickness);
        }
        update_cursor(cursor, event);
        if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS) {
            destroy_emote_wheel();
            changeState(MainMenu);
        }
    }
}

void save() {
    textbox_layer = create_layer(400, 300, 334, 135);
    TextBox* textbox = create_textbox(textbox_layer, textbox_bmp, 10, 10, 1, LeftAlign);
    textbox_write(textbox, "Insert file name...");
    while(gameState == Saving) {
        event = GetEvent();
        update_cursor(cursor, event);
        useTextbox(textbox, event, Training, Training);
    }
    if (strcmp(textbox->text, "") != 0) {
        char path[500];
        strcpy(path, saved_img_folder);
        strcat(path, textbox->text);
        strcat(path, ".bmp");
        char* canvas_map = canvas_get_map();
        saveBitmap(path, canvas_get_width(), canvas_get_height(), canvas_map);    
        free(canvas_map);    
    }
    destroy_textbox(textbox);
    unpress_button(training_buttons[0]);

}

void load() {
    textbox_layer = create_layer(400, 300, 334, 135);
    TextBox* textbox = create_textbox(textbox_layer, textbox_bmp, 10, 10, 1, LeftAlign);
    textbox_write(textbox, "Insert file name...");
    while(gameState == Loading) {
        event = GetEvent();
        update_cursor(cursor, event);
        useTextbox(textbox, event, Training, Training);
    }
    Bitmap* bmp = NULL;
    if (strcmp(textbox->text, "") != 0) {
        char file_name[500] = "";
        strcat(file_name, textbox->text);
        strcat(file_name, ".bmp");
        bmp = loadBitmap(saved_img_folder, file_name);
    }
    destroy_textbox(textbox);
    unpress_button(training_buttons[0]);
    if (bmp == NULL)
        return;
    canvas_draw_image(bmp);
    free(bmp);
}

// Minigames

// FLAPPY //

void play_flappy() {
    start_flappy(background);
    while (gameState == PlayingFlappy) {
        event = GetEvent();
        if (event.isTimerEvent)
            flappy_tick();
        if (event.isKeyboardEvent) {
            if (event.keyboardEvent.type == CHARACTER_PRESS && event.keyboardEvent.character == ' ')
                flappy_jump();
            else if (event.keyboardEvent.type == SPACEBAR_RELEASE)
                flappy_endjump();
        }    
        if (flappy_dead())
            changeState(MainMenu);
    }
    timer_reset_counter();
    while(1) {
        event = GetEvent();
        if (event.isTimerEvent)
            flappy_tick();
        if (event.isTimerEvent && event.timerEvent.has_second_passed && event.timerEvent.seconds_passed == 3)
            flappy_showstats();
        if (event.isKeyboardEvent && event.keyboardEvent.type == ENTER_PRESS)
            break;
    }
    end_flappy();
}

// SNAKE //

void play_snake() {
    bool gameOver = false;
    reloadBackground();
    create_snake_arena(background);
    create_snake(1, 1000);
    while (gameState == PlayingSnake) {
        event = GetEvent();
        if (event.isTimerEvent && !gameOver) {
            snake_game_tick();
        }
        if (event.isKeyboardEvent) {
            if (event.keyboardEvent.type == ARROW_LEFT_PRESS)
                snake_set_turning(Left, true);
            else if (event.keyboardEvent.type == ARROW_RIGHT_PRESS)
                snake_set_turning(Right, true);
            else if (event.keyboardEvent.type == ARROW_LEFT_RELEASE)
                snake_set_turning(Left, false);
            else if (event.keyboardEvent.type == ARROW_RIGHT_RELEASE)
                snake_set_turning(Right, false);
        }
        if (event.isKeyboardEvent && event.keyboardEvent.type == ENTER_PRESS)
            changeState(MainMenu);
        if (snake_isGameOver() && !gameOver) {
            gameOver = true;
            snake_showstats();
        }
    }
    destroy_snake_arena();
}
