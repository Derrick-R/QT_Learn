#ifndef __DS1302_H
#define __DS1302_H
#include "stm32f1xx.h"

#define DS1302_CLK_PORT		GPIOA						//端口
#define DS1302_CLK_PIN		GPIO_PIN_10					//引脚
#define DS1302_DAT_PORT		GPIOA						//端口
#define DS1302_DAT_PIN		GPIO_PIN_11					//引脚

#define DS1302_RST_PORT		GPIOA						//端口
#define DS1302_RST_PIN		GPIO_PIN_12					//引脚

#define DS1302_CLK_HIGH		HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_SET)			//引脚输出高电平
#define DS1302_CLK_LOW		HAL_GPIO_WritePin(DS1302_CLK_PORT, DS1302_CLK_PIN, GPIO_PIN_RESET)			//引脚输出低电平

#define DS1302_DAT_HIGH		HAL_GPIO_WritePin(DS1302_DAT_PORT, DS1302_DAT_PIN, GPIO_PIN_SET)			//引脚输出高电平
#define DS1302_DAT_LOW		HAL_GPIO_WritePin(DS1302_DAT_PORT, DS1302_DAT_PIN, GPIO_PIN_RESET)			//引脚输出低电平

#define DS1302_RST_HIGH		HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_SET)			//引脚输出高电平
#define DS1302_RST_LOW		HAL_GPIO_WritePin(DS1302_RST_PORT, DS1302_RST_PIN, GPIO_PIN_RESET)			//引脚输出低电平

#define DS1302_SET_IN		ds1302_set_input_mode()							//设置输入模式
#define DS1302_SET_OUT 		ds1302_set_output_mode()						//设置输出模式		


/*读相关寄存器地址*/
#define DS1302_READ_SECOND 	0X81   	//秒
#define DS1302_READ_MINUTE 	0X83	//分
#define DS1302_READ_HOUR   	0X85	//时
#define DS1302_READ_DAY		0X87	//日
#define DS1302_READ_MONTH 	0X89	//月
#define DS1302_READ_WEEK 	0X8B	//周
#define DS1302_READ_YEAR 	0X8D	//年
//#define DS1302_READ_TIME 	0XBF	//读取全部时间

/*写相关寄存器地址*/
#define DS1302_WRITE_SECOND 0X80   	//秒
#define DS1302_WRITE_MINUTE 0X82	//分
#define DS1302_WRITE_HOUR   0X84	//时
#define DS1302_WRITE_DAY	0X86	//日
#define DS1302_WRITE_MONTH 	0X88	//月
#define DS1302_WRITE_WEEK 	0X8A	//周
#define DS1302_WRITE_YEAR 	0X8C	//年
#define DS1302_WRITE_PROTECT 0X8E	//保护
//#define DS1302_WRITE_TIME 	0XBE 	//写全部时间


extern uint8_t ds1302_time[8];	//存放日期和时间

void ds1302_write_byte(uint8_t addr_or_data);			//DS1302 写一字节 函数
void ds1302_write_cmd(uint8_t addr,uint8_t dat);		//DS1302 写命令	  函数
uint8_t ds1302_read_byte(void);							//DS1302 读一字节 函数
uint8_t ds1302_read_data(uint8_t addr);					//DS1302 写一字节 函数
void bcd_to_dec(uint8_t *bcd,uint8_t times);			//BCD 转 十进制 函数
void dec_to_bcd(uint8_t *dec,uint8_t times);			//十进制 转 BCD 函数
void ds1302_init(char time_set);									//DS1302 初始化日期和时间 函数
void ds1302_read(void);														//DS1302 读取  日期和时间 函数

#endif

