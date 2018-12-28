#pragma once
#include "i8042.h"

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

typedef struct{
	uint32_t sec,min,hor,day,mth,yer;
} Date;

typedef struct{
	uint32_t sec,min,hor;
} Mini_Date;

int teste();
int loop();

Date rtc_get_date();
Mini_Date rtc_get_mini_date();
uint32_t rtc_get(uint32_t thingy);
void print_date();
void print_mini_date();
void wait_valid_rtc();
int disable();
int enable();
void enable_alarm_int_sec();
void disable_alarm_int();
void rtc_ih();
int rtc_subscribe_int();
int rtc_unsubscribe_int();
