#include <lcom/lcf.h>

#include <lcom/lab2.h>
#include <lcom/timer.h>

#include <stdbool.h>
#include <stdint.h>

#define FUNCTION_FAIL 1
#define BAD_PARAMS 2

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab2/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab2/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;
  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();
  return 0;
}

int(timer_test_read_config)(uint8_t timer, enum timer_status_field field) {
  int returnVal;
  uint8_t st;  
  // -- timer_get_conf -- //
  returnVal = timer_get_conf(timer, &st);
  if (returnVal == FUNCTION_FAIL) {
    printf("Function fail: timer_get_conf() \n");
    return 1;
  }
  else if (returnVal == BAD_PARAMS) {
    printf("Invalid parameters in: timer_get_conf() \n");
    return 1;
  }  
  // -- timer_display_conf -- //
  returnVal = timer_display_conf(timer, st, field);
  if (returnVal == FUNCTION_FAIL) {
    printf("Function fail: timer_display_conf() \n");
    return 1;
  }
  else if (returnVal == BAD_PARAMS) {
    printf("Invalid parameters in: timer_display_conf() \n");
    return 1;
  }  
  return 0;
}

int(timer_test_time_base)(uint8_t timer, uint32_t freq) {
  int returnVal = timer_set_frequency(timer, freq);
  if (returnVal == FUNCTION_FAIL) {
    printf("Function fail: timer_set_frequency() \n");
    return 1;
  }
  else if (returnVal == BAD_PARAMS) {
    printf("Invalid parameters in: timer_set_frequency() \n");
    return 1;
  }  
  return 0;
}

extern int aux_counter;

int(timer_test_int)(uint8_t time) {
  if (time < 0) {
    printf("Invalid parameters in: timer_test_int() \n");
    return 1;
  }
  uint8_t irq_set;
  if(timer_subscribe_int(&irq_set) == FUNCTION_FAIL) { 
    printf("Function fail: timer_subscribe_int() \n");
    return 1;
  }
  int ipc_status;
  message msg;
  aux_counter = 0;
  while(aux_counter < time * 60) { 
    int r;
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            timer_int_handler();
            if(aux_counter%60==0)
              timer_print_elapsed_time();
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */	
      }
    } else { /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }
  }
  if(timer_unsubscribe_int() == FUNCTION_FAIL) { 
    printf("Function fail: timer_unsubscribe_int() \n");
    return 1;
  }
  return 0;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  *lsb = (uint8_t)(val & 0xFF);
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  *msb = (uint8_t)(val >> 8);
  return 0;
}
