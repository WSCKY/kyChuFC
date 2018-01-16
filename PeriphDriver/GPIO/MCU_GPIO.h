/**
  ******************************************************************************
  * @file    MCU_GPIO.h
  * @author  kyChu
  * @version V0.1
  * @date    13-January-2018
  * @brief   Header file for MCU GPIO Driver.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_GPIO_H
#define __MCU_GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "../PeriphDefine.h"

/* Exported types ------------------------------------------------------------*/
typedef struct {
	GPIO_TypeDef *GPIO_PORT;
	uint32_t GPIO_PIN;
	uint32_t GPIO_MODE;
	uint32_t GPIO_PULL;
	uint32_t GPIO_SPEED;
	GPIO_PinState PinState;
} GPIO_InitDataType;
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported definitions ------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
#define GPIO_INIT_QTY                            (7)

#define GPIO_PORT_INIT_TABLE                     {{GPIOB, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_NOPULL, GPIO_SPEED_HIGH, GPIO_PIN_RESET}, \
	                                              {GPIOB, GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_PIN_RESET}, \
                                                  {GPIOB, GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_HIGH, GPIO_PIN_RESET}, \
                                                  {GPIOD, GPIO_PIN_14, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_PIN_RESET}, \
	                                              {GPIOD, GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_PIN_RESET}, \
												  {GPIOE, GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_HIGH, GPIO_PIN_RESET}, \
	                                              {GPIOE, GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_PIN_RESET}}

#define GPIO_PORT_CLK_ENABLE_ALL()               {__HAL_RCC_GPIOA_CLK_ENABLE(); \
	                                              __HAL_RCC_GPIOB_CLK_ENABLE(); \
	                                              __HAL_RCC_GPIOC_CLK_ENABLE(); \
	                                              __HAL_RCC_GPIOD_CLK_ENABLE(); \
	                                              __HAL_RCC_GPIOE_CLK_ENABLE(); \
	                                              __HAL_RCC_GPIOF_CLK_ENABLE(); }

/* Exported functions ------------------------------------------------------- */
HAL_StatusTypeDef MCU_GPIOs_Init(void);
void MCU_GPIO_SetLevelLow(uint8_t n);
void MCU_GPIO_SetLevelHigh(uint8_t n);
void MCU_GPIO_SetLevelToggle(uint8_t n);
GPIO_PinState MCU_GPIO_ReadLevel(uint8_t n);

#endif /* __MCU_GPIO_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
