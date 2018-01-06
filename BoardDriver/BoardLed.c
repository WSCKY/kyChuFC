/**
  ******************************************************************************
  * @file    BoardLed.c
  * @author  kyChu
  * @version V0.1
  * @date    04-January-2018
  * @brief   Board LED Driver.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BoardLed.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
 * @brief  Led Control Pin Initialize.
 * @param  None
 * @retval None
 */
void BoardLedInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	/* -1- Enable GPIO Clock (to be able to program the configuration registers) */
	LED_RED_PORT_CLK_ENABLE();
	LED_BLUE_PORT_CLK_ENABLE();
	LED_GREEN_PORT_CLK_ENABLE();
	/* -2- Configure IO in output push-pull mode to drive external led. */
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	GPIO_InitStruct.Pin = LED_RED_PORT_PIN;
	HAL_GPIO_Init(LED_RED_PORT, &GPIO_InitStruct); /* RED */
	GPIO_InitStruct.Pin = LED_BLUE_PORT_PIN;
	HAL_GPIO_Init(LED_BLUE_PORT, &GPIO_InitStruct); /* BLUE */
	GPIO_InitStruct.Pin = LED_GREEN_PORT_PIN;
	HAL_GPIO_Init(LED_GREEN_PORT, &GPIO_InitStruct); /* GREEN */
	/* default: all led off */
	HAL_GPIO_WritePin(LED_RED_PORT, LED_RED_PORT_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_BLUE_PORT, LED_BLUE_PORT_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_GREEN_PORT, LED_GREEN_PORT_PIN, GPIO_PIN_RESET);
}

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
