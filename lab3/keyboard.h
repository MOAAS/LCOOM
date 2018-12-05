#pragma once

/**
 * @brief Subscribes and enables keyboard interrupts, in exclusive mode.
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int keyboard_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int keyboard_unsubscribe_int();

/**
 * @brief While OUT_BUF is full, discards its contents.
 * 
 */
void kbc_clear_OUT_BUF();

/**
 * @brief Reads keyboard's output buffer
 * 
 * @param value Address of memory which will store the contents of the output buffer
 * @return Return 0 upon success and non-zero otherwise 
 */
int keyboard_read_OUT_BUF(uint8_t *value);

/**
 * @brief Gets the status byte from the KBC status register
 * 
 * @param status_byte Address of memory which will store the status byte
 * @return Return 0 upon success and non-zero otherwise 
 */
int keyboard_get_status(uint8_t *status_byte);

/**
 * @brief Gets the command byte from the KBC
 * 
 * @param cmd_byte Address of memory which will store the command byte
 * @return Return 0 upon success and non-zero otherwise 
 */
int keyboard_get_cmd_byte(uint8_t *cmd_byte);

/**
 * @brief Writes the command byte to the kbc
 * 
 * @param cmd_byte Byte to be send as an argument of the Write command byte command
 * @return Return 0 upon success and non-zero otherwise 
 */
int keyboard_write_cmd_byte(uint8_t cmd_byte);

/**
 * @brief Issues a command to the kbc
 * 
 * @param cmd Command to be issued to the KBC
 * @return Return 0 upon success and non-zero otherwise 
 */
int keyboard_write_cmd(uint8_t cmd);

/**
 * @brief Gets a scancode from the output buffer, if there is one.
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int keyboard_get_scancode();

/**
 * @brief Keeps polling the kbc to until there's something in the Output Buffer
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_enable_poll();

/**
 * @brief Checks if there was any communication error between Keyboard and KBC
 * 
 * @param status_byte status byte that will be analyzed, should have the most recent info from the kbc
 * @return true There was a communication error
 * @return false There was no communication error
 */
bool kbd_has_communication_err(uint8_t status_byte);

/**
 * @brief Waits
 * 
 * @param ms How much time to wait (in milliseconds)
 */
void util_delay(uint32_t ms);

/**
 * @brief Checks if the Input buffer is full
 * 
 * @param status status byte that will be analyzed, should have the most recent info from the kbc
 * @return true Input buffer is full
 * @return false Input buffer is not full
 */
bool kbc_IBF_is_full(uint8_t status);

/**
 * @brief Checks if the Output buffer is full
 * 
 * @param status status byte that will be analyzed, should have the most recent info from the kbc
 * @return true Output buffer is full
 * @return false Output buffer is not full
 */
bool kbc_OBF_is_full(uint8_t status);


#ifdef LAB3
int sys_inb_cnt(port_t port, uint32_t *byte);
#else
#define sys_inb_cnt(p,q) sys_inb(p,q)
#endif
