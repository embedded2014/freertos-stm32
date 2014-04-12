/**
  ******************************************************************************
  * @file    Template/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2013
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
/** @addtogroup Template
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

void RCC_Configuration(void)
{
      /* --------------------------- System Clocks Configuration -----------------*/
      /* USART2 clock enable */
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
      /* GPIOA clock enable */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
}
 
/**************************************************************************************/
 
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*-------------------------- GPIO Configuration ----------------------------*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Connect USART pins to AF */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART2);   // USART2_TX
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART2);  // USART2_RX
}
 
/**************************************************************************************/
 
void USART2_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;

    /* USARTx configuration ------------------------------------------------------*/
    /* USARTx configured as follow:
     *  - BaudRate = 9600 baud
     *  - Word Length = 8 Bits
     *  - One Stop Bit
     *  - No parity
     *  - Hardware flow control disabled (RTS and CTS signals)
     *  - Receive and transmit enabled
     */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);
}

void USART2_puts(char* s)
{
    while(*s) {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, *s);
        s++;
    }
}

/**************************************************************************************/
/*int main(void)*/
/*{*/
	/*RCC_Configuration();*/
	/*GPIO_Configuration();*/
	/*USART2_Configuration();*/

	/*USART2_puts("Hello World!\r\n");*/
	/*USART2_puts("Just for STM32F429I Discovery verify USART2 with USB TTL Cable\r\n");*/
	/*while(1)*/
	/*{*/
		/*while(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);*/
		/*char t = USART_ReceiveData(USART2);*/
		/*if ((t == '\r')) {*/
			/*while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);*/
			/*USART_SendData(USART2, t);*/
			/*t = '\n';*/
		/*}*/
		/*while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);*/
		/*USART_SendData(USART2, t);*/
	/*}*/

	/*while(1); // Don't want to exit*/
/*}*/

void busyloop( uint32_t delay )
{
	while( delay )	delay--;
}

const TickType_t xDelay = 100 / portTICK_PERIOD_MS;

static void UsartTask(void *pvParameters)
{
	STM_EVAL_LEDInit( LED4 );

	for( ;; )
	{
		vTaskDelay( xDelay );
		STM_EVAL_LEDToggle( LED4 );
	}
}

static void LEDTask( void *pvParameters)
{
	STM_EVAL_LEDInit( LED3 );

	for(;;){
			STM_EVAL_LEDToggle(LED3);
			vTaskDelay( xDelay );
	}
}

static void ButtonTask( void *pvParameters )
{
	STM_EVAL_PBInit( BUTTON_USER, BUTTON_MODE_GPIO );
	STM_EVAL_LEDInit( LED3 );
	
	while(1) {
		if( STM_EVAL_PBGetState( BUTTON_USER ) ){
			STM_EVAL_LEDOn( LED3 );
			while( STM_EVAL_PBGetState( BUTTON_USER ) );
			STM_EVAL_LEDOff( LED3 );
		}
	}
}

//Main Function
int main(void)
{
	//Create Task For USART
	xTaskCreate(UsartTask, (signed char*)"UsartTask", 128, NULL, tskIDLE_PRIORITY+1, NULL);
	/*xTaskCreate(LEDTask, (signed char*)"LEDTask", 128, NULL, tskIDLE_PRIORITY+1, NULL);*/
	xTaskCreate(ButtonTask, (signed char*)"ButtonTask", 128, NULL, tskIDLE_PRIORITY+1, NULL);

	//Call Scheduler
	vTaskStartScheduler();
}
