/*
 * main.c
 *
 *  Created on: Nov 30, 2021
 *      Author: leona
 */

#include "stm32f4xx_hal.h"
#include "main.h"

void SystemClockConfig(void);
void UART2_Init(void);
void Error_handler(void);

UART_HandleTypeDef huart2;	/* Identificador do periférico */

int main(void)
{
	HAL_Init();
	SystemClockConfig();

	UART2_Init();

	return 0;
}

void SystemClockConfig(void)
{
	/* usar essa função quando forem necessárias configurações especiais no clock*/

}

void UART2_Init(void) /* Inicialização de alto nível do periférico (parâmetros) */
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	if(HAL_UART_Init(&huart2) != HAL_OK)	/* Habilitar as API's da Hal para a uart2 */
	{
		Error_handler();
	}
}

void Error_handler(void)
{
	while(1);
}
