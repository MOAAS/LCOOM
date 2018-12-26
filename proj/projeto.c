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
extern Bitmap* save_button;
extern Bitmap* save_hl;
extern Bitmap* load_button;
extern Bitmap* load_hl;

extern Bitmap* textbox_bmp;
extern Bitmap* wide_textbox_bmp;
extern Bitmap* ultrawide_textbox_bmp;
extern Bitmap* colorpicker_bmp;
extern Bitmap* clock_bmp;
extern Bitmap* paint_tube_1_bmp;
extern Bitmap* paint_tube_2_bmp;

extern Bitmap* cool_dude_happy;
extern Bitmap* cool_dude_angry;
extern Bitmap* cool_dude_happy_big;
extern Bitmap* cool_dude_angry_big;

static NNY* paint_tube_1;
static NNY* paint_tube_2;

static int num_menu_buttons = 5;
static int num_paint_buttons = 7;
static int num_training_buttons = 2;

static Button* paint_buttons[7];
static Button* menu_buttons[10];
static Button* training_buttons[5];

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


static Event_t event;

char* words_path = "home/lcom/labs/proj/palavras/palavras.txt";
char* snake_hs_path = "home/lcom/labs/proj/highscores/snake.txt";
char* flappy_hs_path = "home/lcom/labs/proj/highscores/flappy.txt";

void loadCursor() {
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
    paint_buttons[6] = create_button(45, 400, background, palette_button, palette_hl); //color picker
    paint_buttons[5]->singleState = true; // trash
    paint_buttons[6]->singleState = true; // color picker

    menu_buttons[0] = create_button(200, 266, background, mainmenu_button1, mainmenu_button1_hl);
    menu_buttons[1] = create_button(305, 499, background, mainmenu_button2, mainmenu_button2_hl);
    menu_buttons[2] = create_button(540, 330, background, mainmenu_button3, mainmenu_button3_hl);
    menu_buttons[3] = create_button(40, 650, background, mainmenu_button4, mainmenu_button4_hl);
    menu_buttons[4] = create_button(703, 462, background, snake_button, snake_button_hl);
    menu_buttons[0]->singleState = true; 
    menu_buttons[1]->singleState = true; 
    menu_buttons[2]->singleState = true; 
    menu_buttons[3]->singleState = true; 
    menu_buttons[4]->singleState = true; 

    training_buttons[0] = create_button(10, 100, background, save_button, save_hl); // save
    training_buttons[1] = create_button(77, 100, background, load_button, load_hl); // save
    training_buttons[0]->singleState = true; // save
    training_buttons[1]->singleState = true; // save
}

void loadBackground() {
    background = create_layer(0, 0, vg_get_hres(), vg_get_vres());
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
    loadBitmaps();
    loadCursor();
    loadLetterMap();    
    loadDictionary(words_path);
    loadSnakeHighscore(snake_hs_path);
    loadFlappyHighscore(flappy_hs_path);
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
            case EndGameWin: end_game(); break;
            case EndGameLoss: end_game(); break;
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
}


// ULTRA TEST //
// ULTRA TEST //
// ULTRA TEST //

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



void play_snake() {
    bool gameOver = false;
    reloadBackground();
    create_snake_arena(background);
    create_snake(500, 500, 1, 1000);
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

// ULTRA TEST END //
// ULTRA TEST END //
// ULTRA TEST END //


void start_screen() {
    layer_draw_image(background, welcome_screen, 0, 0);
    draw_word(background, "WeLCOM user!", vg_get_hres() / 2, 600, 4, 0, CenterAlign);
    draw_word(background, "Press enter to start!", vg_get_hres() / 2, 670, 2, 0, CenterAlign);
    while (gameState == Start) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isKeyboardEvent && event.keyboardEvent.type == ENTER_PRESS)
            changeState(MainMenu); 
    }
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
            case -1: break;
        }
        update_cursor(cursor, event);
    }
}

void start_game(char* solution) {
    if (gameState == WaitDrawing) {
        changeState(Drawing);
        setupDrawing();
        word_pick_start(background, solution, true);
    }
    else if (gameState == WaitGuessing) {
        changeState(Guessing);
        setupGuessing();
        word_pick_start(background, solution, false);
    }
    else changeState(MainMenu);
}

void end_game() {
    word_pick_end();
    destroy_canvas();
    destroy_clock();   
    reset_pencil(pencil);
    layer_draw_image(background, background_menu , 0, 0);
    layer_draw_image(background, wide_textbox_bmp, 100, 200); // 860x100 image
    if (gameState == EndGameWin) {
        draw_word(background, "You win!", 530, 235, 2, 0, CenterAlign);
        layer_draw_image(background, cool_dude_happy_big, 500, 325);
    }
    else {
        draw_word(background, "You lose!", 530, 235, 2, 1, CenterAlign);
        layer_draw_image(background, cool_dude_angry_big, 500, 325);
    }
    // Revela a palavra
    char reveal[100];
    sprintf(reveal, "The word was: %s", get_solution());
    draw_word(background, reveal, 530, 275, 1, 0, CenterAlign);

    timer_reset_counter();
    while(1) {
        event = GetEvent();
        if (event.isTimerEvent && event.timerEvent.seconds_passed == 2)
            break;
        update_cursor(cursor, event);
    }
    if (is_wordgame_over()) {
        changeState(MainMenu);
        reset_wordgame();
    }
    else if (lastGameState == TypingGuess || lastGameState == Guessing)
        changeState(WaitDrawing);
    else if (lastGameState == Drawing || lastGameState == PickingColor)
        changeState(WaitGuessing);    
}


void wait_for_guesser() {
    layer_draw_image(background, wide_textbox_bmp, 85, 428); // 860x100 image
    draw_word(background, "Waiting for opponent...", 515, 480, 2, 0, CenterAlign);
    while (1) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isUARTEvent && get_msg_by_id(MSG_GUESSER_READY, event.uart_messages, event.num_uart_messages)) {
            free_messages(event.uart_messages, event.num_uart_messages);
            break;
        }
        if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS) {
            changeState(MainMenu); // Verifica DEPOIS de (nao) receber
            return;
        }
    }
    char* solution = get_random_word();
    uart_send_drawer_ready(solution);
    start_game(solution);
}

void wait_for_drawer() {
    UARTMessage* msg; char* solution = NULL;
    layer_draw_image(background, wide_textbox_bmp, 85, 428); // 860x100 image
    draw_word(background, "Waiting for opponent...", 515, 480, 2, 0, CenterAlign);
    while (1) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS) {
            changeState(MainMenu); // Verifica ANTES de mandar
            return;
        }
        if (event.isTimerEvent && event.timerEvent.timer_counter % 30 == 0)
            uart_send_guesser_ready(); // manda de meio em meio segundo
        if (event.isUARTEvent && (msg = get_msg_by_id(MSG_DRAWER_READY, event.uart_messages, event.num_uart_messages))) {
            solution = malloc(strlen((char*)msg->bytes + 1) * sizeof(char));
            strcpy(solution, (char*)msg->bytes);
            free_messages(event.uart_messages, event.num_uart_messages);
            break;
        }
    }
    start_game(solution);
    free(solution);
}


//////////////
// GUESSING //
//////////////

void setupGuessing() {
    reloadBackground();
    create_clock(clock_bmp);
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
        if (clock_time_up()) {
            changeState(EndGameLoss);
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
        if (clock_time_up()) {
            changeState(EndGameLoss);
            uart_send_game_loss();
            break;
        }
        useTextbox(textbox, event, Guessing, Guessing);
        update_cursor(cursor, event);
    }
    if (verify_guess(textbox->text) && !clock_time_up()) { // if guess is correct 
        changeState(EndGameWin);
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
    pencil->rainbowColor= RED;
    change_tool(pencil, None);
}

void setupDrawing() {
    reloadBackground();
    create_clock(clock_bmp);
    create_canvas(background, 150, 150, 1000, 750, WHITE);
    paint_tube_1 = create_NNY(45, 280, background, paint_tube_1_bmp);
    paint_tube_2 = create_NNY(79, 283, background, paint_tube_2_bmp);
    nny_change_color(paint_tube_1, pencil->color1);
    nny_change_color(paint_tube_2, pencil->color2);
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
        if (event.isTimerEvent && event.timerEvent.has_second_passed) {
            tick_clock();
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
                changeState(EndGameWin);
            else if (get_msg_by_id(MSG_GAME_LOST, event.uart_messages, event.num_uart_messages))
                changeState(EndGameLoss);
            free_messages(event.uart_messages, event.num_uart_messages);    
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
                if (!clock_time_up()) 
                    usePencil(pencil, event); 
                break; // Se um botao for carregado o pincel nao e usado (rimou) xd
        }
        update_cursor(cursor, event);
        if (event.isTimerEvent && event.timerEvent.has_second_passed) {
            tick_clock();
            uart_send_tick_clock();
        }
        if (event.isKeyboardEvent) {
            switch(event.keyboardEvent.type) {
                case RSHIFT_PRESS: increaseThickness(pencil); break;
                case LSHIFT_PRESS: decreaseThickness(pencil); break;
                default: break;
            }
        }
        if (event.isUARTEvent) {
            if (get_msg_by_id(MSG_GAME_WON, event.uart_messages, event.num_uart_messages))
                changeState(EndGameWin);
            else if (get_msg_by_id(MSG_GAME_LOST, event.uart_messages, event.num_uart_messages))
                changeState(EndGameLoss);
            free_messages(event.uart_messages, event.num_uart_messages);   
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
    nny_change_color(paint_tube_1, pencil->rainbowColor);
    nny_change_color(paint_tube_2, pencil->rainbowColor);
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
        nny_change_color(paint_tube_1, pencil->rainbowColor);
        nny_change_color(paint_tube_2, pencil->rainbowColor);  
        sprite_set_color(cursor, pencil->rainbowColor);
    }
    else if (tool != None) {
        nny_change_color(paint_tube_1, pencil->color1);
        nny_change_color(paint_tube_2, pencil->color2);
        sprite_set_color(cursor, pencil->color1);
    }
    else sprite_set_color(cursor, WHITE);
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

//////////////
// Training //
//////////////

void setup_training() {
    reloadBackground();
    create_canvas(background, 150, 75, 1000, 750, WHITE);
    paint_tube_1 = create_NNY(45, 320, background, paint_tube_1_bmp);
    paint_tube_2 = create_NNY(79, 323, background, paint_tube_2_bmp);
    nny_change_color(paint_tube_1, pencil->color1);
    nny_change_color(paint_tube_2, pencil->color2);
    disable_buttons(paint_buttons, num_paint_buttons);
    disable_buttons(training_buttons, num_training_buttons);
    draw_buttons(cursor, paint_buttons, num_paint_buttons);
    draw_buttons(cursor, training_buttons, num_training_buttons);
    change_tool(pencil, Brush);
    press_button(paint_buttons[0]); // Brush
}

void training() {
    while (gameState == Training) {
        event = GetEvent();
        switch (checkButtonPress(event, cursor, training_buttons, num_training_buttons)) {
            case 0: changeState(Saving); return;
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
        update_cursor(cursor, event);
        if (event.isKeyboardEvent) {
            switch(event.keyboardEvent.type) {
                case ESC_PRESS: changeState(MainMenu); return;
                case RSHIFT_PRESS: increaseThickness(pencil); break;
                case LSHIFT_PRESS: decreaseThickness(pencil); break;
                default: break;
            }
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
        char path[500] = "home/lcom/labs/proj/saved_images/";
        strcat(path, textbox->text);
        strcat(path, ".bmp");
        char* canvas_map = canvas_get_map();
        saveBitmap(path, canvas_get_width(), canvas_get_height(), canvas_map);    
        free(canvas_map);    
    }
    destroy_textbox(textbox);
    unpress_button(training_buttons[0]);

}
