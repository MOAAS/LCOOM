// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "pengoo.h"


/** @defgroup proj proj
 * @{
 *
 * Pen-Goo hub state manager.
 */


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");
  //lcf_trace_calls("/home/lcom/labs/proj/trace.txt");
  lcf_log_output("/home/lcom/labs/proj/output.txt");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

extern GameState gameState;
extern GameState lastGameState;

static char* words_path =       "home/lcom/labs/proj/palavras/palavras.txt";
static char* snake_hs_path =    "home/lcom/labs/proj/highscores/snake.txt";
static char* flappy_hs_path =   "home/lcom/labs/proj/highscores/flappy.txt";
static char* wordgame_hs_path = "home/lcom/labs/proj/highscores/wordgame.txt";
static char* bitmap_folder =    "home/lcom/labs/proj/bitmaps/";
static char* saved_img_folder = "home/lcom/labs/proj/saved_images/";

/**
 * @brief Setups, starts and manages the entire game hub.
 * 
 */
int (proj_main_loop)() {
  /*
  subscribe_device(SerialPort);
  subscribe_device(Keyboard);
  subscribe_device(Mouse);
  subscribe_device(Timer);
  int rcv_cnt = 0, xmit_cnt = 0;
  sleep(1);
  while (1) {
    Event_t event = GetEvent();
    if (event.isTimerEvent) {
      xmit_cnt++;
      printf("transmit no. %d: Sending...\n", xmit_cnt);
      uart_send_drawer_ready("HI HOW ARE");
    }
    if (event.isKeyboardEvent && event.keyboardEvent.type == ESC_PRESS)
      break;
    if (event.isKeyboardEvent && event.keyboardEvent.type == ENTER_PRESS) {
      uint8_t status;
      uart_read_status(&status);
      while (status & UART_RECEIVER_DATA) {
        printf("status = %d \n", status);
        uart_read_data(&status);
        uart_read_status(&status);
      }
      printf("done (status = %d)", status);
    }
    if (event.isUARTEvent) {
     // printf("num uart_messages: %d\n", event.num_uart_messages);
      for (int j = 0; j < event.num_uart_messages; j++) {
        UARTMessageContents cont = uart_process_msg(&event.uart_messages[j]);
        if (cont.type == MSG_DRAWER_READY)
          rcv_cnt++; 
          printf("receive no. %d: %s\n", rcv_cnt, event.uart_messages[j].bytes);
        
      }
    }
  }
  unsubscribe_device(Mouse);
  unsubscribe_device(Keyboard);
  unsubscribe_device(SerialPort);
  return 0;
  */





  srand(time(NULL));
  loadBitmaps(bitmap_folder);
  loadCursor();
  loadLetterMap();    
  wordgame_set_rounds(4);
  loadDictionary(words_path);
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
  while(gameState != ExitGame) {
      switch(gameState) {
          case Start: start_screen(); break;
          case MainMenu: main_menu(); break;
          case StartWordGame: select_drawer(); break;
          case WaitDrawing: wait_for_guesser(); break;
          case WaitGuessing: wait_for_drawer(); break;
          case Drawing: draw(); break;
          case PickingColor: color_picker(); break;
          case Guessing: guess(); break;
          case TypingGuess: type_guess(); break;
          case Training: training(); break;
          case Saving: save(saved_img_folder); break;
          case Loading: load(saved_img_folder); break;
          case WaitCollab: wait_for_collab(); break;
          case CollabDrawing: collab_drawing(); break;
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
  return 0;


 
 /*
  subscribe_device(RTC);
  for(int i=0 ; i< 10 ;){
    Notification notification = GetNotification();
    if(notification.rtcAlarmNotif){
      Mini_Date d = rtc_get_mini_date();
      print_mini_date(d);
      i++;
    }
  }
 unsubscribe_device(RTC);
 */
}


