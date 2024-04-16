/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bsp_oled.h"
#include "ds1302.h"
#include "at24c02.h"
#include "bsp_led_bee.h"
#include "stdio.h"
#include "stdlib.h" 
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern I2C_HandleTypeDef hi2c2;
uint8_t last_alarm_time[8];
char date[20];
char time[20];
double flow_speed = 0;
double flow=0;
double flow_limit=40;
int key1=0;
int key2=0;
int key3=0;
int key4=0;
char home=1;
char csb=0;
float distance;
char tmp[50];
unsigned char a=0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim2);
	AT24_Init(hi2c2);
	ds1302_init(0);
	flow_speed = 3.5;
	IIC_GPIO_Config();
	OLED_Init();
	OLED_Clear();
	
	OLED_ShowCHinese(0,0,0);
	OLED_ShowCHinese(16,0,1);
	OLED_ShowChar(32,0,':', 16);
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%.3fm/s", flow_speed);
	OLED_ShowString(48, 0, tmp, 16);
	
	OLED_ShowCHinese(0,2,0);
	OLED_ShowCHinese(16,2,2);
	OLED_ShowChar(32,2,':', 16);
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%.3fm^3", flow);
	OLED_ShowString(48, 2, tmp, 16);
	
	OLED_ShowCHinese(0,4,3);
	OLED_ShowCHinese(16,4,4);
	OLED_ShowChar(32,4,':', 16);
	memset(tmp, 0, sizeof(tmp));
	sprintf(tmp, "%.0fm^3", flow_limit);
	OLED_ShowString(48, 4, tmp, 16);
	
	OLED_ShowCHinese(0,6,5);
	OLED_ShowCHinese(16,6,6);
	OLED_ShowChar(32,6,':', 16);
	ds1302_read();
	OLED_ShowString(48, 6, date, 8);
	OLED_ShowString(56, 7, time, 8);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		//getDistance();
		flow_speed = distance;
		//
		if(flow >= flow_limit)
		{
			LED_ON();
			Buzzer_ON();
			alarm_time_recording();
		}
		else 
		{
			LED_OFF();
			Buzzer_OFF();
		}

		if(key1){
			OLED_Clear();
			home++;
				if(home ==4)home=1;
			key1=0;
			key2=0;
			key3=0;
			key4=0;
		}
		if(home == 1)
		{
			OLED_ShowCHinese(0,0,0);
			OLED_ShowCHinese(16,0,1);
			OLED_ShowChar(32,0,':', 16);
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%06.3fm/s", flow_speed);
			OLED_ShowString(48, 0, tmp, 16);
			
			OLED_ShowCHinese(0,2,0);
			OLED_ShowCHinese(16,2,2);
			OLED_ShowChar(32,2,':', 16);
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%.3fm^3", flow);
			OLED_ShowString(48, 2, tmp, 16);
			
			OLED_ShowCHinese(0,4,3);
			OLED_ShowCHinese(16,4,4);
			OLED_ShowChar(32,4,':', 16);
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%.0fm^3", flow_limit);
			OLED_ShowString(48, 4, tmp, 16);
			
			OLED_ShowCHinese(0,6,5);
			OLED_ShowCHinese(16,6,6);
			OLED_ShowChar(32,6,':', 16);
			ds1302_read();
			OLED_ShowString(48, 6, date, 8);
			OLED_ShowString(56, 7, time, 8);
			if(key4 && home == 1){
				key4=0;
				flow=0;
		}
		}
		else if(home == 2)
		{
			OLED_ShowCHinese(0,0,3);
			OLED_ShowCHinese(16,0,4);
			OLED_ShowCHinese(32,0,7);
			OLED_ShowCHinese(48,0,8);
			OLED_ShowCHinese(0,2,3);
			OLED_ShowCHinese(16,2,4);
			OLED_ShowChar(32,2,':', 16);
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%.0fm^3", flow_limit);
			OLED_ShowString(48, 2, tmp, 16);
			OLED_ShowString(0,4,"KEY1", 16);
			OLED_ShowCHinese(32,4,9);
			OLED_ShowCHinese(45,4,10);
			OLED_ShowString(0,6,"KEY2++ KEY3--", 16);
			
			
			if(key2 && flow_limit<=999){
				flow_limit++;
				key2 = 0;
			}
			else if(key3 && flow_limit>=5)
			{
				flow_limit--;
				key3=0;
			}
		}
		else if(home == 3)
		{
			display_last_alrm();
		}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
