// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "mouse.h"
#define FUNCTION_FAIL   1
#define KBC_COMM_ERROR  2
#define KBC_TIMEOUT     3
#define ACK_ERROR       4

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

extern struct packet mouse_packet;

int (mouse_test_packet)(uint32_t cnt) {
  uint32_t num_packets_read = 0;
  uint8_t irq_set_mouse;
  // Subscribing mouse interrupts
  if (mouse_subscribe_int(&irq_set_mouse) == FUNCTION_FAIL) {
    printf("Function fail: mouse_subscribe_int() \n");
    return 1;
  } 
  // Disabling mouse interrupts
  if (mouse_disable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_disable_int() \n");
    return 1;
  }
  // Setting stream mode
  switch(mouse_set_stream()) {
    case FUNCTION_FAIL: printf("Function fail: mouse_set_stream() \n");
    case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_set_stream() \n");
    case KBC_TIMEOUT: printf("KBC Timeout: mouse_set_stream() \n");
    case ACK_ERROR: printf("ACK Error: mouse_set_stream() \n");
    default: break;      
  }
  // Enabling data reporting
  switch(mouse_enable_data_report()) {
    case FUNCTION_FAIL: printf("Function fail: mouse_enable_data_report() \n");
    case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_enable_data_report() \n");
    case KBC_TIMEOUT: printf("KBC Timeout: mouse_enable_data_report() \n");
    case ACK_ERROR: printf("ACK Error: mouse_enable_data_report() \n");
    default: break;      
  }
  // Reenabling mouse interrupts
  if (mouse_enable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_enable_int() \n");
    return 1;
  }
  // Interrupt loop
  int ipc_status; message msg;
  while(num_packets_read < cnt) { 
    int r; // Get a request message.
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // hardware interrupt notification                                
          if (msg.m_notify.interrupts & BIT(irq_set_mouse)) { // subscribed interrupt
            mouse_ih();
            int8_t byte_read_no = mouse_assemble_packet();
            if (byte_read_no == 2) {
              mouse_print_packet(&mouse_packet);
              num_packets_read++;
            }
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
  }
  // Disabling mouse interrupts
  if (mouse_disable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_disable_int() \n");
    return 1;
  }
  // Disabling data reporting
  switch(mouse_disable_data_report()) {
    case FUNCTION_FAIL: printf("Function fail: mouse_disable_data_report() \n");
    case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_disable_data_report() \n");
    case KBC_TIMEOUT: printf("KBC Timeout: mouse_disable_data_report() \n");
    case ACK_ERROR: printf("ACK Error: mouse_disable_data_report() \n");
    default: break;      
  }
  // Reenabling mouse interrupts
  if (mouse_enable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_enable_int() \n");
    return 1;
  }
  // Unsubscribing mouse interrupts
  if (mouse_unsubscribe_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_unsubscribe_int() \n");
    return 1;
  } 
  return 0;
}


int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
  for (uint8_t i = 0; i < cnt; i++) {
    // Reads a mouse packet
    switch (mouse_read_data()) {
      case FUNCTION_FAIL: printf("Function fail: mouse_read_data() \n");
      case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_read_data() \n");
      case KBC_TIMEOUT: printf("KBC Timeout: mouse_read_data() \n");
      case ACK_ERROR: printf("ACK Error: mouse_read_data() \n");
      default: break;      
    }
    mouse_print_packet(&mouse_packet);
    util_delay(period);
  }
  // Setting stream mode
  switch(mouse_set_stream()) {
    case FUNCTION_FAIL: printf("Function fail: mouse_set_stream() \n");
    case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_set_stream() \n");
    case KBC_TIMEOUT: printf("KBC Timeout: mouse_set_stream() \n");
    case ACK_ERROR: printf("ACK Error: mouse_set_stream() \n");
    default: break;      
  }
  // Disabling data reporting
  switch(mouse_disable_data_report()) {
    case FUNCTION_FAIL: printf("Function fail: mouse_disable_data_report() \n");
    case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_disable_data_report() \n");
    case KBC_TIMEOUT: printf("KBC Timeout: mouse_disable_data_report() \n");
    case ACK_ERROR: printf("ACK Error: mouse_disable_data_report() \n");
    default: break;      
  }
  kbc_set_default();
  return 0;
}


extern uint32_t timer_aux_counter;

int (mouse_test_async)(uint8_t idle_time) {
	bool end_of_time = false;
	uint32_t hz = sys_hz(); //nao sei se e assim que funciona
	uint8_t irq_set_mouse, irq_set_timer;
  // Subscribing mouse interrupts
  if (mouse_subscribe_int(&irq_set_mouse) == FUNCTION_FAIL) {
    printf("Function fail: mouse_subscribe_int() \n");
    return 1;
  } 
  // Disabling mouse interrupts
  if (mouse_disable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_disable_int() \n");
    return 1;
  }
  // Setting stream mode
  switch(mouse_set_stream()) {
    case FUNCTION_FAIL: printf("Function fail: mouse_set_stream() \n");
    case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_set_stream() \n");
    case KBC_TIMEOUT: printf("KBC Timeout: mouse_set_stream() \n");
    case ACK_ERROR: printf("ACK Error: mouse_set_stream() \n");
    default: break;      
  }
  // Enabling data reporting
  switch(mouse_enable_data_report()) {
    case FUNCTION_FAIL: printf("Function fail: mouse_enable_data_report() \n");
    case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_enable_data_report() \n");
    case KBC_TIMEOUT: printf("KBC Timeout: mouse_enable_data_report() \n");
    case ACK_ERROR: printf("ACK Error: mouse_enable_data_report() \n");
    default: break;      
  }
  // Reenabling mouse interrupts
  if (mouse_enable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_enable_int() \n");
    return 1;
  }
  // Subscribing timer interrupts
  if (timer_subscribe_int(&irq_set_timer) == FUNCTION_FAIL) {
    printf("Function fail: timer_subscribe_int() \n");
    return 1;
  } 
	// Interrupt loop
	int ipc_status; message msg;
	while (!end_of_time) {
		int r; // Get a request message.
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { // received notification
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: // hardware interrupt notification                                
				if (msg.m_notify.interrupts & BIT(irq_set_mouse)) { // subscribed interrupt
					mouse_ih();
					int8_t byte_read_no = mouse_assemble_packet();
					if (byte_read_no == 2) {
						mouse_print_packet(&mouse_packet);
						timer_aux_counter = 0;
					}
				}
				if (msg.m_notify.interrupts & irq_set_timer) {
					timer_int_handler();
					if (timer_aux_counter >= idle_time * hz) end_of_time = true;
				}
				break;
			default:
				break; // no other notifications expected: do nothing
			}
		}
	}
  // Disabling mouse interrupts
  if (mouse_disable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_disable_int() \n");
    return 1;
  }
  // Disabling data reporting
  switch(mouse_disable_data_report()) {
    case FUNCTION_FAIL: printf("Function fail: mouse_disable_data_report() \n");
    case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_disable_data_report() \n");
    case KBC_TIMEOUT: printf("KBC Timeout: mouse_disable_data_report() \n");
    case ACK_ERROR: printf("ACK Error: mouse_disable_data_report() \n");
    default: break;      
  }
  // Reenabling mouse interrupts
  if (mouse_enable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_enable_int() \n");
    return 1;
  }
  // Unsubscribing mouse interrupts
  if (mouse_unsubscribe_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_unsubscribe_int() \n");
    return 1;
  } 
  // Unsubscribing timer interrupts
  if (timer_unsubscribe_int() == FUNCTION_FAIL) {
    printf("Function fail: timer_unsubscribe_int() \n");
    return 1;
  } 
	return 0;
}

typedef enum {INIT, DRAW_UP, VERTEX, DRAW_DOWN, COMPLETE} state_t;

// Event handler for an inverted V gesture
state_t gesture_state_machine(struct mouse_ev *evt ,uint8_t x_len, uint8_t tolerance) {
    static int32_t delX_total, delY_total;
    static state_t state = INIT;
    switch (state) {
      case INIT:
        delX_total = 0;
        delY_total = 0;
        if(evt->type == LB_PRESSED)
          state = DRAW_UP;
        break;
      case DRAW_UP:
        if(evt->type == LB_RELEASED) {
          if (delX_total >= x_len && delY_total > delX_total)
            state = VERTEX;
          else state = INIT;
        }
        else if(evt->type == BUTTON_EV)
          state = INIT;
        else if(evt->type == MOUSE_MOV) {
          delX_total += evt->delta_x;
          delY_total += evt->delta_y;
          if(evt->delta_x + tolerance < 0)
            state = INIT;
        }
        break;
      case VERTEX:
        if (evt->type == BUTTON_EV)
          state = INIT;
        else if(evt->type == LB_PRESSED) {
          delX_total = 0;
          delY_total = 0;
          state = DRAW_UP;
        }
        else if(evt->type == RB_PRESSED) {
          delX_total = 0;
          delY_total = 0;
          state = DRAW_DOWN;
        }
        else if(evt->type == MOUSE_MOV)
          if(abs(evt->delta_x) > tolerance || abs(evt->delta_y) > tolerance) 
            state = INIT;
        break;
      case DRAW_DOWN:
        if(evt->type == RB_RELEASED) {
          if (delX_total >= x_len &&  delY_total < -delX_total)
            state = COMPLETE;
          else state = INIT;
        }
        else if(evt->type == BUTTON_EV)
          state = INIT;
        else if(evt->type == MOUSE_MOV) {
          delX_total += evt->delta_x;
          delY_total += evt->delta_y;
          if(evt->delta_x + tolerance < 0)
            state = INIT;
        }
        break;
      case COMPLETE: break;
    }
  return state;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
  uint8_t irq_set_mouse;
  struct mouse_ev MOUSE_EVT;
  bool has_mouse_event = false;
  state_t state = IDLE;
  // Subscribing mouse interrupts
  if (mouse_subscribe_int(&irq_set_mouse) == FUNCTION_FAIL) {
    printf("Function fail: mouse_subscribe_int() \n");
    return 1;
  } 
  // Disabling mouse interrupts
  if (mouse_disable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_disable_int() \n");
    return 1;
  }
  // Setting stream mode
  switch(mouse_set_stream()) {
    case FUNCTION_FAIL: printf("Function fail: mouse_set_stream() \n");
    case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_set_stream() \n");
    case KBC_TIMEOUT: printf("KBC Timeout: mouse_set_stream() \n");
    case ACK_ERROR: printf("ACK Error: mouse_set_stream() \n");
    default: break;      
  }
  // Enabling data reporting
  switch(mouse_enable_data_report()) {
    case FUNCTION_FAIL: printf("Function fail: mouse_enable_data_report() \n");
    case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_enable_data_report() \n");
    case KBC_TIMEOUT: printf("KBC Timeout: mouse_enable_data_report() \n");
    case ACK_ERROR: printf("ACK Error: mouse_enable_data_report() \n");
    default: break;      
  }
  // Reenabling mouse interrupts
  if (mouse_enable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_enable_int() \n");
    return 1;
  }
  // Interrupt loop
  int ipc_status; message msg;
  while(state != COMPLETE) {
    int r; // Get a request message.
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // hardware interrupt notification                                
          if (msg.m_notify.interrupts & BIT(irq_set_mouse)) { // subscribed interrupt
            mouse_ih();
            int8_t byte_read_no = mouse_assemble_packet();
            if (byte_read_no == 2) {
              mouse_print_packet(&mouse_packet);
              MOUSE_EVT = mouse_detect_ev(&mouse_packet);
              has_mouse_event = true;
            }
          }
          break;
        default:
          break; // no other notifications expected: do nothing
      }
    }
    if(has_mouse_event) {
      state = gesture_state_machine(&MOUSE_EVT, x_len, tolerance);
      has_mouse_event = false;
    }
  }
  // Disabling mouse interrupts
  if (mouse_disable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_disable_int() \n");
    return 1;
  }
  // Disabling data reporting
  switch(mouse_disable_data_report()) {
    case FUNCTION_FAIL: printf("Function fail: mouse_disable_data_report() \n");
    case KBC_COMM_ERROR: printf("KBC Comm. Error: mouse_disable_data_report() \n");
    case KBC_TIMEOUT: printf("KBC Timeout: mouse_disable_data_report() \n");
    case ACK_ERROR: printf("ACK Error: mouse_disable_data_report() \n");
    default: break;      
  }
  // Reenabling mouse interrupts
  if (mouse_enable_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_enable_int() \n");
    return 1;
  }
  // Unsubscribing mouse interrupts
  if (mouse_unsubscribe_int() == FUNCTION_FAIL) {
    printf("Function fail: mouse_unsubscribe_int() \n");
    return 1;
  } 
  return 0;
}
