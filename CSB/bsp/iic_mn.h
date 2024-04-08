#ifndef __IIC_MN_H__
#define __IIC_MN_H__
#include "stm32f1xx.h"
#define F1
#define IIC_GPIO_Port       GPIOB
#define SDA_PIN             GPIO_PIN_7
#define SCL_PIN		        	GPIO_PIN_6
#ifdef F4
#define SDA_OUT()				{IIC_GPIO_Port->MODER&=~(3<<(7*2));GPIOB->MODER|=1<<7*2;}
#define SDA_IN()		    {IIC_GPIO_Port->MODER&=~(3<<(7*2));GPIOB->MODER|=0<<7*2;}
#else
#define SDA_OUT()				{IIC_GPIO_Port->CRL&=0x0fffffff;IIC_GPIO_Port->CRL|=3<<28;}
#define SDA_IN()		    {IIC_GPIO_Port->CRL&=0x0fffffff;IIC_GPIO_Port->CRL|=8<<28;}
#endif
#define IIC_SCL(n)          (n?HAL_GPIO_WritePin(IIC_GPIO_Port, SCL_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(IIC_GPIO_Port, SCL_PIN, GPIO_PIN_RESET))
#define IIC_SDA(n)          (n?HAL_GPIO_WritePin(IIC_GPIO_Port, SDA_PIN, GPIO_PIN_SET):HAL_GPIO_WritePin(IIC_GPIO_Port, SDA_PIN, GPIO_PIN_RESET))
#define SDA_Read()          HAL_GPIO_ReadPin(IIC_GPIO_Port, SDA_PIN)
void IIC_GPIO_Config(void);
void IIC_Start(void);
void IIC_Stop(void);
unsigned IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Write_Byte(unsigned char data);
unsigned char IIC_Read_Byte(unsigned char ack);
unsigned char IIC_Write_Len(unsigned char addr, unsigned char reg, unsigned char len,unsigned char *buff);
unsigned char IIC_Read_Len(unsigned char addr, unsigned char reg, unsigned char len,unsigned char *buff);
void ee_w(unsigned char addr ,unsigned data);
unsigned char ee_r(unsigned char addr);
#endif/*__IIC_MN_H__*/
