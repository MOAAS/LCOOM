#pragma once

/** @defgroup i8042 i8042
 * @{
 *
 * Constants used to program the i8024.
 */


#define BIT(n) (0x01<<(n))

#define KEYBOARD_IRQ        1
#define MOUSE_IRQ           12

#define KBC_OUT_BUF         0x60
#define KBC_IN_BUF          0x60
#define KBC_CMD_REG         0x64
#define KBC_STAT_REG        0x64

#define KBC_READ_CMD        0x20
#define KBC_WRITE_CMD       0x60
#define KBC_WRITE_TO_MOUSE  0xD4

#define KBC_INT_KBD         BIT(0)
#define KBC_INT_MOUSE       BIT(1)
#define KBC_DISABLE_KBD     BIT(4)
#define KBC_DISABLE_MOUSE   BIT(5)

// Scancode list

#define SCANCODE_PREFIX     0xE0 

#define ESC_MAKE    0x01
#define ESC_BREAK   0x81

#define F1_MAKE         0x3b
#define F2_MAKE         0x3c
#define F3_MAKE         0x3d
#define F4_MAKE         0x3e
#define F5_MAKE         0x3f
#define F6_MAKE         0x40
#define F7_MAKE         0x41
#define F8_MAKE         0x42
#define F9_MAKE         0x43
#define F10_MAKE        0x44
#define F11_MAKE        0x57
#define F12_MAKE        0x58

#define F1_BREAK        0xbb
#define F2_BREAK        0xbc
#define F3_BREAK        0xbd
#define F4_BREAK        0xbe
#define F5_BREAK        0xbf
#define F6_BREAK        0xc0
#define F7_BREAK        0xc1
#define F8_BREAK        0xc2
#define F9_BREAK        0xc3
#define F10_BREAK       0xc4
#define F11_BREAK       0xd7
#define F12_BREAK       0xd8

// '\'
#define KEY1_MAKE       0x29
#define KEY1_BREAK      0xa9 

#define NUM1_MAKE       0x02
#define NUM2_MAKE       0x03
#define NUM3_MAKE       0x04
#define NUM4_MAKE       0x05
#define NUM5_MAKE       0x06
#define NUM6_MAKE       0x07
#define NUM7_MAKE       0x08
#define NUM8_MAKE       0x09
#define NUM9_MAKE       0x0a
#define NUM0_MAKE       0x0b

#define NUM1_BREAK      0x82
#define NUM2_BREAK      0x83
#define NUM3_BREAK      0x84
#define NUM4_BREAK      0x85
#define NUM5_BREAK      0x86
#define NUM6_BREAK      0x87
#define NUM7_BREAK      0x88
#define NUM8_BREAK      0x89
#define NUM9_BREAK      0x8a
#define NUM0_BREAK      0x8b

// '
#define KEY12_MAKE      0x0c 
#define KEY12_BREAK     0x8c

// «
#define KEY13_MAKE      0x0d
#define KEY13_BREAK     0x8d

#define BACKSPACE_MAKE  0x0e
#define BACKSPACE_BREAK 0x8e

#define TAB_MAKE        0x0f
#define TAB_BREAK       0x8f

#define Q_MAKE          0x10
#define W_MAKE          0x11
#define E_MAKE          0x12
#define R_MAKE          0x13
#define T_MAKE          0x14
#define Y_MAKE          0x15
#define U_MAKE          0x16
#define I_MAKE          0x17
#define O_MAKE          0x18
#define P_MAKE          0x19

#define Q_BREAK     	0x90
#define W_BREAK     	0x91
#define E_BREAK     	0x92
#define R_BREAK     	0x93
#define T_BREAK     	0x94
#define Y_BREAK     	0x95
#define U_BREAK     	0x96
#define I_BREAK     	0x97
#define O_BREAK     	0x98
#define P_BREAK     	0x99

// +
#define KEY27_MAKE      0x1a
#define KEY27_BREAK     0x9a

// ´
#define KEY28_MAKE      0x1b
#define KEY28_BREAK     0x9b

// ~
#define KEY29_MAKE      0x2b
#define KEY29_BREAK     0xab

#define CAPSLOCK_MAKE   0x2b
#define CAPSLOCK_BREAK  0xab

#define A_MAKE          0x1e
#define S_MAKE          0x1f
#define D_MAKE          0x20
#define F_MAKE          0x21
#define G_MAKE          0x22
#define H_MAKE          0x23
#define J_MAKE          0x24
#define K_MAKE          0x25
#define L_MAKE          0x26

#define A_BREAK     	0x9e
#define S_BREAK     	0x9f
#define D_BREAK     	0xa0
#define F_BREAK     	0xa1
#define G_BREAK     	0xa2
#define H_BREAK     	0xa3
#define J_BREAK     	0xa4
#define K_BREAK     	0xa5
#define L_BREAK     	0xa6

// ç
#define KEY40_MAKE      0x27
#define KEY40_BREAK     0x28

// º
#define KEY41_MAKE      0xa7
#define KEY41_BREAK     0xa8

#define ENTER_MAKE      0x1c
#define ENTER_BREAK     0x9c

#define LSHIFT_MAKE     0x2a
#define LSHIFT_BREAK    0xaa

#define Z_MAKE          0x2c
#define X_MAKE          0x2d
#define C_MAKE          0x2e
#define V_MAKE          0x2f
#define B_MAKE          0x30
#define N_MAKE          0x31
#define M_MAKE          0x32
#define COMMA_MAKE      0x33
#define PERIOD_MAKE     0x34

#define Z_BREAK     	0xac
#define X_BREAK     	0xad
#define C_BREAK     	0xae
#define V_BREAK     	0xaf
#define B_BREAK     	0xb0
#define N_BREAK     	0xb1
#define M_BREAK     	0xb2
#define COMMA_BREAK     0xb3
#define PERIOD_BREAK    0xb4

// -
#define KEY55_MAKE      0x35
#define KEY55_BREAK     0xb5

#define RSHIFT_MAKE     0x36
#define RSHIFT_BREAK    0xb6

#define LCTRL_MAKE      0x1d
#define LCTRL_BREAK     0x9d

#define LALT_MAKE       0x38
#define LALT_BREAK      0xb8

#define SPACE_MAKE      0x39
#define SPACE_BREAK     0xb9

#define RALT_MAKE       0xe038
#define RALT_BREAK      0xe0b8

#define RCTRL_MAKE      0xe01d
#define RCTRL_BREAK     0xe09d

#define ARROW_U_MAKE    0xe048
#define ARROW_D_MAKE    0xe050
#define ARROW_L_MAKE    0xe04b
#define ARROW_R_MAKE    0xe04d

#define ARROW_U_BREAK   0xe0c8
#define ARROW_D_BREAK   0xe0d0
#define ARROW_L_BREAK   0xe0cb
#define ARROW_R_BREAK   0xe0cd

#define KEYPAD1_MAKE    0x4f
#define KEYPAD2_MAKE    0x50
#define KEYPAD3_MAKE    0x51
#define KEYPAD4_MAKE    0x4b
#define KEYPAD5_MAKE    0x4c
#define KEYPAD6_MAKE    0x4d
#define KEYPAD7_MAKE    0x47
#define KEYPAD8_MAKE    0x48
#define KEYPAD9_MAKE    0x49
#define KEYPAD0_MAKE    0x52

#define KEYPAD1_BREAK   0xcf
#define KEYPAD2_BREAK   0xd0
#define KEYPAD3_BREAK   0xd1
#define KEYPAD4_BREAK   0xcb
#define KEYPAD5_BREAK   0xcc
#define KEYPAD6_BREAK   0xcd
#define KEYPAD7_BREAK   0xc7
#define KEYPAD8_BREAK   0xc8
#define KEYPAD9_BREAK   0xc9
#define KEYPAD0_BREAK   0xd2

#define KBC_STATREG_OBF     BIT(0)
#define KBC_STATREG_IBF     BIT(1)
#define KBC_STATREG_SYS     BIT(2)
#define KBC_STATREG_A2      BIT(3)
#define KBC_STATREG_INH     BIT(4)
#define KBC_STATREG_Aux     BIT(5)
#define KBC_STATREG_TO      BIT(6)
#define KBC_STATREG_PAR     BIT(7)

#define KBC_DELAY_MS        20
#define MOUSE_DELAY_MS      25

#define TRIES_TIL_TIMEOUT   5

#define MOUSE_RESET         0xFF
#define MOUSE_RESEND        0xFE
#define MOUSE_SET_DEFAULTS  0xF6
#define MOUSE_DISABLE_RPRT  0xF5
#define MOUSE_ENABLE_RPRT   0xF4
#define MOUSE_SET_SAMPLE    0xF3
#define MOUSE_SET_REMOTE    0xF0
#define MOUSE_READ_DATA     0xEB
#define MOUSE_SET_STREAM    0xEA
#define MOUSE_STAT_REQ      0xE9
#define MOUSE_SET_RES       0xE8
#define MOUSE_ACCEL_MODE    0xE7
#define MOUSE_LIN_MODE      0xE6

#define MOUSE_ACK           0xFA
#define MOUSE_NACK          0xFE
#define MOUSE_ACK_ERROR     0xFC
