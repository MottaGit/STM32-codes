/*
 * main.c
 *
 *  Created on: Dec 7, 2021
 *      Author: leona
 */

#include <string.h>
#include "stm32f4xx_hal.h"

void SystemClockConfig(void);
void UART2_Init(void);
void Error(void);

UART_HandleTypeDef huart2;

char *pData = "mensagem teste aula firmware epower\r\n";

int main(void)
{
	HAL_Init();
	SystemClockConfig();

	UART2_Init();

	uint16_t Size = strlen(pData);
	HAL_UART_Transmit(&huart2, (uint8_t*)pData, Size, HAL_TIMEOUT);



	return 0;
}


void SystemClockConfig(void)
{
	//função especial clock
}

void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		Error();
	}
}


void Error(void)
{
	while(1);
}
