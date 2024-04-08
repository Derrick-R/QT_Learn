#include "at24c02.h"
 
I2C_HandleTypeDef AT24_IIC;
 
uint8_t AT24_Init(I2C_HandleTypeDef hi2c){
  AT24_IIC = hi2c;
  uint8_t R_Data;
  return (HAL_I2C_Mem_Read(&AT24_IIC, AT24_Read, 0x00, I2C_MEMADD_SIZE_8BIT, &R_Data, 1, 0xff) != HAL_OK);
}
 
 
uint8_t AT24_Read_Byte(uint8_t reg){
  unsigned char R_Data=0;
 
    HAL_I2C_Mem_Read(&AT24_IIC, AT24_Read, reg, I2C_MEMADD_SIZE_8BIT, &R_Data, 1, 0xfff);
    HAL_Delay(20);
 
    return R_Data; 
}
 
uint8_t AT24_Write_Byte(uint8_t reg,uint8_t data)         
{
  unsigned char W_Data=0;
 
  W_Data = data;
  HAL_I2C_Mem_Write(&AT24_IIC, AT24_Write, reg, I2C_MEMADD_SIZE_8BIT, &W_Data, 1, 0xfff);
  HAL_Delay(20);
 
  return 0;
}
//连续写入函数需要根据AT24C02芯片的特性更改
uint8_t AT24_Write_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{
  if(len >8){
    uint8_t count = len/8;
    uint8_t noun = len%8;
    for(int p=0; p<count ; p++){
      HAL_I2C_Mem_Write(&AT24_IIC, AT24_Write, reg+8*p,I2C_MEMADD_SIZE_8BIT,buf+8*p,8,0xff);
			HAL_Delay(5);
    }
    if(noun){
      HAL_I2C_Mem_Write(&AT24_IIC, AT24_Write, reg+8*count, I2C_MEMADD_SIZE_8BIT, buf+8*count, noun, 0xff);
			HAL_Delay(5);
    }
  }
  else{
      HAL_I2C_Mem_Write(&AT24_IIC, AT24_Write, reg ,I2C_MEMADD_SIZE_8BIT, buf,len,0xff);
      HAL_Delay(5);
  }
  return 0;
}
 
uint8_t AT24_Read_Len(uint8_t reg,uint8_t len,uint8_t *buf)
{
  HAL_I2C_Mem_Read(&AT24_IIC, AT24_Read, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 0xfff);
  HAL_Delay(20);
 
  return 0;
}