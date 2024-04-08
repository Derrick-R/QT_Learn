#include "bsp_led_bee.h"

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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
		case GPIO_PIN_12:
			break;
		case GPIO_PIN_13:
			break;
		case GPIO_PIN_14:
			break;
		case GPIO_PIN_15:
			break;
		default:
			break;
	}

}