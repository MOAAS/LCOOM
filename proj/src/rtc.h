#pragma once
#include "i8042.h"

/** @defgroup rtc rtc
 * @{
 *
 * Functions and constants used to program the PC's Real time Clock (RTC).
 */

#define RTC_IRQ 0x08
#define RTC_ADDR_REG 0X70
#define RTC_DATA_REG 0x71
#define RTC_REG_A 0X0A
#define RTC_REG_B 0X0B
#define RTC_REG_C 0X0C
#define RTC_REG_SEC 0X00
#define RTC_REG_MIN 0X02
#define RTC_REG_HOR 0X04
#define RTC_REG_DAY 0X07
#define RTC_REG_MTH 0X08
#define RTC_REG_YER 0X09
#define RTC_REG_SEC_AL 0X01
#define RTC_REG_MIN_AL 0X03
#define RTC_REG_HOR_AL 0X05

#define AL_INT_BIT BIT(5)
#define ONE_SEC_ALARM 0XFF
#define RTC_UIP BIT(7)
#define RTC_24  BIT(1)

/**
 * @brief Represents a complete date 
 * 
 */
typedef struct{
	uint32_t sec; ///< seconds 
	uint32_t min; ///< minutes
	uint32_t hor; ///< hours
	uint32_t day; ///< day
	uint32_t mth; ///< month
	uint32_t yer; ///< year
} Date;

/**
 * @brief Represents a hour/min/sec 
 * 
 */
typedef struct{
	uint32_t sec; ///< seconds 
	uint32_t min; ///< minutes
	uint32_t hor; ///< hours
} Mini_Date;

/**
 * @brief Subscribes rtc interrupts in exclusive mode
 * 
 * @param bit_no address of memory to be initialized with the bit number to be set in the mask returned upon an interrupt
 * @return int Return 0 upon success and non-zero otherwise
 */
int rtc_subscribe_int(uint8_t *bit_no);

/**
 * @brief Unsubscribe rtc interrupts
 * 
 * @return int Return 0 upon success and non-zero otherwise
 */
int rtc_unsubscribe_int();

/**
 * @brief prints struct Date on the screen
 * 
 * @param D date that is going to be printed
 */
void print_date(Date D);

/**
 * @brief prints struct Mini_Date on the screen
 * 
 * @param D Mini_Date that is going to be printed
 */
void print_mini_date(Mini_Date D);

/**
 * @brief reads rtc registers to get a date
 * 
 * @return Date date where info will be stored
 */
Date rtc_get_date();

/**
 * @brief reads rtc registers to get a mini-date
 * 
 * @return Mini_Date Mini_date where info will be stored
 */
Mini_Date rtc_get_mini_date();

/**
 * @brief reads a specific rtc register
 * 
 * @param param register that will be read
 * @return uint32_t value of the register
 */
uint32_t rtc_get(uint32_t param);

/**
 * @brief wait until rtc can be read by checking bit UIP from register A
 * 
 */
void wait_valid_rtc();

/**
 * @brief disables rtc updates by setting register B bit 7
 * 
 */
void rtc_disable();

/**
 * @brief enable rtc updates
 * 
 */
void rtc_enable();

/**
 * @brief sets rtc periodic 1 second alarm interrupts and activates rtc interrupts
 * 
 */
void enable_alarm_int_sec();

/**
 * @brief disables rtc periodic 1 second alarm interrupts
 * 
 */
void disable_alarm_int();

/**
 * @brief checks if it is an alarm interrupt clears alarm interrupt bit from register C
 * 
 */
void rtc_ih();
