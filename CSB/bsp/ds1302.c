#include "ds1302.h"

//���ں� ʱ�� ���� 		  �� �� �� ʱ �� �� ��
uint8_t ds1302_time[8] = {24,4,9,10,01,00,0X02};

/******************************************************************
����: DS1302 DAT���ó�����ģʽ ����
*******************************************************************/
void ds1302_set_input_mode(void)
{
	GPIO_InitTypeDef DS1302_Struction;				//����ṹ���Ա
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	DS1302_Struction.Pin  = DS1302_DAT_PIN;		//����
	DS1302_Struction.Mode = GPIO_MODE_INPUT;			//��������
	DS1302_Struction.Pull = GPIO_PULLUP;
	
	HAL_GPIO_Init(DS1302_DAT_PORT,&DS1302_Struction);		//�Գ�Ա���г�ʼ��	
}


/******************************************************************
����: DS1302 DAT���ó����ģʽ ����
*******************************************************************/
void ds1302_set_output_mode(void)
{
	GPIO_InitTypeDef DS1302_Struction;				//����ṹ���Ա
	__HAL_RCC_GPIOA_CLK_ENABLE();	//��ʱ��
	
	DS1302_Struction.Pin  = DS1302_DAT_PIN;		//����
	DS1302_Struction.Mode = GPIO_MODE_OUTPUT_PP;		//�������
	DS1302_Struction.Speed = GPIO_SPEED_HIGH;		//����	

	HAL_GPIO_Init(DS1302_DAT_PORT,&DS1302_Struction);		//�Գ�Ա���г�ʼ��	
}


/******************************************************************
����: DS1302 ���� ����
*******************************************************************/
void ds1302_config(void)
{
	GPIO_InitTypeDef DS1302_Struction;				//����ṹ���Ա

	__HAL_RCC_GPIOA_CLK_ENABLE();	//��ʱ��

	//DS1302 CLK
	DS1302_Struction.Pin  = DS1302_CLK_PIN;		//����
	DS1302_Struction.Mode = GPIO_MODE_OUTPUT_PP;		//�������	
	DS1302_Struction.Speed = GPIO_SPEED_HIGH; 	//50MHz
	HAL_GPIO_Init(DS1302_CLK_PORT,&DS1302_Struction);		//�Գ�Ա���г�ʼ��		
	//DS1302 DAT
	DS1302_Struction.Pin  = DS1302_DAT_PIN;		//����
	DS1302_Struction.Mode = GPIO_MODE_OUTPUT_PP;		//�������	
	DS1302_Struction.Speed = GPIO_SPEED_HIGH; 	//50MHz
	HAL_GPIO_Init(DS1302_DAT_PORT,&DS1302_Struction);		//�Գ�Ա���г�ʼ��	
	//DS1302 RST
	DS1302_Struction.Pin  = DS1302_RST_PIN;		//����
	DS1302_Struction.Mode = GPIO_MODE_OUTPUT_PP;		//�������	
	DS1302_Struction.Speed = GPIO_SPEED_HIGH; 	//50MHz
	HAL_GPIO_Init(DS1302_RST_PORT,&DS1302_Struction);		//�Գ�Ա���г�ʼ��	

}
/******************************************************************
����: DS1302 дһ�ֽ� ����
*******************************************************************/
void ds1302_write_byte(uint8_t addr_or_data)
{
	uint8_t i;
	DS1302_SET_OUT;
	for(i=0;i<8;i++)
	{
		if(addr_or_data & 0x01)		//�ӵ�λ��ʼ����
		{
			DS1302_DAT_HIGH;
		}
		else
		{
			DS1302_DAT_LOW;
		}
		addr_or_data >>= 1;		//����һλ
		DS1302_CLK_HIGH;		//����ʱ�ӱ�ʾ�Ѿ�����
		DS1302_CLK_LOW;			//����ʱ��׼���������µ�����λ
	}
}

/******************************************************************
����: DS1302 д���� ����
*******************************************************************/
void ds1302_write_cmd(uint8_t addr,uint8_t dat)
{
	DS1302_RST_LOW;
	DS1302_CLK_LOW;
	DS1302_RST_HIGH;
	ds1302_write_byte(addr);
	ds1302_write_byte(dat);
	DS1302_RST_LOW;
}

/******************************************************************
����: DS1302 ��һ�ֽ� ����
*******************************************************************/
uint8_t ds1302_read_byte(void)
{
	uint8_t i;
	uint8_t dat = 0;
	DS1302_SET_IN;
	for(i=0;i<8;i++)
	{
		dat >>= 1;		//����һ�� ��λ��ʼ ����Ч����7��
		if( HAL_GPIO_ReadPin(DS1302_DAT_PORT, DS1302_DAT_PIN) == GPIO_PIN_SET)
		{
			dat = dat | 0x80;
		}
//		else
//		{
//			dat = dat & 0x7f;
//		}
		DS1302_CLK_HIGH;
		DS1302_CLK_LOW;
	}
	return dat;
}


/******************************************************************
����: DS1302 ������ ����
*******************************************************************/
uint8_t ds1302_read_data(uint8_t addr)
{
	uint8_t dat = 0;
	DS1302_RST_LOW;
	DS1302_CLK_LOW;
	DS1302_RST_HIGH;
	ds1302_write_byte(addr);
	dat = ds1302_read_byte();
	DS1302_RST_LOW;
	return dat;
}

/******************************************************************
����: BCD ת ʮ���� ���� ���� 0X24 ת�� 24
*******************************************************************/
void bcd_to_dec(uint8_t *bcd,uint8_t times)
{
	uint8_t i;
	for(i=0;i<times;i++)
	{
		bcd[i] = ((bcd[i]>>4)*10) + (bcd[i]&0x0f);
	}
}

/******************************************************************
����: ʮ���� ת BCD ���� ���� 12 ת�� 0X12
*******************************************************************/
void dec_to_bcd(uint8_t *dec,uint8_t times)
{
	uint8_t i;
	for(i=0;i<times;i++)
	{
		dec[i] = ((dec[i]/10)<<4) | (dec[i]%10);
	}
}




/******************************************************************
����: DS1302 ��ʼ�� ���ں�ʱ�� ����
*******************************************************************/
void ds1302_init(char time_set)
{
	ds1302_config();	//��������
	if(time_set){
		dec_to_bcd(ds1302_time,7);	//ʮ����תBCD��
		ds1302_write_cmd(DS1302_WRITE_PROTECT,0x00); //�ر�д����	
		
		ds1302_write_cmd(DS1302_WRITE_YEAR,	ds1302_time[0]);	//��
		ds1302_write_cmd(DS1302_WRITE_MONTH,ds1302_time[1]);	//��
		ds1302_write_cmd(DS1302_WRITE_DAY,	ds1302_time[2]);  	//��
		ds1302_write_cmd(DS1302_WRITE_HOUR,	ds1302_time[3]);   	//ʱ
		ds1302_write_cmd(DS1302_WRITE_MINUTE,ds1302_time[4]); 	//��
		ds1302_write_cmd(DS1302_WRITE_SECOND,ds1302_time[5]);  	//��
		ds1302_write_cmd(DS1302_WRITE_WEEK,	ds1302_time[6]);  	//��	

		ds1302_write_cmd(DS1302_WRITE_PROTECT,0x80); //����д����	
	}
}

/******************************************************************
����: DS1302 ��ȡ ���ں�ʱ�� ����
*******************************************************************/
void ds1302_read(void)
{
	ds1302_time[0] = ds1302_read_data(DS1302_READ_YEAR);  //��ȡ��
	ds1302_time[1] = ds1302_read_data(DS1302_READ_MONTH); //��ȡ��
	ds1302_time[2] = ds1302_read_data(DS1302_READ_DAY);   //��ȡ��
	ds1302_time[3] = ds1302_read_data(DS1302_READ_HOUR);  //��ȡʱ
	ds1302_time[4] = ds1302_read_data(DS1302_READ_MINUTE);//��ȡ��
	ds1302_time[5] = ds1302_read_data(DS1302_READ_SECOND);//��ȡ��
	ds1302_time[6] = ds1302_read_data(DS1302_READ_WEEK);  //��ȡ��

	bcd_to_dec(ds1302_time,7);	//BCD��ת��ʮ����
}


