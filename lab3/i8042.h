#pragma once

#define BIT(n) (0x01<<(n))
#define KEYBOARD_IRQ    1

#define KBC_OUT_BUF     0x60
#define KBC_ARG_REG     0x60
#define KBC_CMD_REG     0x64
#define KBC_STAT_REG    0x64

#define KBC_READ_CMD    0x20
#define KBC_WRITE_CMD   0x60

#define KBC_INT_KBD     BIT(0)
#define KBC_INT_MOUSE   BIT(1)

#define ESC_KEY_BREAK   0x81 
#define SCANCODE_PREFIX 0xE0 

#define KBC_STATREG_OBF BIT(0)
#define KBC_STATREG_IBF BIT(1)
#define KBC_STATREG_SYS BIT(2)
#define KBC_STATREG_A2  BIT(3)
#define KBC_STATREG_INH BIT(4)
#define KBC_STATREG_Aux BIT(5)
#define KBC_STATREG_TO  BIT(6)
#define KBC_STATREG_PAR BIT(7)

#define KBD_DELAY_MS        20

#define TRIES_TIL_TIMEOUT   5
