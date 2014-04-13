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
#include "game.h"

#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/** @addtogroup Template
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


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
	STM_EVAL_LEDInit( LED3 );
	
	while(1) {
		if( STM_EVAL_PBGetState( BUTTON_USER ) ){
			STM_EVAL_LEDOn( LED3 );
			while( STM_EVAL_PBGetState( BUTTON_USER ) );
			STM_EVAL_LEDOff( LED3 );
		}
	}
}

static void LCDTask( void *pvParameters )
{
	LCD_Init();
	LCD_LayerInit();
	IOE_Config();
	LTDC_Cmd( ENABLE );
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LCD_Clear(LCD_COLOR_RED);
	LCD_DrawLine( 0x50, 0x50, 0x50, 0x0000 );
	LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)"Hello World");

	while(1){

		if( IOE_TP_GetState()->TouchDetected ){
			uint8_t* tickPerMs =  portTICK_PERIOD_MS;
			LCD_Clear(LCD_COLOR_BLUE);
			LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)tickPerMs);
			while( IOE_TP_GetState()->TouchDetected );
		}
	}
}

void
prvInit()
{
	//LCD init
	LCD_Init();
	IOE_Config();
	LTDC_Cmd( ENABLE );

	LCD_LayerInit();
	LCD_SetLayer( LCD_FOREGROUND_LAYER );
	LCD_Clear( LCD_COLOR_BLACK );
	LCD_SetTextColor( LCD_COLOR_WHITE );

	//Button
	STM_EVAL_PBInit( BUTTON_USER, BUTTON_MODE_GPIO );

	//LED
	STM_EVAL_LEDInit( LED3 );
}

static void GameEventTask1( void *pvParameters )
{
	while( 1 ){
		GAME_EventHandler1();
	}
}

static void GameEventTask2( void *pvParameters )
{
	while( 1 ){
		GAME_EventHandler2();
	}
}

static void GameEventTask3( void *pvParameters )
{
	while( 1 ){
		GAME_EventHandler3();
	}
}

static void GameTask( void *pvParameters )
{
	while( 1 ){
		GAME_Update();
		GAME_Render();
		vTaskDelay( 10 );
	}
}

//Main Function
int main(void)
{
	prvInit();

	xTaskCreate( GameTask, (signed char*) "GameTask", 128, NULL, tskIDLE_PRIORITY + 1, NULL );
	xTaskCreate( GameEventTask1, (signed char*) "GameEventTask1", 128, NULL, tskIDLE_PRIORITY + 1, NULL );
	xTaskCreate( GameEventTask2, (signed char*) "GameEventTask2", 128, NULL, tskIDLE_PRIORITY + 1, NULL );
	xTaskCreate( GameEventTask3, (signed char*) "GameEventTask3", 128, NULL, tskIDLE_PRIORITY + 1, NULL );

	//Call Scheduler
	vTaskStartScheduler();
}
