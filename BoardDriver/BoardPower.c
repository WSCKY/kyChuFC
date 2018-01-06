/**
  ******************************************************************************
  * @file    BoardPower.c
  * @author  kyChu
  * @version V0.1
  * @date    04-January-2018
  * @brief   Board Power Control Driver.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "BoardPower.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*
 * @brief  Power Control Pin Initialize.
 * @param  None
 * @retval None
 */
void PowerControlInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	/* -1- Enable GPIO Clock (to be able to program the configuration registers) */
	PWR_CTRL_PORT_CLK_ENABLE();
	/* -2- Configure IO in output push-pull mode to drive power control pin */
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Pin = PWR_CTRL_PORT_PIN;
	HAL_GPIO_Init(PWR_CTRL_PORT, &GPIO_InitStruct);
	/* default: Power off */
	HAL_GPIO_WritePin(PWR_CTRL_PORT, PWR_CTRL_PORT_PIN, GPIO_PIN_RESET);
}

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
