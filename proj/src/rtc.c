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
    disable();
    sys_outb(RTC_ADDR_REG,RTC_REG_SEC); sys_inb(RTC_DATA_REG,&segundo);
    sys_outb(RTC_ADDR_REG,RTC_REG_MIN); sys_inb(RTC_DATA_REG,&minuto);
    sys_outb(RTC_ADDR_REG,RTC_REG_HOR); sys_inb(RTC_DATA_REG,&hora);
    sys_outb(RTC_ADDR_REG,RTC_REG_DAY); sys_inb(RTC_DATA_REG,&dia);
    sys_outb(RTC_ADDR_REG,RTC_REG_MTH); sys_inb(RTC_DATA_REG,&mes);
	sys_outb(RTC_ADDR_REG,RTC_REG_YER); sys_inb(RTC_DATA_REG,&ano);
    enable();
    d.sec=segundo;d.min = minuto;d.hor=hora;d.day=dia;d.mth=mes;d.yer=ano;
	return d;
}

Mini_Date rtc_get_mini_date(){
    uint32_t hora, minuto, segundo;
    Mini_Date d;
    wait_valid_rtc();
    disable();
    sys_outb(RTC_ADDR_REG,RTC_REG_SEC); sys_inb(RTC_DATA_REG,&segundo);
    sys_outb(RTC_ADDR_REG,RTC_REG_MIN); sys_inb(RTC_DATA_REG,&minuto);
    sys_outb(RTC_ADDR_REG,RTC_REG_HOR); sys_inb(RTC_DATA_REG,&hora);
    enable();
    d.sec=segundo;d.min = minuto;d.hor=hora;
	return d;
}

uint32_t rtc_get(uint32_t thingy){
    uint32_t num;
    sys_outb(RTC_ADDR_REG, thingy); 
    sys_inb(RTC_DATA_REG, &num);
    return num;
}

void print_date(Date  d){
    printf("%x / %x /%x    " ,d.yer,d.mth,d.day);
	printf("%x : %x : %x \n" ,d.hor,d.min,d.sec);
}

void print_mini_date(Mini_Date  d){
	printf("%x : %x : %x \n" ,d.hor,d.min,d.sec);
}


void wait_valid_rtc(){
    do{
        disable();
        sys_outb(RTC_ADDR_REG,RTC_REG_A);
        sys_inb(RTC_DATA_REG,&regA);
        enable();
    }while(regA & RTC_UIP);
}


int disable(){
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_inb(RTC_DATA_REG,&regB);
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_outb(RTC_DATA_REG,RTC_UIP | regB);
    return 0;
}

int enable(){    
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_outb(RTC_DATA_REG,regB & ~RTC_UIP | RTC_24 ) ;
    return 0;
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

/*
int loop(int num){
  // Subscribing interrupts
  uint8_t irq_set;
  if (rtc_subscribe_int(&irq_set) == FUNCTION_FAIL) {
    printf("Function fail: rtc_subscribe_int() \n");
    return 1;
  }
   enable_alarm_int_sec();
  // Interrupt loop
  int ipc_status; message msg;
  while(num>0) { 
    int r;
    // Get a request message. 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { // received notification 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // hardware interrupt notification                             
          if (msg.m_notify.interrupts & irq_set) { // subscribed interrupt
              rtc_ih();
              if(alarm_int){
                num --;
                Mini_Date d = rtc_get_mini_date();
                print_mini_date(d);
                }
            }
          break;
          default:
          break; // no other notifications expected: do nothing     
          }    
      }
    }
    disable_alarm_int();

  if (rtc_unsubscribe_int() == FUNCTION_FAIL) {
    printf("Function fail: rtc_unsubscribe_int() \n");
    return 1;
    }
  printf("Function ended successfully. \n");
  return 0;
}

int teste(){
    loop(5);
    Mini_Date d;
    for (int i =0;i< 4 ; i++){
            d= rtc_get_mini_date();
            print_mini_date(d);
        }
    return 0;
}

*/
