/*
 * msp.c
 *
 *  Created on: Nov 30, 2021
 *      Author: leona
 */


#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
	/* Inicialização de baixo lvl do processador */

	/* 1. Configurar o agrupamento de prioridades do processador.
	* Path:
	* stm32f4xx_hal_cortex.c -> HAL_NVIC_SetPriorityGrouping()
	* preemption priority: decide qual interrupção deve ser executada no processador (prioridade mais baixa executa primeiro)
	* subpriority: se duas interrupções tiverem a mesma preferencia, o que tiver subpriority maior executa primeiro
	* NVIC_PRIORITYGROUP_4: 4 bits para preemption priority - 0 a 15
	*/
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	/* 2. Ativar as exceções do sistema
	* Path:
	* Bloco informar as esceções: System Control Block (SCB)
	* Registrador para ativar as exceções: System Handler Control and State Register (SHCSR)
	* item 4.3.9 do pdf Cortex-M4 Devices Generic User Guide
	*
	* Exceções habilitadas:
	* MemoryManagement: Falha devido a estar tentando acessar uma região protegida da memória.
	* BusFault: Falha devido a um erro detectado em um barramento da memória. Erro na transação de dados na memória.
	* UsageFault: Falha relacionada a execução do script.
	*/
	SCB -> SHCSR |= 0x7 << 16; //exceções de falha de uso, falha de memória e falha de barramento
	//(*SCB).SHCSR |= 0x7 << 16;

	/* 3. Configurar a prioridade das exceções do sistema.
	* Path:
	* stm32f4xx_hal_cortex.c -> HAL_NVIC_SetPriority()
	*/
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;

	/* Inicialização de baixo lvl do periférico UART2 */

	/* 1. habilitar o clock do periférico USART2 e GPIOA*/
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/* 2. configurar pinos
	 * Path:
	 * consultar datasheet do processador
	 * item Pinouts and pin description
	 * para USAR2: TX -> PA2 e RX -> PA3
	 * consultar stm32f4xx_hal_gpio.h
	 * */
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;	//exemplo uart frame communication no google
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	gpio_uart.Alternate = GPIO_AF7_USART2;	//UART2_TX
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	gpio_uart.Pin = GPIO_PIN_3;	//UART2_RX
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	/* 3. habilitar IRQ e definir prioridade (configuração NVIC)*/
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);

}
