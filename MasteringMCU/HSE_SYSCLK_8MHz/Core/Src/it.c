/*
 * it.c
 *
 *  Created on: Dec 03, 2021
 *      Author: leona
 */

#include "stm32f4xx_hal.h"
extern UART_HandleTypeDef huart2;

void SysTick_Handler (void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}
