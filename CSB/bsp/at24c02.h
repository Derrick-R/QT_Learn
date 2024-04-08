#ifndef __AT24C02_H__
#define __AT24C02_H__
 
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_i2c.h"
 
#define AT24_Write 0xA0
#define AT24_Read 0xA1
 
 
uint8_t AT24_Init(I2C_HandleTypeDef);
uint8_t AT24_Write_Byte(uint8_t reg , uint8_t data);
uint8_t AT24_Read_Byte(uint8_t reg);
uint8_t AT24_Write_Len(uint8_t reg,uint8_t len,uint8_t *buf); //iicÁ¬Ðø¶ÁÐ´
uint8_t AT24_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf);
#endif/*__AT24C02_H__*/