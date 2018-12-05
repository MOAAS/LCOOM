#include <lcom/lcf.h>
#include <stdint.h>
#include "mouse.h"

#define FUNCTION_FAIL   1
#define KBC_COMM_ERROR  2
#define KBC_TIMEOUT     3
#define ACK_ERROR       4

int hook_id_mouse = 4;

int mouse_subscribe_int(uint8_t *bit_no) {
    *bit_no = hook_id_mouse;
    if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE , &hook_id_mouse) != OK)
        return FUNCTION_FAIL;
    return 0;
}

int mouse_unsubscribe_int() {
    int8_t ret = sys_irqrmpolicy(&hook_id_mouse);
    kbc_clear_OUT_BUF(MOUSE_DELAY_MS);
    return ret;
}

int mouse_disable_int() {
    if (sys_irqdisable(&hook_id_mouse) != OK)
        return FUNCTION_FAIL;
    return 0;
}

int mouse_enable_int() {
    if (sys_irqenable(&hook_id_mouse) != OK)
        return FUNCTION_FAIL;
    return 0;
}

int mouse_get_ack(uint8_t* ack_byte) {
    *ack_byte = 0;
    while (*ack_byte != MOUSE_ACK && *ack_byte != MOUSE_NACK && *ack_byte != MOUSE_ACK_ERROR) {
        switch (kbc_poll_out_buf(ack_byte)) {
            case FUNCTION_FAIL: return FUNCTION_FAIL;
            case KBC_COMM_ERROR: return KBC_COMM_ERROR;
            case KBC_TIMEOUT: return KBC_TIMEOUT;
            default: break;
        }
    }
    return 0;
}

int mouse_write_cmd(uint8_t byte, bool hasArgument, uint8_t arg) {
    switch (kbc_write_cmd(KBC_WRITE_TO_MOUSE)) {
        case KBC_COMM_ERROR: return KBC_COMM_ERROR;
        case KBC_TIMEOUT: return KBC_TIMEOUT;
        default: break;      
    }
    uint8_t ack_byte = 0;
    while (ack_byte != MOUSE_ACK) {
        // Sends the command
        switch (kbc_poll_in_buf(byte)) {
            case FUNCTION_FAIL: return FUNCTION_FAIL;
            case KBC_COMM_ERROR: return KBC_COMM_ERROR;
            case KBC_TIMEOUT: return KBC_TIMEOUT;
            default: break;      
        }
        // Gets the ACK
        switch (mouse_get_ack(&ack_byte)) {
            case FUNCTION_FAIL: return FUNCTION_FAIL;
            case KBC_COMM_ERROR: return KBC_COMM_ERROR;
            case KBC_TIMEOUT: return KBC_TIMEOUT;
            default: break;
        }
        switch (ack_byte) {
            case MOUSE_ACK: break;
            case MOUSE_NACK: continue;
            case MOUSE_ACK_ERROR: return ACK_ERROR;
            default:
                printf("ERROR: Unrecognized ACK_BYTE: %d \n", ack_byte);
                return ACK_ERROR;
        }
        if (!hasArgument) 
            return 0;
        // Same for the argument...
        // Sends the command
        switch (kbc_poll_in_buf(arg)) {
            case FUNCTION_FAIL: return FUNCTION_FAIL;
            case KBC_COMM_ERROR: return KBC_COMM_ERROR;
            case KBC_TIMEOUT: return KBC_TIMEOUT;
            default: break;      
        }
        // Gets the ACK: UNUSED FOR NOW
        switch (mouse_get_ack(&ack_byte)) {
            case FUNCTION_FAIL: return FUNCTION_FAIL;
            case KBC_COMM_ERROR: return KBC_COMM_ERROR;
            case KBC_TIMEOUT: return KBC_TIMEOUT;
            default: break;
        }
        switch (ack_byte) {
            case MOUSE_ACK: break;
            case MOUSE_NACK: continue;
            case MOUSE_ACK_ERROR: return ACK_ERROR;
            default:
                printf("ERROR: Unrecognized ACK_BYTE: %d \n", ack_byte);
                return ACK_ERROR;
        }
    }
    return 0;

}

int mouse_set_stream() {
    return mouse_write_cmd(MOUSE_SET_STREAM, false, 0);
}

int mouse_set_remote() {
    return mouse_write_cmd(MOUSE_SET_REMOTE, false, 0);
}

int mouse_enable_data_report() {
    return mouse_write_cmd(MOUSE_ENABLE_RPRT, false, 0);
}

int mouse_disable_data_report() {
    return mouse_write_cmd(MOUSE_DISABLE_RPRT, false, 0);
}

uint8_t packet_byte;
struct packet mouse_packet;

int mouse_read_data() {    
    switch(mouse_write_cmd(MOUSE_READ_DATA, false, 0)) {
        case FUNCTION_FAIL: return FUNCTION_FAIL;
        case KBC_COMM_ERROR: return KBC_COMM_ERROR;
        case KBC_TIMEOUT: return KBC_TIMEOUT;
        case ACK_ERROR: return ACK_ERROR;
        default: break;      
    }
    // Should get the three bytes and sync them if needed
    uint8_t byte_read_no;
    do {
        switch (kbc_get_out_buf(&packet_byte)) {
            case FUNCTION_FAIL: return FUNCTION_FAIL;
            case KBC_COMM_ERROR: return KBC_COMM_ERROR;
            case KBC_TIMEOUT: return KBC_TIMEOUT;
            default: break;
        }
        byte_read_no = mouse_assemble_packet();
    } while (byte_read_no != 2);
    return 0;
}

void (mouse_ih)() {
    kbc_get_out_buf(&packet_byte);
}

int8_t mouse_assemble_packet() {
    static uint8_t byte_no = 0;
    if (!mouse_is_packet_synced(packet_byte, byte_no)) {
        byte_no = 0;
        return 0;
    }
    mouse_packet.bytes[byte_no] = packet_byte;
    switch(byte_no) {
    case 0:
        mouse_packet.lb = packet_byte & MOUSE_LB;
        mouse_packet.rb = packet_byte & MOUSE_RB;
        mouse_packet.mb = packet_byte & MOUSE_MB;
        mouse_packet.x_ov = packet_byte & MOUSE_X_OVF;
        mouse_packet.y_ov = packet_byte & MOUSE_Y_OVF;
        if (packet_byte & MOUSE_XSIGN)
            mouse_packet.delta_x = SIGNED_DELTA;
        else mouse_packet.delta_x = UNSIGNED_DELTA;
        if (packet_byte & MOUSE_YSIGN)
            mouse_packet.delta_y = SIGNED_DELTA;
        else mouse_packet.delta_y = UNSIGNED_DELTA;
        byte_no = 1;
        return 0;
    case 1:
        mouse_packet.delta_x |= packet_byte;
        byte_no = 2;
        return 1;
    case 2:
        mouse_packet.delta_y |= packet_byte;
        byte_no = 0;
        return 2;
    default:
        byte_no = 0;
        return -1;
    }
}

bool mouse_is_packet_synced(uint8_t packet_byte, uint8_t byte_no) {
    return byte_no != 0 || (packet_byte & BIT(3));
}

struct mouse_ev mouse_detect_ev(struct packet *pp) {
    static bool is_LB_PRESSED = false, is_RB_PRESSED = false, is_MB_PRESSED = false;
    struct mouse_ev event;
    // LB was pressed
    if (pp->lb != is_LB_PRESSED) {
        if (pp->lb && !is_RB_PRESSED && !is_MB_PRESSED) event.type = LB_PRESSED;
        else if (pp->lb || is_RB_PRESSED || is_MB_PRESSED) event.type = BUTTON_EV;
        else event.type = LB_RELEASED;
        is_LB_PRESSED = pp->lb;
    }
    // RB was pressed
    else if (pp->rb != is_RB_PRESSED) {
        if (pp->rb && !is_LB_PRESSED && !is_MB_PRESSED) event.type = RB_PRESSED;
        else if (pp->rb || is_LB_PRESSED || is_MB_PRESSED) event.type = BUTTON_EV;
        else event.type = RB_RELEASED;
        is_RB_PRESSED = pp->rb;
    }
    // MB was pressed
    else if (pp->mb != is_MB_PRESSED) {
        event.type = BUTTON_EV;
        is_MB_PRESSED = pp->mb;
    }
    // Any mouse movement
    else if (pp->delta_x || pp->delta_y) {
        event.type = MOUSE_MOV;
        event.delta_x = pp->delta_x;
        event.delta_y = pp->delta_y;
    }
    // Some other packet sending that is not supported, such as side mouse button pressing
    else event.type = BUTTON_EV;
    return event;
}

void mouse_print_evt(struct mouse_ev* event) {
    switch(event->type) {
        case LB_PRESSED: printf("LB PRESSED \n"); break;
        case RB_PRESSED: printf("RB PRESSED \n"); break;
        case LB_RELEASED: printf("LB RELEASED \n"); break;
        case RB_RELEASED: printf("RB RELEASED \n"); break;
        case MOUSE_MOV: printf("MOUSE MOV (%d, %d) \n", event->delta_x, event->delta_y); break;
        case BUTTON_EV: printf("BUTTON EV \n"); break;
        default: printf("Unrecognized event! \n"); break;
    }
}
