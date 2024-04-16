#include "bsp_led_bee.h"
#include "bsp_oled.h"
#include "stdio.h"
#include "stdlib.h"

uint32_t ultrasonic_ranging(void);
float last_distance;
void LED_ON()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}
void LED_OFF()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
}
void Buzzer_ON()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}
void Buzzer_OFF()
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
}
extern int key1;
extern int key2;
extern int key3;
extern int key4;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	static char flag = 0;
	if(flag == 0){
		last_distance = distance;
		flag = 1;
	}
		if (HAL_GPIO_ReadPin(sr04Echo_Port, sr04Echo_Pin) == SET)
		{
			ultrasonic_ranging();
		}else{
			uint32_t tick = ultrasonic_ranging();
			if (tick > 0)
			{
				distance = (tick * 0.017)*0.4 + last_distance*0.6;
				last_distance = distance;
			}
		}

}
extern uint8_t ds1302_time[8];
extern uint8_t last_alarm_time[8];
void alarm_time_recording()
{
	AT24_Write_Len(0x00, 6, ds1302_time);
}
void last_alarm_time_read()
{
	AT24_Read_Len(0x00, 6, last_alarm_time);
}

void display_last_alrm()
{
	char tmp[20];
	last_alarm_time_read();
	OLED_ShowCHinese(0,0,11);
	OLED_ShowCHinese(16,0,12);
	OLED_ShowCHinese(32,0,13);
	OLED_ShowCHinese(48,0,14);
	OLED_ShowCHinese(64,0,5);
	OLED_ShowCHinese(80,0,6);
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "20%02d-%02d-%02d", last_alarm_time[0], last_alarm_time[1], last_alarm_time[2]);
	OLED_ShowString(16, 4, tmp, 8);
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%02d:%02d:%02d", last_alarm_time[3], last_alarm_time[4], last_alarm_time[5]);
	OLED_ShowString(24, 5, tmp, 8);
}



uint32_t ultrasonic_ranging(void)
{
   uint32_t  tick  = 0 ; 
   static uint8_t initFlag = 0;
	static uint32_t tick_old = 0 ;
	 //  首次调用， 完成初始化
   if(initFlag == 0)
	{
			initFlag = 1;
		  //  DWT 计数初始化
			CoreDebug->DEMCR |= 1<<24 ;  // 使能DWT外设
			DWT->CYCCNT       = 0     ;  // 清零CYCCNT
			DWT->CTRL        |= 1<<0  ;  // 使能计数
	}
	
	if(tick_old < DWT->CYCCNT)
	{
		tick = (0xffffffff - tick_old) + DWT->CYCCNT  + 1 ;
	}
	else
	{
		tick = DWT->CYCCNT - tick_old;
	}	
	tick_old  = DWT->CYCCNT ;	
	
	return tick/CLOCK_FREQUENCY;
}
 
//  获取超声波距离
void getDistance(void)
{
	//  启动超声波测距
	int time = 100;
	HAL_GPIO_WritePin(sr04Trig_Port, sr04Trig_Pin, GPIO_PIN_SET);
	while (time--)
		;
	HAL_GPIO_WritePin(sr04Trig_Port, sr04Trig_Pin, GPIO_PIN_RESET);
}
                        