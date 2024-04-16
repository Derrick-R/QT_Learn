#include "ds1302.h"

//日期和 时间 定义 		  年 月 日 时 分 秒 周
uint8_t ds1302_time[8] = {24,4,9,10,01,00,0X02};

/******************************************************************
描述: DS1302 DAT设置成输入模式 函数
*******************************************************************/
void ds1302_set_input_mode(void)
{
	GPIO_InitTypeDef DS1302_Struction;				//定义结构体成员
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	DS1302_Struction.Pin  = DS1302_DAT_PIN;		//引脚
	DS1302_Struction.Mode = GPIO_MODE_INPUT;			//上拉输入
	DS1302_Struction.Pull = GPIO_PULLUP;
	
	HAL_GPIO_Init(DS1302_DAT_PORT,&DS1302_Struction);		//对成员进行初始化	
}


/******************************************************************
描述: DS1302 DAT设置成输出模式 函数
*******************************************************************/
void ds1302_set_output_mode(void)
{
	GPIO_InitTypeDef DS1302_Struction;				//定义结构体成员
	__HAL_RCC_GPIOA_CLK_ENABLE();	//打开时钟
	
	DS1302_Struction.Pin  = DS1302_DAT_PIN;		//引脚
	DS1302_Struction.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出
	DS1302_Struction.Speed = GPIO_SPEED_HIGH;		//速率	

	HAL_GPIO_Init(DS1302_DAT_PORT,&DS1302_Struction);		//对成员进行初始化	
}


/******************************************************************
描述: DS1302 配置 函数
*******************************************************************/
void ds1302_config(void)
{
	GPIO_InitTypeDef DS1302_Struction;				//定义结构体成员

	__HAL_RCC_GPIOA_CLK_ENABLE();	//打开时钟

	//DS1302 CLK
	DS1302_Struction.Pin  = DS1302_CLK_PIN;		//引脚
	DS1302_Struction.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出	
	DS1302_Struction.Speed = GPIO_SPEED_HIGH; 	//50MHz
	HAL_GPIO_Init(DS1302_CLK_PORT,&DS1302_Struction);		//对成员进行初始化		
	//DS1302 DAT
	DS1302_Struction.Pin  = DS1302_DAT_PIN;		//引脚
	DS1302_Struction.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出	
	DS1302_Struction.Speed = GPIO_SPEED_HIGH; 	//50MHz
	HAL_GPIO_Init(DS1302_DAT_PORT,&DS1302_Struction);		//对成员进行初始化	
	//DS1302 RST
	DS1302_Struction.Pin  = DS1302_RST_PIN;		//引脚
	DS1302_Struction.Mode = GPIO_MODE_OUTPUT_PP;		//推挽输出	
	DS1302_Struction.Speed = GPIO_SPEED_HIGH; 	//50MHz
	HAL_GPIO_Init(DS1302_RST_PORT,&DS1302_Struction);		//对成员进行初始化	

}
/******************************************************************
描述: DS1302 写一字节 函数
*******************************************************************/
void ds1302_write_byte(uint8_t addr_or_data)
{
	uint8_t i;
	DS1302_SET_OUT;
	for(i=0;i<8;i++)
	{
		if(addr_or_data & 0x01)		//从低位开始传输
		{
			DS1302_DAT_HIGH;
		}
		else
		{
			DS1302_DAT_LOW;
		}
		addr_or_data >>= 1;		//右移一位
		DS1302_CLK_HIGH;		//拉高时钟表示已经发送
		DS1302_CLK_LOW;			//拉低时钟准备继续放新的数据位
	}
}

/******************************************************************
描述: DS1302 写命令 函数
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
描述: DS1302 读一字节 函数
*******************************************************************/
uint8_t ds1302_read_byte(void)
{
	uint8_t i;
	uint8_t dat = 0;
	DS1302_SET_IN;
	for(i=0;i<8;i++)
	{
		dat >>= 1;		//左移一次 低位开始 共有效左移7次
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
描述: DS1302 读数据 函数
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
描述: BCD 转 十进制 函数 例如 0X24 转成 24
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
描述: 十进制 转 BCD 函数 例如 12 转成 0X12
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
描述: DS1302 初始化 日期和时间 函数
*******************************************************************/
void ds1302_init(char time_set)
{
	ds1302_config();	//引脚配置
	if(time_set){
		dec_to_bcd(ds1302_time,7);	//十进制转BCD码
		ds1302_write_cmd(DS1302_WRITE_PROTECT,0x00); //关闭写保护	
		
		ds1302_write_cmd(DS1302_WRITE_YEAR,	ds1302_time[0]);	//年
		ds1302_write_cmd(DS1302_WRITE_MONTH,ds1302_time[1]);	//月
		ds1302_write_cmd(DS1302_WRITE_DAY,	ds1302_time[2]);  	//日
		ds1302_write_cmd(DS1302_WRITE_HOUR,	ds1302_time[3]);   	//时
		ds1302_write_cmd(DS1302_WRITE_MINUTE,ds1302_time[4]); 	//分
		ds1302_write_cmd(DS1302_WRITE_SECOND,ds1302_time[5]);  	//秒
		ds1302_write_cmd(DS1302_WRITE_WEEK,	ds1302_time[6]);  	//周	

		ds1302_write_cmd(DS1302_WRITE_PROTECT,0x80); //开启写保护	
	}
}

/******************************************************************
描述: DS1302 读取 日期和时间 函数
*******************************************************************/
void ds1302_read(void)
{
	ds1302_time[0] = ds1302_read_data(DS1302_READ_YEAR);  //读取年
	ds1302_time[1] = ds1302_read_data(DS1302_READ_MONTH); //读取月
	ds1302_time[2] = ds1302_read_data(DS1302_READ_DAY);   //读取日
	ds1302_time[3] = ds1302_read_data(DS1302_READ_HOUR);  //读取时
	ds1302_time[4] = ds1302_read_data(DS1302_READ_MINUTE);//读取分
	ds1302_time[5] = ds1302_read_data(DS1302_READ_SECOND);//读取秒
	ds1302_time[6] = ds1302_read_data(DS1302_READ_WEEK);  //读取周

	bcd_to_dec(ds1302_time,7);	//BCD码转成十进制
}


