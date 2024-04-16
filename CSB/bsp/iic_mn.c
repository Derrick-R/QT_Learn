#include "iic_mn.h"

void IIC_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_Init;
	
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_Init.Mode=GPIO_MODE_OUTPUT_PP;
	GPIO_Init.Pin =SDA_PIN|SCL_PIN;
	GPIO_Init.Speed=GPIO_SPEED_FREQ_HIGH;
	GPIO_Init.Pull =GPIO_PULLUP;
	HAL_GPIO_WritePin(IIC_GPIO_Port, SDA_PIN|SCL_PIN, GPIO_PIN_SET);
	HAL_GPIO_Init(IIC_GPIO_Port, &GPIO_Init);
	
}
void delay()
{
	int i=0;
	while(i--)
	{
		
	}
}
void IIC_Start(void)
{
	SDA_OUT();
	IIC_SDA(1);
	IIC_SCL(1);
	delay();
	IIC_SDA(0);
	delay();
	IIC_SCL(0);
}

void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL(0);
	IIC_SDA(0);
	delay();
	IIC_SCL(1);
	IIC_SDA(1);
	delay();
}
unsigned IIC_Wait_Ack(void)
{
	unsigned char t=0;
	SDA_OUT();
	IIC_SDA(1);
	SDA_IN();
	//IIC_SDA(1);
	delay();
	IIC_SCL(1);
	delay();
	while(SDA_Read())
	{
		t++;
		if(t>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);
	return 0;
}
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	delay();
	IIC_SCL(1);
	delay();
	IIC_SCL(0);
}

void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	delay();
	IIC_SCL(1);
	delay();
	IIC_SCL(0);
}

void IIC_Write_Byte(unsigned char data)
{
	unsigned char i;
	SDA_OUT();
	IIC_SCL(0);
	for(i=0;i<8;i++)
	{
		IIC_SDA((data&0x80)>>7);
		data<<=1;
		delay();
		IIC_SCL(1);
		delay();
		IIC_SCL(0);
		delay();
	}
}
//��һ���ֽڣ�ACK=1ʱ����ACK
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		IIC_SCL(0);
		delay();
		IIC_SCL(1);
		receive<<=1;
		if(SDA_Read())
			receive++;
		delay();
	}
	if(!ack)
		IIC_NAck();
	else
		IIC_Ack();
	return receive;
}

unsigned char IIC_Write_Len(unsigned char addr, unsigned char reg, unsigned char len,unsigned char *buff)
{
	unsigned char i;
	IIC_Start();
	IIC_Write_Byte((addr<<1)|0);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	IIC_Write_Byte(reg);
	IIC_Wait_Ack();
	for(i=0;i<len;i++)
	{
		IIC_Write_Byte(buff[i]);
		if(IIC_Wait_Ack())
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_Stop();
	return 0;
}

unsigned char IIC_Read_Len(unsigned char addr, unsigned char reg, unsigned char len,unsigned char *buff)
{
	IIC_Start();
	IIC_Write_Byte((addr<<1)|0);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}
	IIC_Write_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Write_Byte((addr<<1)|1);
	IIC_Wait_Ack();
	while(len)
	{
		if(len==1)
			*buff=IIC_Read_Byte(0);
		else
			*buff=IIC_Read_Byte(1);
		len--;
		buff++;
	}
	IIC_Stop();
	return 0;
}
void ee_w(unsigned char addr ,unsigned data)
{
	IIC_GPIO_Config();
	IIC_Start();
	IIC_Write_Byte(0xa0);
	IIC_Wait_Ack();
	IIC_Write_Byte(addr);
	IIC_Wait_Ack();
	IIC_Write_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
	HAL_Delay(30);
}

unsigned char ee_r(unsigned char addr)
{
	unsigned char recieve;
	IIC_GPIO_Config();
	IIC_Start();
	IIC_Write_Byte(0xa0);
	IIC_Wait_Ack();
	IIC_Write_Byte(addr);
	IIC_Wait_Ack();
	
	IIC_Start();
	IIC_Write_Byte(0xa1);
	IIC_Wait_Ack();
	recieve=IIC_Read_Byte(0);
	IIC_Wait_Ack();
	IIC_Stop();
	HAL_Delay(30);
	return recieve;
}
