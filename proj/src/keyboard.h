#pragma once

/** @defgroup keyboard keyboard
 * @{
 *
 * Functions and constants used to program the PC Keyboard and its controller, the KBC.
 */

#define IRQ_SET_KEYBOARD    1

/**
 * @brief Subscribes and enables keyboard interrupts, in exclusive mode.
 *
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
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
 * @brief Disables keyboard interrupts.
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int keyboard_disable_int();

/**
 * @brief Enables keyboard interrupts.
 * 
 * @return Return 0 upon success and non-zero otherwise
 */

int keyboard_enable_int();

/**
 * @brief While OUT_BUF is "full" with a keyboard byte, discards its contents.
 * 
 */
void kbc_clear_kbd_OUT_BUF();

/**
 * @brief While OUT_BUF is "full" with a mouse byte, discards its contents.
 * 
 */
void kbc_clear_mouse_OUT_BUF();

/**
 * @brief Reads keyboard's output buffer
 * 
 * @param value Address of memory which will store the contents of the output buffer
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_read_OUT_BUF(uint8_t *value);

/**
 * @brief Gets the status byte from the KBC status register
 * 
 * @param status_byte Address of memory which will store the status byte
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_get_status(uint8_t *status_byte);

/**
 * @brief Polls the out buffer until it has something, in which case reads it. Returns after several failed attempts.
 * 
 * @param byte Address of memory which will store the content of the output buffer.
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_poll_out_buf(uint8_t* byte);

/**
 * @brief Polls the input buffer until it is empty, in which case puts byte inside it. Returns after several failed attempts.
 * 
 * @param byte Byte to be inserted
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_poll_in_buf(uint8_t byte);

/**
 * @brief Sets the KBC to its default mode by writing to the command byte
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_set_default();


/**
 * @brief Gets the command byte from the KBC
 * 
 * @param cmd_byte Address of memory which will store the command byte
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_get_cmd_byte(uint8_t *cmd_byte);

/**
 * @brief Writes the command byte to the kbc
 * 
 * @param cmd_byte Byte to be send as an argument of the Write command byte command
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_write_cmd_byte(uint8_t cmd_byte);

/**
 * @brief Issues a command to the kbc
 * 
 * @param cmd Command to be issued to the KBC
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_write_cmd(uint8_t cmd);

/**
 * @brief Gets the contents from the output buffer, if they exist.
 * 
 * @param content Address where to put whatever comes from OUT_BUF 
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_get_out_buf(uint8_t* content);

/**
 * @brief Keeps polling the kbc to until there's something in the Output Buffer
 * 
 * @return Return 0 upon success and non-zero otherwise 
 */
int kbc_enable_poll();

/**
 * @brief Assembles a scancode received by the IH. May also use the previous scancode if it's a 2-byte scancode.
 * 
 * @return true The byte is the scancode's final byte
 * @return false The byte is not the scancode's final byte.
 */
bool kbd_assemble_scancode();

/**
 * @brief Checks if there was any communication error between Keyboard and KBC
 * 
 * @param status_byte status byte that will be analyzed, should have the most recent info from the kbc
 * @return true There was a communication error
 * @return false There was no communication error
 */
bool kbc_has_communication_err(uint8_t status_byte);

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

/**
 * @brief Checks if the OBF contains mouse byte.
 * 
 * @param status status byte that will be analyzed, should have the most recent info from the kbc
 * @return true OBF contains a mouse byte.
 * @return false OBF does not contain a mouse byte.
 */
bool kbc_OBF_is_mouse_data(uint8_t status);
