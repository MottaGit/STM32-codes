/*
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

/**
 * ADC é configurado para ser disparado pelo evento de TIM3
 * TIM3 é configurado para gerar um evento a cada update
 * Interrupcao de TIM3 realiza o debounce do botao
 * TIM10 é configurado para gerar interrupcao periodica
 * Interrupcao aciona o led e dispara envio do valor convertido pela serial

 * @file main.c
 * @author Marcos Zuccolotto
 * @date april, 2017
 * @brief ADC disparado pelo TIM3 e debouce de botao
  */

// Comentarios para doxygen e Graphviz
// O comando abaixo gera o gráfico na pagina principal do documento
// É necessario usar a ferramenta Grapviz para interpretar a linguagem DOT
/*! \mainpage
 ADC disparado pelo evento de update do TIM3 \n
 Debounce de botao usando interrupcao periodica\n
 Configuracao de hardware utilizada :
  \dot
  digraph hardconf  {
      node [shape=box, fontname=Helvetica, fontsize=11];
      a [ label="TIM3" ];
      b [ label="ADC1" ];
      a -> b [label="UE", fontname=Helvetica, fontsize=10];
       {rank=same; a b}
  }
  \enddot

 */


//#define MODO_TESTE 1

//! Tempo de espera pela conversao ad
#define ADC_TIMEOUT 100
//! tempo de espera da serial
#define SERIAL_TIMEOUT 100
//! Tamanho do buffer de transmissao serial
#define MSG_LENGHT 100


// Debouce botao
//! Limite superior para detecção botao pressionado
#define BT_THRESH 10
//! Limite inferior para detecção botao pressionado
#define BT_THRESL 5
//! Valor maximo da variavel de debouce
#define BT_MAX 15
//! Valor minimo da variavel de debouce
#define BT_MIN 0


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//! Buffer transmissao serial
int8_t msg[MSG_LENGHT];
//! Valor de conversao AD
int32_t adcvalue;

//! Flag de conversao completa
volatile uint8_t adDone=0;
//! Hora de transmitir
volatile uint8_t serialGo=0;
//! Hora de piscar
volatile uint8_t pisca=0;

// Debounce botao


//! variavel de debounce do botao1
static int8_t dbt1=BT_MIN;
//! estado botao 1
static uint8_t bt1,
//! valor anterior do botao 1
		bt1old,
//! borda botao 1
		bt1up;
//! auxiliar valor botao entrada
GPIO_PinState btaux;




/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_TIM10_Init();

  /* USER CODE BEGIN 2 */
  // Inicializa variaveis globais
   dbt1 = BT_MIN;
  bt1=bt1old=bt1up=0;
// Dispara perifericos
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Start_IT(&htim10);
  HAL_ADC_Start_IT(&hadc1);

// envia cabeçalho tela
   sprintf(msg,"\aFundacao Liberato - EleTROnica\r\nprof. Marcos Zuccolotto\r\nADC com disparo pelo TIM3 \n\r");
   HAL_UART_Transmit(&huart2, msg, strlen(msg),SERIAL_TIMEOUT);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
	  // Se é hora, envia mensagem pela serial
	  if (serialGo)
		  	  {
		  		  serialGo=0;
		  		   sprintf(msg,"Valor ADC : %04ld \r",adcvalue);
		  		   HAL_UART_Transmit(&huart2, msg, strlen(msg),SERIAL_TIMEOUT);
			  }
	  // Se botao foi pressionado muda estado controle pisca
	  if (bt1up)
	  {
		  bt1up=0;
		  pisca=!pisca;
	  }
// Esta parte foi feita em aula, transmite valor convertido logo apos conversao
//	  if (adDone==1)
//	  {
//		  adDone=0;
//		   sprintf(msg,"Valor ADC : %d \n\r",adcvalue);
//		   HAL_UART_Transmit(&huart2, msg, strlen(msg),SERIAL_TIMEOUT);
//	  }
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
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

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */
/**
 * @brief Interrupcao periodica TIM3 (debounce botao) e TIM10 (PiscaLed)
 * @param Handler timer
 * @retval nenhum
 *
 *
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance==TIM3)
		{
		// Rotina de "debouce" do botao 1
		// Le botao e atualiza variavel de debouce
		btaux=HAL_GPIO_ReadPin(B1_GPIO_Port,B1_Pin);
		if (btaux==GPIO_PIN_RESET)
			{dbt1++;}
		else
			{dbt1--;}
		// colocando limites no tempo
		if (dbt1>BT_MAX)
				{dbt1=BT_MAX;}
		if (dbt1<BT_MIN)
				{dbt1=BT_MIN;}
		// definindo estado do botao
		if (dbt1>BT_THRESH)
				{bt1=1;}
		if (dbt1<BT_THRESL)
				{bt1=0;}
		// Detetando bordas
		if ((bt1old==0) && (bt1==1))
				{bt1up=1;}
		// atualizando
		bt1old=bt1;
		}
	if (htim->Instance==TIM10)
		{
		// Pisca led
			if (pisca)
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		// manda valor ADC pela serial
			serialGo=1;
		}
	}


/**
 * @brief Interrupcao conversao AD completa
 * @param Handler adc gerou interrupcao
 * @retval nenhum
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
#ifdef MODO_TESTE  // compilacao condicional para ser utilizada durante teste do codigo
	HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
#endif
	adcvalue=HAL_ADC_GetValue(hadc);
	adDone=1;

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
