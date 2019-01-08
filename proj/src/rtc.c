#include <lcom/lcf.h>
#include <stdint.h>
#include "rtc.h"

#define FUNCTION_FAIL   1

static uint32_t regA;
static uint32_t regB;
static uint32_t regC;

Date rtc_get_date(){
	uint32_t ano, mes, dia, hora, minuto, segundo;
    Date d;
    wait_valid_rtc();
    rtc_disable();
    sys_outb(RTC_ADDR_REG,RTC_REG_SEC); sys_inb(RTC_DATA_REG,&segundo);
    sys_outb(RTC_ADDR_REG,RTC_REG_MIN); sys_inb(RTC_DATA_REG,&minuto);
    sys_outb(RTC_ADDR_REG,RTC_REG_HOR); sys_inb(RTC_DATA_REG,&hora);
    sys_outb(RTC_ADDR_REG,RTC_REG_DAY); sys_inb(RTC_DATA_REG,&dia);
    sys_outb(RTC_ADDR_REG,RTC_REG_MTH); sys_inb(RTC_DATA_REG,&mes);
	sys_outb(RTC_ADDR_REG,RTC_REG_YER); sys_inb(RTC_DATA_REG,&ano);
    rtc_enable();
    d.sec=segundo;d.min = minuto;d.hor=hora;d.day=dia;d.mth=mes;d.yer=ano;
	return d;
}

Mini_Date rtc_get_mini_date(){
    uint32_t hora, minuto, segundo;
    Mini_Date d;
    wait_valid_rtc();
    rtc_disable();
    sys_outb(RTC_ADDR_REG,RTC_REG_SEC); sys_inb(RTC_DATA_REG,&segundo);
    sys_outb(RTC_ADDR_REG,RTC_REG_MIN); sys_inb(RTC_DATA_REG,&minuto);
    sys_outb(RTC_ADDR_REG,RTC_REG_HOR); sys_inb(RTC_DATA_REG,&hora);
    rtc_enable();
    d.sec=segundo;d.min = minuto;d.hor=hora;
	return d;
}

uint32_t rtc_get(uint32_t param){
    uint32_t num;
    sys_outb(RTC_ADDR_REG, param); 
    sys_inb(RTC_DATA_REG, &num);
    return num;
}

void print_date(Date  d) {
    printf("%x / %x /%x    " ,d.yer,d.mth,d.day);
	printf("%x : %x : %x \n" ,d.hor,d.min,d.sec);
}

void print_mini_date(Mini_Date  d){
	printf("%x : %x : %x \n" ,d.hor,d.min,d.sec);
}


void wait_valid_rtc(){
    do{
        rtc_disable();
        sys_outb(RTC_ADDR_REG,RTC_REG_A);
        sys_inb(RTC_DATA_REG,&regA);
        rtc_enable();
    }while(regA & RTC_UIP);
}


void rtc_disable(){
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_inb(RTC_DATA_REG,&regB);
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_outb(RTC_DATA_REG,RTC_UIP | regB);
}

void rtc_enable(){    
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_outb(RTC_DATA_REG,regB & ~RTC_UIP | RTC_24 ) ;
}

static int hook_id_rtc;

int rtc_subscribe_int(uint8_t *bit_no) {
    const uint8_t BIT_SET = RTC_IRQ;
    hook_id_rtc = BIT_SET;
    if (sys_irqsetpolicy(RTC_IRQ ,IRQ_REENABLE | IRQ_EXCLUSIVE , &hook_id_rtc) != OK)
        return FUNCTION_FAIL;
    *bit_no = BIT(BIT_SET);
    return 0;
}

int rtc_unsubscribe_int() {    
    if (sys_irqrmpolicy(&hook_id_rtc) != OK)
        return FUNCTION_FAIL;
    return 0;
}


void enable_alarm_int_sec(){
    //set 1 sec interrupts
    sys_outb(RTC_ADDR_REG,RTC_REG_SEC_AL);
    sys_outb(RTC_DATA_REG, ONE_SEC_ALARM);
    sys_outb(RTC_ADDR_REG,RTC_REG_MIN_AL);
    sys_outb(RTC_DATA_REG, ONE_SEC_ALARM);
    sys_outb(RTC_ADDR_REG,RTC_REG_HOR_AL);
    sys_outb(RTC_DATA_REG, ONE_SEC_ALARM);

    //activate interrupts
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_inb(RTC_DATA_REG,&regB);
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_outb(RTC_DATA_REG,regB |  AL_INT_BIT);
}

void disable_alarm_int() {     
    //clear alarm interrupt bit reg C
    sys_outb(RTC_ADDR_REG,RTC_REG_C);
    sys_inb(RTC_DATA_REG,&regC);
    sys_outb(RTC_ADDR_REG,RTC_REG_C);
    sys_outb(RTC_ADDR_REG,regC & ~AL_INT_BIT);

    //disable alarm interrupts
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_outb(RTC_DATA_REG,regB & ~AL_INT_BIT); 
}

bool rtc_alarm_int = false;

void rtc_ih(){
    rtc_alarm_int = false;
    sys_outb(RTC_ADDR_REG,RTC_REG_C); // tirar bit do C 
    sys_inb(RTC_DATA_REG,&regC);
    if(regC & AL_INT_BIT){ 
        rtc_alarm_int = true;
        sys_outb(RTC_ADDR_REG,RTC_REG_C);
        sys_outb(RTC_ADDR_REG,regC & ~AL_INT_BIT);
    }
}
