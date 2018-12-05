#include <lcom/lcf.h>
#include <stdint.h>
#include "keyboard.h"
#include "i8042.h"

#ifdef LAB3
uint32_t sys_in_counter = 0;
int sys_inb_cnt(port_t port, uint32_t *byte) {
    sys_in_counter++;
    return sys_inb(port, byte);
}
#endif

#define FUNCTION_FAIL   1
#define KBD_COMM_ERROR  2
#define KBD_TIMEOUT     3

int hook_id_keyboard;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
    const uint8_t BIT_SET = 1;
    hook_id_keyboard = BIT_SET;
    if (sys_irqsetpolicy(KEYBOARD_IRQ ,IRQ_REENABLE | IRQ_EXCLUSIVE , &hook_id_keyboard) != OK)
        return FUNCTION_FAIL;
    *bit_no = BIT(BIT_SET);
    return 0;
}

int (keyboard_unsubscribe_int)() {    
    kbc_clear_OUT_BUF();
    if (sys_irqrmpolicy(&hook_id_keyboard) != OK)
        return FUNCTION_FAIL;
    return 0;
}

void (kbc_clear_OUT_BUF)() {
    uint8_t status; uint32_t temp;
    util_delay(KBD_DELAY_MS);
    if (keyboard_get_status(&status) == FUNCTION_FAIL) return;
    while (kbc_OBF_is_full(status)) {
        if (sys_inb_cnt(KBC_OUT_BUF, &temp) != OK) return;
        util_delay(KBD_DELAY_MS);
        if (keyboard_get_status(&status) == FUNCTION_FAIL) return;
    }
}

int (keyboard_read_OUT_BUF)(uint8_t *value) {
    uint32_t temp;
    if (sys_inb_cnt(KBC_OUT_BUF, &temp) != OK)
        return FUNCTION_FAIL;
    *value = (uint8_t)temp;
    return 0;
}

int (keyboard_get_status)(uint8_t *status_byte) {
    // Simply reads the status register and returns it through the argument
    uint32_t status_temp;
    if(sys_inb_cnt(KBC_STAT_REG, &status_temp) != OK)
        return FUNCTION_FAIL;
    *status_byte = (uint8_t)status_temp;
    return 0;
}

int (keyboard_get_cmd_byte)(uint8_t *cmd_byte) {
    // Writes the read-command-byte command to the kbc
    switch (keyboard_write_cmd(KBC_READ_CMD)) {
        case KBD_COMM_ERROR: return KBD_COMM_ERROR;
        case KBD_TIMEOUT: return KBD_TIMEOUT;
        default: break;      
    }
    // Loops while OUT_BUF is empty, after X tries exits with KBD_TIMEOUT.
    for (int i = 0; i < TRIES_TIL_TIMEOUT; i++) { 
        // Checks the status byte first
        uint8_t status;
        if (keyboard_get_status(&status) == FUNCTION_FAIL) return FUNCTION_FAIL;
        if (kbd_has_communication_err(status)) return KBD_COMM_ERROR;       
        if (kbc_OBF_is_full(status))
            return keyboard_read_OUT_BUF(cmd_byte);
        util_delay(KBD_DELAY_MS);
    }
    return KBD_TIMEOUT;
}

int (keyboard_write_cmd_byte)(uint8_t cmd_byte) {
    // Writes the write-command-byte command to the kbc
    switch (keyboard_write_cmd(KBC_WRITE_CMD)) {
        case KBD_COMM_ERROR: return KBD_COMM_ERROR;
        case KBD_TIMEOUT: return KBD_TIMEOUT;
        default: break;      
    }
    // Loops while IN_BUF is not empty, after X tries exits with KBD_TIMEOUT.
    for (int i = 0; i < TRIES_TIL_TIMEOUT; i++) { 
        // Checks the status byte first
        uint8_t status;
        if (keyboard_get_status(&status) == FUNCTION_FAIL) return FUNCTION_FAIL;
        if (kbd_has_communication_err(status)) return KBD_COMM_ERROR;       
        // Although arguments go to 0x60, we need to check IBF, not OBF.
        if (!kbc_IBF_is_full(status))
            return sys_outb(KBC_ARG_REG, (uint32_t)cmd_byte);
        util_delay(KBD_DELAY_MS);
    }
    return KBD_TIMEOUT;
}

int (keyboard_write_cmd)(uint8_t cmd) {
    // Loops while IN_BUF is not empty, after X tries exits with KBD_TIMEOUT.
    for (int i = 0; i < TRIES_TIL_TIMEOUT; i++) { 
        // Checks the status byte first
        uint8_t status;
        if (keyboard_get_status(&status) == FUNCTION_FAIL) return FUNCTION_FAIL;
        if (kbd_has_communication_err(status)) return KBD_COMM_ERROR;
        if (!kbc_IBF_is_full(status))
            return sys_outb(KBC_CMD_REG, (uint32_t)cmd);
        util_delay(KBD_DELAY_MS);
    }
    return KBD_TIMEOUT;
}

uint8_t scancode;
uint8_t kbc_ih_return = 0;

void (kbc_ih)() {
    kbc_ih_return = keyboard_get_scancode();
}

int (kbc_enable_poll)() {
    uint8_t status;
    // Loops while OUT_BUF is empty (Also checks AUX bit but not as relevant for now)
    while(1) {
        // Checks the status byte first, then goes to read the OUT_BUF if it has something.
        if (keyboard_get_status(&status) == FUNCTION_FAIL) return FUNCTION_FAIL;
        if (kbc_OBF_is_full(status) && !(status & KBC_STATREG_Aux))
            return keyboard_get_scancode();
        util_delay(KBD_DELAY_MS);
    }
}


int (keyboard_get_scancode)() {
    uint8_t status;
    // The loop isn't that necessary, but doesn't hurt to recheck.
    for (int i = 0; i < TRIES_TIL_TIMEOUT; i++) { 
        // Gets here when there's a scancode to be read.
        // Checks the status byte first
        if (keyboard_get_status(&status) == FUNCTION_FAIL) return FUNCTION_FAIL;
        if (kbd_has_communication_err(status)) {
            // If there's a communication error, returns an error and discards the scancode.
            // But it still needs to take it from the OUT_BUF!
            uint8_t temp;
            keyboard_read_OUT_BUF(&temp);
            return KBD_COMM_ERROR;
        }
        // If the OUT_BUF is full, reads it and leaves
        if (kbc_OBF_is_full(status))
            return keyboard_read_OUT_BUF(&scancode);
    }
    return KBD_TIMEOUT;
}

// Utility functions

bool (kbd_has_communication_err)(uint8_t status_byte) {
    // Returns true if PAR_ERROR or TIMEOUT_ERROR bits are set to 1
    return status_byte & (KBC_STATREG_PAR | KBC_STATREG_TO);
}

void (util_delay)(uint32_t ms) {
    tickdelay(micros_to_ticks(1000*ms));
}

bool (kbc_IBF_is_full)(uint8_t status) {
    // Returns true if IBF bit is set to 1
    return status & KBC_STATREG_IBF;
}

bool (kbc_OBF_is_full)(uint8_t status) {
    // Returns true if OBF bit is set to 1
    return status & KBC_STATREG_OBF;
}
