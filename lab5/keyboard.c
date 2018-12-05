#include <lcom/lcf.h>
#include <stdint.h>
#include "keyboard.h"
#include "i8042.h"

#define FUNCTION_FAIL   1
#define KBC_COMM_ERROR  2
#define KBC_TIMEOUT     3

static int hook_id_keyboard = IRQ_SET_KEYBOARD;

int keyboard_subscribe_int(uint8_t *bit_no) {
    *bit_no = hook_id_keyboard = IRQ_SET_KEYBOARD;
    if (sys_irqsetpolicy(KEYBOARD_IRQ ,IRQ_REENABLE | IRQ_EXCLUSIVE , &hook_id_keyboard) != OK)
        return FUNCTION_FAIL;
    return 0;
}

int keyboard_unsubscribe_int() {    
    kbc_clear_kbd_OUT_BUF();
    if (sys_irqrmpolicy(&hook_id_keyboard) != OK)
        return FUNCTION_FAIL;
    return 0;
}

int keyboard_disable_int() {
    if (sys_irqdisable(&hook_id_keyboard) != OK)
        return FUNCTION_FAIL;
    return 0;
}

int keyboard_enable_int() {
    if (sys_irqenable(&hook_id_keyboard) != OK)
        return FUNCTION_FAIL;
    return 0;
}

void kbc_clear_mouse_OUT_BUF() {
    uint8_t status; uint32_t temp;
    util_delay(MOUSE_DELAY_MS);
    if (kbc_get_status(&status) == FUNCTION_FAIL) return;
    while (kbc_OBF_is_full(status) && kbc_OBF_is_mouse_data(status)) {
        if (sys_inb(KBC_OUT_BUF, &temp) != OK) return;
        util_delay(MOUSE_DELAY_MS);
        if (kbc_get_status(&status) == FUNCTION_FAIL) return;
    }
}

void kbc_clear_kbd_OUT_BUF() {
    uint8_t status; uint32_t temp;
    util_delay(KBC_DELAY_MS);
    if (kbc_get_status(&status) == FUNCTION_FAIL) return;
    while (kbc_OBF_is_full(status) && !kbc_OBF_is_mouse_data(status)) {
        if (sys_inb(KBC_OUT_BUF, &temp) != OK) return;
        util_delay(KBC_DELAY_MS);
        if (kbc_get_status(&status) == FUNCTION_FAIL) return;
    }
}

int kbc_read_OUT_BUF(uint8_t *value) {
    uint32_t temp;
    if (sys_inb(KBC_OUT_BUF, &temp) != OK)
        return FUNCTION_FAIL;
    *value = (uint8_t)temp;
    return 0;
}

int kbc_get_status(uint8_t *status_byte) {
    // Simply reads the status register and returns it through the argument
    uint32_t status_temp;
    if(sys_inb(KBC_STAT_REG, &status_temp) != OK)
        return FUNCTION_FAIL;
    *status_byte = (uint8_t)status_temp;
    return 0;
}

int kbc_poll_out_buf(uint8_t* byte) {
    // Loops while OUT_BUF is empty, after X tries exits with KBC_TIMEOUT.
    for (int i = 0; i < TRIES_TIL_TIMEOUT; i++) { 
        // Checks the status byte first
        uint8_t status;
        if (kbc_get_status(&status) == FUNCTION_FAIL) return FUNCTION_FAIL;
        if (kbc_has_communication_err(status)) return KBC_COMM_ERROR;       
        if (kbc_OBF_is_full(status))
            return kbc_read_OUT_BUF(byte);
        util_delay(MOUSE_DELAY_MS);
    }
    return KBC_TIMEOUT;
}

int kbc_poll_in_buf(uint8_t byte) {
    // Loops while IN_BUF is not empty, after X tries exits with KBC_TIMEOUT.
    for (int i = 0; i < TRIES_TIL_TIMEOUT; i++) { 
        // Checks the status byte first
        uint8_t status;
        if (kbc_get_status(&status) == FUNCTION_FAIL) return FUNCTION_FAIL;
        if (kbc_has_communication_err(status)) return KBC_COMM_ERROR;       
        if (!kbc_IBF_is_full(status))
            return sys_outb(KBC_IN_BUF, (uint32_t)byte);
        util_delay(MOUSE_DELAY_MS);
    }
    return KBC_TIMEOUT;
}

int kbc_set_default() {
    uint8_t cmd_byte = minix_get_dflt_kbc_cmd_byte();
    return kbc_write_cmd_byte(cmd_byte);
}

int kbc_get_cmd_byte(uint8_t *cmd_byte) {
    // Writes the read-command-byte command to the kbc
    switch (kbc_write_cmd(KBC_READ_CMD)) {
        case KBC_COMM_ERROR: return KBC_COMM_ERROR;
        case KBC_TIMEOUT: return KBC_TIMEOUT;
        default: break;      
    }
    return kbc_poll_out_buf(cmd_byte);
}


int kbc_write_cmd_byte(uint8_t cmd_byte) {
    // Writes the write-command-byte command to the kbc
    switch (kbc_write_cmd(KBC_WRITE_CMD)) {
        case KBC_COMM_ERROR: return KBC_COMM_ERROR;
        case KBC_TIMEOUT: return KBC_TIMEOUT;
        default: break;      
    }
    return kbc_poll_in_buf(cmd_byte);
}

int kbc_write_cmd(uint8_t cmd) {
    // Loops while IN_BUF is not empty, after X tries exits with KBC_TIMEOUT.
    for (int i = 0; i < TRIES_TIL_TIMEOUT; i++) { 
        // Checks the status byte first
        uint8_t status;
        if (kbc_get_status(&status) == FUNCTION_FAIL) return FUNCTION_FAIL;
        if (kbc_has_communication_err(status)) return KBC_COMM_ERROR;
        if (!kbc_IBF_is_full(status))
            return sys_outb(KBC_CMD_REG, (uint32_t)cmd);
        util_delay(MOUSE_DELAY_MS);
    }
    return KBC_TIMEOUT;
}

uint16_t scancode = 0;
static uint8_t scancode_byte = 0;

void (kbc_ih)() {
    kbc_get_out_buf(&scancode_byte);
}

int kbc_enable_poll() {
    uint8_t status;
    // Loops while OUT_BUF is empty (Also checks AUX bit but not as relevant for now)
    while(1) {
        // Checks the status byte first, then goes to read the OUT_BUF if it has something.
        if (kbc_get_status(&status) == FUNCTION_FAIL) return FUNCTION_FAIL;
        if (kbc_OBF_is_full(status) && !(status & KBC_STATREG_Aux)) {
            switch (kbc_get_out_buf(&scancode_byte)) {
                case FUNCTION_FAIL: return FUNCTION_FAIL;
                case KBC_COMM_ERROR: return KBC_COMM_ERROR;
                case KBC_TIMEOUT: return KBC_TIMEOUT;
                default: break;
            }
            if (kbd_assemble_scancode())
                return 0;
        }
        util_delay(KBC_DELAY_MS);
    }
}

bool kbd_assemble_scancode() {
    if (scancode == SCANCODE_PREFIX) {
        scancode |= scancode_byte << 8;
        return true;
    }
    scancode = scancode_byte;
    if (scancode == SCANCODE_PREFIX)
        return false;
    return true;
}

int kbc_get_out_buf(uint8_t* content) {
    uint8_t status;
    // The loop isn't that necessary, but doesn't hurt to recheck.
    for (int i = 0; i < TRIES_TIL_TIMEOUT; i++) { 
        // Gets here when there's something to be read.
        // Checks the status byte first
        if (kbc_get_status(&status) == FUNCTION_FAIL) return FUNCTION_FAIL;
        if (kbc_has_communication_err(status)) {
            // If there's a communication error, returns an error and discards the contents
            uint8_t temp;
            kbc_read_OUT_BUF(&temp);
            return KBC_COMM_ERROR;
        }
        // If the OUT_BUF is full, reads it and leaves
        if (kbc_OBF_is_full(status))
            return kbc_read_OUT_BUF(content);
    }
    return KBC_TIMEOUT;
}

// Utility functions

bool kbc_has_communication_err(uint8_t status_byte) {
    // Returns true if PAR_ERROR or TIMEOUT_ERROR bits are set to 1
    return status_byte & (KBC_STATREG_PAR | KBC_STATREG_TO);
}

void util_delay(uint32_t ms) {
    tickdelay(micros_to_ticks(1000*ms));
}

bool kbc_IBF_is_full(uint8_t status) {
    // Returns true if IBF bit is set to 1
    return status & KBC_STATREG_IBF;
}

bool kbc_OBF_is_full(uint8_t status) {
    // Returns true if OBF bit is set to 1
    return status & KBC_STATREG_OBF;
}

bool kbc_OBF_is_mouse_data(uint8_t status) {
   return status & KBC_STATREG_Aux;
}
