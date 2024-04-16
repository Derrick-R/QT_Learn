#ifndef __BSP_LED_BEE_H__
#define __BSP_LED_BEE_H__
#include "stm32f1xx.h"
#include "at24c02.h"
#include "ds1302.h"


struct Keys{
	char key_on;
	char key_flag;
};
extern float distance;
#define CLOCK_FREQUENCY	 72    //系统时钟频率宏定义 单位：Mhz
#define sr04Echo_Port  GPIOB
#define sr04Echo_Pin   GPIO_PIN_4
 
//触发信号引脚宏定义
#define sr04Trig_Port  GPIOB
#define sr04Trig_Pin   GPIO_PIN_5
void LED_ON();
void LED_OFF();
void Buzzer_ON();
void Buzzer_OFF();
void alarm_time_recording();
void last_alarm_time_read();
void display_last_alrm();

void getDistance(void);
#endif/*__BSP_LED_BEE_H__*/
