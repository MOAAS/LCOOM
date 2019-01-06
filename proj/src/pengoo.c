#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "pengoo.h"

extern GameState gameState;
extern GameState lastGameState;

extern DrawingState* pencil;

/////////////
// Bitmaps //
/////////////

extern Bitmap* cursor_normal;
extern Bitmap* cursor_brush;
extern Bitmap* cursor_rainbow;
extern Bitmap* cursor_balde;
extern Bitmap* cursor_picker;
extern Bitmap* cursor_rubber;
extern Bitmap* cursor_shape;

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

extern Bitmap* info_button;
extern Bitmap* info_button_hl;

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

extern Bitmap* shape0_bmp;
extern Bitmap* shape1_bmp;
extern Bitmap* shape2_bmp;

extern Bitmap* shape0_hl_bmp;
extern Bitmap* shape1_hl_bmp;
extern Bitmap* shape2_hl_bmp;


extern Bitmap* emote_icon;
extern Bitmap* emote_icon_hl;

extern Bitmap* paint_tube_1_bmp;
extern Bitmap* paint_tube_2_bmp;

extern Bitmap* save_button;
extern Bitmap* save_hl;
extern Bitmap* load_button;
extern Bitmap* load_hl;
extern Bitmap* undo_button;
extern Bitmap* undo_hl;

extern Bitmap* textbox1_bmp;
extern Bitmap* textbox2_bmp;
extern Bitmap* textbox5_bmp;
extern Bitmap* textbox6_bmp;
extern Bitmap* colorpicker_bmp;

static int num_menu_buttons = 10;
static int num_paint_buttons = 10;
static int num_training_buttons = 3;
static int num_wordgame_buttons = 1;
static int num_collab_buttons = 1;

static Button* paint_buttons[10];
static Button* menu_buttons[10];
static Button* training_buttons[5];
static Button* wordgame_buttons[3];
static Button* collab_buttons[3];

static IdleSprite* paint_tube_1;
static IdleSprite* paint_tube_2;

static Slider* thickness_slider;

/////////////////
// Buttons end //
/////////////////


static Layer* background = NULL;
static Layer* textbox_layer = NULL;
static Layer* color_picker_layer = NULL;

static Sprite* cursor;

static Event_t event;

void loadCursor() {
    cursor = create_sprite(cursor_normal, 0, 0);
    create_pencil(cursor);
}

void loadButtons() {
    paint_buttons[0] = create_button(5, 470, background, brush_button, brush_hl); // normal
    paint_buttons[1] = create_button(5, 550, background, balde_button, balde_hl); // bucket
    paint_buttons[2] = create_button(5, 630, background, picker_button, picker_hl); // color coiso
    paint_buttons[3] = create_button(75, 470, background, rainbow_button, rainbow_hl); // rainbow
    paint_buttons[4] = create_button(75, 550, background, rubber_button, rubber_hl); // borracha
    paint_buttons[5] = create_button(75, 630, background, trash_button, trash_hl); // trash
    paint_buttons[6] = create_button(45, 355, background, palette_button, palette_hl); //color picker

    paint_buttons[7] = create_button(15, 425, background, shape0_bmp, shape0_hl_bmp); // retangul
    paint_buttons[8] = create_button(60, 425, background, shape1_bmp, shape1_hl_bmp); // circulo
    paint_buttons[9] = create_button(105, 425, background, shape2_bmp, shape2_hl_bmp); // circumferencia

    paint_buttons[5]->singleState = true; // trash
    paint_buttons[6]->singleState = true; // color picker

    menu_buttons[0] = create_button(200, 266, background, mainmenu_button1, mainmenu_button1_hl);
    menu_buttons[1] = create_button(305, 499, background, mainmenu_button2, mainmenu_button2_hl);
    menu_buttons[2] = create_button(540, 330, background, mainmenu_button3, mainmenu_button3_hl);
    menu_buttons[3] = create_button(40, 650, background, mainmenu_button4, mainmenu_button4_hl);
    menu_buttons[4] = create_button(703, 462, background, snake_button, snake_button_hl);
    menu_buttons[5] = create_button(758, 405, background, bird_button, bird_button_hl);

    // info_buttons
    menu_buttons[6] = create_button(190, 250, background, info_button, info_button_hl); // wordgame info
    menu_buttons[7] = create_button(535, 335, background, info_button, info_button_hl); // singleplayer info
    menu_buttons[8] = create_button(705, 580, background, info_button, info_button_hl); // snake info
    menu_buttons[9] = create_button(750, 385, background, info_button, info_button_hl); // flappy info

    menu_buttons[0]->singleState = true; 
    menu_buttons[1]->singleState = true; 
    menu_buttons[2]->singleState = true; 
    menu_buttons[3]->singleState = true; 
    menu_buttons[4]->singleState = true; 
    menu_buttons[5]->singleState = true; 
    menu_buttons[6]->singleState = true; 
    menu_buttons[7]->singleState = true; 
    menu_buttons[8]->singleState = true; 
    menu_buttons[9]->singleState = true; 

    training_buttons[0] = create_button(150, 5, background, undo_button, undo_hl);// undo
    training_buttons[1] = create_button(225, 5, background, save_button, save_hl);// save
    training_buttons[2] = create_button(300, 5, background, load_button, load_hl);// load
    training_buttons[0]->singleState = true; // undo
    training_buttons[1]->singleState = true; // save
    training_buttons[2]->singleState = true; // load

    wordgame_buttons[0] = create_button(10, 10, background, emote_icon, emote_icon_hl);

    collab_buttons[0] = create_button(150, 5, background, save_button, save_hl);

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
            case 0: changeState(StartWordGame); break;
            case 1: changeState(WaitCollab); break;
            case 2: changeState(Training); setup_canvas(); break;
            case 3: changeState(ExitGame); break;
            case 4: changeState(PlayingSnake); break;
            case 5: changeState(PlayingFlappy); break;
            case 6: toggle_instructions(WordGameInfo); break;
            case 7: toggle_instructions(DrawToolInfo); break;
            case 8: toggle_instructions(SnakeInfo); break;
            case 9: toggle_instructions(FlappyInfo); break;
            case -1: break;
        }
        if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS)
            clear_instructions();
        update_cursor(cursor, event);
    }
    clear_instructions();
}

void start_round(char* solution) {
    if (gameState == Drawing) {
        setup_canvas();
        wordgame_start_round(background, solution, true);
    }
    else if (gameState == Guessing) {
        setup_canvas();
        wordgame_start_round(background, solution, false);
    }
    else changeState(MainMenu);
}

void end_round() {
    int points = 100 + wordgame_get_time_left() * 5 - 100 * (gameState == EndRoundLoss);
    destroy_canvas();
    destroy_emote();
    destroy_emote_wheel();
    reset_pencil(pencil);
    reloadBackground();
    layer_draw_image(background, textbox6_bmp, 60, 250); // 860x100 image
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
    }
    else if (lastGameState == TypingGuess || lastGameState == Guessing) {
        changeState(WaitDrawing);
        draw_word(background, "You failed!", 500, 300, 2, 0, CenterAlign);
    }
    else if (lastGameState == Drawing || lastGameState == PickingColor) {
        changeState(WaitGuessing);
        draw_word(background, "Your teammate failed!", 500, 300, 2, 0, CenterAlign);
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
    timer_reset_counter();
    while(gameState == EndGame) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isKeyboardEvent && event.keyboardEvent.type == ENTER_PRESS)
            changeState(MainMenu); 
        if (event.isTimerEvent && event.timerEvent.seconds_passed == 10)
            changeState(MainMenu); 
    }
    reset_wordgame();
}

void select_drawer() {
    UARTMessage* msg; unsigned choice = rand();
    waiting_for_player();
    while(gameState == StartWordGame) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isTimerEvent && event.timerEvent.timer_counter % 20 == 0)
            uart_send_number(choice);
        // recebeu um numero
        if (event.isUARTEvent && (msg = get_msg_by_id(MSG_NUMBER, event.uart_messages, event.num_uart_messages))) {
            UARTMessageContents msgCont = uart_process_msg(msg);
            unsigned opponent_choice = msgCont.number;
            if (opponent_choice > choice) // quem tem um numero maior desenha
                changeState(WaitGuessing);
            else if (opponent_choice < choice)
                changeState(WaitDrawing);
            else changeState(MainMenu);
        }
        else if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS) {
            changeState(MainMenu);
            return;
        }
    }
    uart_send_number(choice);
}

void wait_for_guesser() {
    char* solution = get_random_word();
    waiting_for_player();
    while (gameState == WaitDrawing) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isUARTEvent && get_msg_by_id(MSG_GUESSER_READY, event.uart_messages, event.num_uart_messages))
            changeState(Drawing);
    }
    uart_send_drawer_ready(solution);
    start_round(solution);
}

void wait_for_drawer() {
    UARTMessage* msg; char* solution = NULL;
    waiting_for_player();
    while (gameState == WaitGuessing) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isTimerEvent && event.timerEvent.timer_counter % 20 == 0)
            uart_send_empty_msg(MSG_GUESSER_READY);
        if (event.isUARTEvent && (msg = get_msg_by_id(MSG_DRAWER_READY, event.uart_messages, event.num_uart_messages))) {
            solution = malloc(strlen((char*)msg->bytes + 1) * sizeof(char));
            strcpy(solution, (char*)msg->bytes);
            changeState(Guessing);
        }
    }
    start_round(solution);
    free(solution);
}


//////////////
// GUESSING //
//////////////

void guess() {
    while(gameState == Guessing) {
        event = GetEvent();
        // Checking emote wheel pressing
        if (checkButtonPress(event, cursor, wordgame_buttons, num_wordgame_buttons) == 0)
            toggle_emote_wheel(wordgame_buttons[0], cursor);
        int emote_pressed = check_emote_press(event, cursor);
        if (emote_pressed != -1) {
            uart_send_emote(emote_pressed);
            toggle_emote_wheel(wordgame_buttons[0], cursor);
        }
        // Checking if enough time has passed to reveal the next letter
        if (event.isTimerEvent && event.timerEvent.has_second_passed && event.timerEvent.seconds_passed % 15 == 0)
            reveal_letter();
        // Processes received messages (emotes, drawing, etc)
        if (event.isUARTEvent)
            uart_process_msgs(event.uart_messages, event.num_uart_messages);
        // Checks if time is up, notifies the opponent and ends the round if so
        if (wordgame_time_up()) {
            changeState(EndRoundLoss);
            uart_send_empty_msg(MSG_ROUND_LOST);
            break;
        }
        if (event.isKeyboardEvent && event.keyboardEvent.type == ENTER_PRESS)
            changeState(TypingGuess);
        update_cursor(cursor, event);
    }
}

void type_guess() {    
    textbox_layer = create_layer(690, 660, 330, 100);
    TextBox* textbox = create_textbox(textbox_layer, textbox1_bmp, 10, 10, 1, LeftAlign);
    textbox_write(textbox, "Insert guess...");
    while(gameState == TypingGuess) {
        event = GetEvent();
        // Checking emote wheel pressing
        if (checkButtonPress(event, cursor, wordgame_buttons, num_wordgame_buttons) == 0)
            toggle_emote_wheel(wordgame_buttons[0], cursor);
        int emote_pressed = check_emote_press(event, cursor);
        if (emote_pressed != -1) {
            uart_send_emote(emote_pressed);
            toggle_emote_wheel(wordgame_buttons[0], cursor);
        }
        // Checking if enough time has passed to reveal the next letter
        if (event.isTimerEvent && event.timerEvent.has_second_passed && event.timerEvent.seconds_passed % 15 == 0)
            reveal_letter();
        // Processes received messages (emotes, drawing, etc)
        if (event.isUARTEvent)
            uart_process_msgs(event.uart_messages, event.num_uart_messages);
        // Checks if time is up, notifies the opponent and ends the round if so
        if (wordgame_time_up()) {
            changeState(EndRoundLoss);
            uart_send_empty_msg(MSG_ROUND_LOST);
            break;
        }
        // Updates the cursor and the textbox if needed (keyboard key pressed)
        useTextbox(textbox, event, Guessing, Guessing);
        update_cursor(cursor, event);
    }
    // Checks if guess is correct and notifies the opponent and ends the round if so
    if (verify_guess(textbox->text) && !wordgame_time_up()) {
        changeState(EndRoundWin);
        uart_send_tick_clock(wordgame_get_time_left());
        //util_delay(1);
        uart_send_empty_msg(MSG_ROUND_WON);
        //util_delay(1);
        //uart_send_empty_msg(MSG_ROUND_WON);
    }
    destroy_textbox(textbox);
}

/////////////
// DRAWING //
/////////////

void reset_pencil() {
    pencil->color1 = BLACK;
    pencil->color2 = WHITE;
    pencil->thickness = 4;
    pencil->midShape = false;
    thickness_slider->handle_pos = thickness_slider->x + + 0.4 * thickness_slider->width;
    pencil->rainbowColor= RED;
    change_tool(NoTool);
}

void draw() {
    while (gameState == Drawing) {
        event = GetEvent();
        update_emote_wheel(event);
        update_pencil(event);
        // Updates clock if needed
        if (event.isRtcEvent) {
            wordgame_tick_clock();
            uart_send_tick_clock(wordgame_get_time_left());
        }
        // Checking if the game is over, and processing other messages (emotes)
        if (event.isUARTEvent) {
            uart_process_msgs(event.uart_messages, event.num_uart_messages);
            if (get_msg_by_id(MSG_ROUND_WON, event.uart_messages, event.num_uart_messages))
                changeState(EndRoundWin);
            else if (get_msg_by_id(MSG_ROUND_LOST, event.uart_messages, event.num_uart_messages))
                changeState(EndRoundLoss);
        }
    }
}

void update_pencil(Event_t event) {
    if (gameState == Drawing && wordgame_time_up()) {
        update_cursor(cursor, event);
        return;
    }
    switch (checkButtonPress(event, cursor, paint_buttons, num_paint_buttons)) {
        case 0: change_tool(Brush); break;
        case 1: change_tool(Bucket);break;
        case 2: change_tool(ColorPicker); break;
        case 3: change_tool(Rainbow); break;
        case 4: change_tool(Rubber); break;
        case 5: 
            if (gameState != Training)
                uart_send_empty_msg(MSG_TRASH);
            else canvas_save_drawing();
            canvas_set_color(WHITE); 
            break;
        case 6: changeState(PickingColor); break;
        case 7: change_tool(ShapeDraw); pencil->shape = Rectangle; break;
        case 8: change_tool(ShapeDraw); pencil->shape = Circle; break;
        case 9: change_tool(ShapeDraw); pencil->shape = Circumference; break;
        case -1: 
            if (!is_emote_wheel_on())
                usePencil(event); 
            break;
    }
    // Checking if the thickness slider was updated
    if (update_slider(event, cursor, thickness_slider)) {
        uint8_t new_thickness = round(slider_get_perc(thickness_slider) * 10) + 1;
        changeThickness(new_thickness);
    }
    // Updates cursor if needed
    update_cursor(cursor, event);
} 

void usePencil(Event_t event) {
    if (!event.isMouseEvent)
        return;
    int xf = cursor_get_xf(pencil->cursor, event.mouseEvent.delta_x);
    int yf = cursor_get_yf(pencil->cursor, -event.mouseEvent.delta_y);
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
            if (gameState != Training)
                uart_send_draw_line(cursor->x, cursor->y, xf, yf, color, pencil->thickness);
            canvas_draw_line(cursor->x, cursor->y, xf, yf, color, pencil->thickness);
            break;
        case Rainbow:
            if (gameState != Training)
                uart_send_draw_line(cursor->x, cursor->y, xf, yf, pencil->rainbowColor, pencil->thickness);
            rainbow_draw(cursor->x, cursor->y, xf, yf);
            sprite_set_color(cursor, pencil->rainbowColor);
            break;
        case ColorPicker:
            if (event.mouseEvent.type == LB_PRESS || event.mouseEvent.type == RB_PRESS) {
                if (event.mouseEvent.type == LB_PRESS)
                    pencil->color1 = layer_get_pixel(background, cursor->x, cursor->y);
                else pencil->color2 = layer_get_pixel(background, cursor->x, cursor->y);
                pencil->tool = Brush;
                change_tool(Brush);
                press_button(paint_buttons[0]); // Brush
                unpress_button(paint_buttons[2]);
            }
            break;
        case Bucket:
            if (event.mouseEvent.type != LB_PRESS && event.mouseEvent.type != RB_PRESS)
                break;       
            if (gameState != Training)
                uart_send_bucket(cursor->x, cursor->y, color);
            bucket_tool(cursor->x, cursor->y, color);
            break;
        case Rubber:
            if (gameState != Training)
                uart_send_draw_line2(cursor->x, cursor->y, xf, yf, colorRubber, pencil->thickness + 2);
            canvas_draw_line2(cursor->x, cursor->y, xf, yf, colorRubber, pencil->thickness + 2);
            break;
        case ShapeDraw:
            if (event.mouseEvent.type != LB_PRESS && event.mouseEvent.type != RB_PRESS)
                break; 
            if (pencil->midShape) {
                if (is_inside_canvas(cursor->x, cursor->y)) {
                    if (gameState != Training)
                        uart_send_draw_shape(pencil->shape, pencil->x_shapeClick1, pencil->y_shapeClick1, cursor->x, cursor->y, color, pencil->thickness);
                    canvas_draw_shape(pencil->shape, pencil->x_shapeClick1, pencil->y_shapeClick1, cursor->x, cursor->y, color, pencil->thickness);
                    pencil->midShape = false;
                }
            }
            else if (is_inside_canvas(cursor->x, cursor->y)) {
                pencil->midShape = true;
                pencil->x_shapeClick1 = cursor->x;
                pencil->y_shapeClick1 = cursor->y;  
            }
            break;
        default: break;
    }
}

void rainbow_draw(uint16_t x0, uint16_t y0, uint16_t xf, uint16_t yf) {
    pencil->rainbowColor = rainbow(pencil->rainbowColor);
    idle_sprite_change_color(paint_tube_1, pencil->rainbowColor);
    idle_sprite_change_color(paint_tube_2, pencil->rainbowColor);
    canvas_draw_line(x0, y0, xf, yf, pencil->rainbowColor, pencil->thickness);
}

void color_picker() {    
    color_picker_layer = create_layer(45, 440, 538, 323);
    layer_draw_image(color_picker_layer, colorpicker_bmp, 45, 440);
    while(gameState == PickingColor) {
        event = GetEvent();
        update_emote_wheel(event);
        update_cursor(cursor, event);
        // Checking if a color was selected
        if(event.isMouseEvent && (event.mouseEvent.type == LB_PRESS || event.mouseEvent.type == RB_PRESS)) {
            if (is_within_bounds(color_picker_layer, cursor->x, cursor->y)) {
                if (event.mouseEvent.type == LB_PRESS)
                    pencil->color1 = layer_get_pixel(color_picker_layer, cursor->x, cursor->y);
                else pencil->color2 = layer_get_pixel(color_picker_layer, cursor->x, cursor->y);  
            }
            changeState(lastGameState);
        }
        if (lastGameState == Training)
            continue;
        // Checking if the game is over, and processing other messages (emotes)
        if (event.isUARTEvent) {
            uart_process_msgs(event.uart_messages, event.num_uart_messages);
            if (get_msg_by_id(MSG_ROUND_WON, event.uart_messages, event.num_uart_messages))
                changeState(EndRoundWin);
            else if (get_msg_by_id(MSG_ROUND_LOST, event.uart_messages, event.num_uart_messages))
                changeState(EndRoundLoss);
        }
        if (lastGameState == CollabDrawing)
            continue;
        // Updates clock if needed
        if (event.isRtcEvent) {
            wordgame_tick_clock();
            uart_send_tick_clock(wordgame_get_time_left());
        }
    }
    change_tool(pencil->tool); // atualiza os paint tubes
    destroy_layer(color_picker_layer);
    unpress_button(paint_buttons[6]);
}

void change_tool(Tool tool) {
    switch (tool) {
        case Brush: cursor_change_bmp(cursor,cursor_brush);  break;
        case Bucket: cursor_change_bmp(cursor,cursor_balde);break;
        case ColorPicker: cursor_change_bmp(cursor,cursor_picker); break;
        case Rainbow: cursor_change_bmp(cursor, cursor_rainbow); break;
        case Rubber: cursor_change_bmp(cursor, cursor_rubber); break;
        case ShapeDraw: cursor_change_bmp(cursor, cursor_shape); break;
        case NoTool: cursor_change_bmp(cursor, cursor_normal); break;
    } 
    if (tool == Rainbow) {
        idle_sprite_change_color(paint_tube_1, pencil->rainbowColor);
        idle_sprite_change_color(paint_tube_2, pencil->rainbowColor);  
        sprite_set_color(cursor, pencil->rainbowColor);
    }
    else if (tool != NoTool) {
        idle_sprite_change_color(paint_tube_1, pencil->color1);
        idle_sprite_change_color(paint_tube_2, pencil->color2);
        sprite_set_color(cursor, pencil->color1);
    }
    else sprite_set_color(cursor, WHITE);
    draw_sprite_color(cursor);
    pencil->tool = tool;
    pencil->midShape = false;
}

void changeThickness(uint8_t thickness) {
    if (thickness > 10)
        pencil->thickness = 10;
    else if (thickness == 0)
        pencil->thickness = 1;
    else pencil->thickness = thickness;
}

//////////////
// Training //
//////////////

void training() {
    while (gameState == Training) {
        event = GetEvent();
        switch (checkButtonPress(event, cursor, training_buttons, num_training_buttons)) {
            case 0: canvas_undo(); break;
            case 1: changeState(Saving); return;
            case 2: changeState(Loading); return;
            case -1: break; 
        }
        if (event.isMouseEvent && (pencil->tool == Brush || pencil->tool == Rainbow || pencil->tool == Rubber || pencil-> tool == Bucket || pencil->tool == ShapeDraw)) {
            if (event.mouseEvent.type == LB_PRESS || event.mouseEvent.type == RB_PRESS)
                canvas_start_drawing(cursor->x, cursor->y);
            else if (event.mouseEvent.type == LB_RELEASE || event.mouseEvent.type == RB_RELEASE)
                canvas_stop_drawing();
        }   
        update_pencil(event);
        if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS)            
            changeState(MainMenu);
        if (event.isKeyboardEvent && event.isCtrlPressed && event.keyboardEvent.scancode == Z_MAKE)
            canvas_undo();

    }
}

////////////////////////////
// Drawing Saving/Loading //
////////////////////////////

char* create_bmp_path(char* file_name, char* saved_img_folder) {
    for (int i = 0; file_name[i] != '\0'; i++) {
        switch (file_name[i]) {
            case '\\':
            case '\"':
            case '/':
            case ':':
            case '*':
            case '?':
            case '<':
            case '>':
            case '|': file_name[i] = ' '; break;
            default: break;
        }
    }
    char* path = malloc(strlen(saved_img_folder) + strlen(file_name) + strlen(".bmp") + 1);
    strcpy(path, saved_img_folder);
    strcat(path, file_name);
    strcat(path, ".bmp");
    return path;
}

void save(char* saved_img_folder) {
    textbox_layer = create_layer(175, 40, 330, 100);
    TextBox* textbox = create_textbox(textbox_layer, textbox1_bmp, 10, 10, 1, LeftAlign);
    textbox_write(textbox, "Insert file name...");
    while(gameState == Saving) {
        event = GetEvent();
        update_cursor(cursor, event);
        useTextbox(textbox, event, lastGameState, lastGameState);
        // Only processes received messages in case it's playing with someone else.
        if ((gameState == CollabDrawing || lastGameState == CollabDrawing) && event.isUARTEvent)
            uart_process_msgs(event.uart_messages, event.num_uart_messages);
    }
    if (strcmp(textbox->text, "") != 0) {
        char* path = create_bmp_path(textbox->text, saved_img_folder);
        char* canvas_map = canvas_get_map();
        saveBitmap(path, canvas_get_width(), canvas_get_height(), canvas_map);    
        free(path);
        free(canvas_map);    
    }
    destroy_textbox(textbox);
    if (gameState == Training)
        unpress_button(training_buttons[1]); // Releases save button
    else if (gameState == CollabDrawing)
        unpress_button(collab_buttons[0]); // Releases Save button
    else changeState(MainMenu); // nao devia acontecer :O
}

void load(char* saved_img_folder) {
    textbox_layer = create_layer(175, 40, 330, 100);
    TextBox* textbox = create_textbox(textbox_layer, textbox1_bmp, 10, 10, 1, LeftAlign);
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
    unpress_button(training_buttons[2]); // Releases load button
    if (bmp) {
        canvas_draw_image(bmp);
        free(bmp);
    }
}

///////////////////
// Multi Drawing //
///////////////////

void wait_for_collab() {
    waiting_for_player();
    while (gameState == WaitCollab) {
        event = GetEvent();
        update_cursor(cursor, event);
        if (event.isTimerEvent && event.timerEvent.timer_counter % 20 == 0)
            uart_send_empty_msg(MSG_COLLAB_READY);
        if (event.isUARTEvent && get_msg_by_id(MSG_COLLAB_READY, event.uart_messages, event.num_uart_messages)) {
            changeState(CollabDrawing);
            setup_canvas();
        }
        else if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS) {
            changeState(MainMenu);
            return;
        }
    }
    uart_send_empty_msg(MSG_COLLAB_READY);
}

void collab_drawing() {
    while (gameState == CollabDrawing) {
        event = GetEvent();
        //printf("evt gten\n");
        if (checkButtonPress(event, cursor, collab_buttons, num_collab_buttons) == 0) {
            changeState(Saving);
            return;
        }
        update_pencil(event);
        if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS)            
            changeState(MainMenu);
        if (event.isUARTEvent)
            uart_process_msgs(event.uart_messages, event.num_uart_messages);
    }
}

/// Other functions

void waiting_for_player() {
    layer_draw_image(background, textbox2_bmp, 85, 650); // 860x100 image
    draw_word(background, "Waiting for player...", 515, 700, 2, 0, CenterAlign);
}

void setup_canvas() {
    reloadBackground();    
    if (gameState == Drawing || gameState == Guessing)
        create_canvas(background, 150, 150, 1000, 750, WHITE);
    else create_canvas(background, 150, 75, 1000, 750, WHITE);
    if (gameState != Guessing) {
        draw_idle_sprite(paint_tube_1);
        draw_idle_sprite(paint_tube_2);
        draw_slider(thickness_slider);
        disable_buttons(paint_buttons, num_paint_buttons);
        draw_buttons(cursor, paint_buttons, num_paint_buttons);
        change_tool(Brush);
        press_button(paint_buttons[0]); // Brush
    }
    if (gameState == CollabDrawing) {
        disable_buttons(collab_buttons, num_collab_buttons);
        draw_buttons(cursor, collab_buttons, num_collab_buttons);
    }
    else if (gameState == Training) {
        disable_buttons(training_buttons, num_training_buttons);
        draw_buttons(cursor, training_buttons, num_training_buttons);
    }
    else {
        disable_buttons(wordgame_buttons, num_wordgame_buttons);
        draw_buttons(cursor, wordgame_buttons, num_wordgame_buttons);
    }
}

void update_emote_wheel(Event_t event) {
    // Checks if the emote wheel button was pressed, toggles it if so
    if (checkButtonPress(event, cursor, wordgame_buttons, num_wordgame_buttons) == 0)
        toggle_emote_wheel(wordgame_buttons[0], cursor);
    // Checks if an emote was presed while the wheel is open, and sends an emote if so
    int emote_pressed = check_emote_press(event, cursor);
    if (emote_pressed != -1) {
        uart_send_emote(emote_pressed);
        toggle_emote_wheel(wordgame_buttons[0], cursor);
    }
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
            if (event.keyboardEvent.scancode == SPACE_MAKE)
                flappy_jump();
            else if (event.keyboardEvent.scancode == SPACE_BREAK)
                flappy_endjump();
        } 
        if (flappy_dead())
            break;
        if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS)
            changeState(MainMenu);
    }
    timer_reset_counter();
    while(gameState == PlayingFlappy) {
        event = GetEvent();
        if (event.isTimerEvent)
            flappy_tick();
        if (event.isTimerEvent && event.timerEvent.has_second_passed && event.timerEvent.seconds_passed == 3)
            flappy_showstats();
        if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS)
            changeState(MainMenu);
    }
    end_flappy();
}

// SNAKE //

void play_snake() {
    bool gameOver = false;
    reloadBackground();
    create_snake_arena(background);
    create_snake(1, 1250);
    while (gameState == PlayingSnake) {
        event = GetEvent();
        if (event.isTimerEvent && !gameOver) {
            snake_game_tick();
        }
        if (event.isKeyboardEvent) {
            if (event.keyboardEvent.type == ARROW_LEFT_PRESS)
                snake_set_turning(LeftDir, true);
            else if (event.keyboardEvent.type == ARROW_RIGHT_PRESS)
                snake_set_turning(RightDir, true);
            else if (event.keyboardEvent.type == ARROW_LEFT_RELEASE)
                snake_set_turning(LeftDir, false);
            else if (event.keyboardEvent.type == ARROW_RIGHT_RELEASE)
                snake_set_turning(RightDir, false);
        }
        if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS)
            changeState(MainMenu);
        if (snake_isGameOver() && !gameOver) {
            gameOver = true;
            snake_showstats();
        }
    }
    destroy_snake_arena();
}
