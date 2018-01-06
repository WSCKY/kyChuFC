/**
  ******************************************************************************
  * @file    BoardLed.h
  * @author  kyChu
  * @version V0.1
  * @date    04-January-2018
  * @brief   Header file for Board Led Control Driver.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARDLED_H
#define __BOARDLED_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"

/* Macros --------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported definitions ------------------------------------------------------*/
#define LED_RED_PORT                             GPIOE
#define LED_RED_PORT_PIN                         GPIO_PIN_15

#define LED_BLUE_PORT                            GPIOD
#define LED_BLUE_PORT_PIN                        GPIO_PIN_15

#define LED_GREEN_PORT                           GPIOD
#define LED_GREEN_PORT_PIN                       GPIO_PIN_14

#define LED_RED_PORT_CLK_ENABLE()                __HAL_RCC_GPIOE_CLK_ENABLE();
#define LED_RED_PORT_CLK_DISABLE()               __HAL_RCC_GPIOE_CLK_DISABLE();

#define LED_BLUE_PORT_CLK_ENABLE()               __HAL_RCC_GPIOD_CLK_ENABLE();
#define LED_BLUE_PORT_CLK_DISABLE()              __HAL_RCC_GPIOD_CLK_DISABLE();

#define LED_GREEN_PORT_CLK_ENABLE()              __HAL_RCC_GPIOD_CLK_ENABLE();
#define LED_GREEN_PORT_CLK_DISABLE()             __HAL_RCC_GPIOD_CLK_DISABLE();
/* Exported macro ------------------------------------------------------------*/
#define LED_RED_ON()                             {LED_RED_PORT->BSRR = LED_RED_PORT_PIN;}
#define LED_RED_OFF()                            {LED_RED_PORT->BSRR = (uint32_t)LED_RED_PORT_PIN << 16;}
#define LED_RED_TOG()                            {LED_RED_PORT->ODR ^= LED_RED_PORT_PIN;}

#define LED_BLUE_ON()                            {LED_BLUE_PORT->BSRR = LED_BLUE_PORT_PIN;}
#define LED_BLUE_OFF()                           {LED_BLUE_PORT->BSRR = (uint32_t)LED_BLUE_PORT_PIN << 16;}
#define LED_BLUE_TOG()                           {LED_BLUE_PORT->ODR ^= LED_BLUE_PORT_PIN;}

#define LED_GREEN_ON()                           {LED_GREEN_PORT->BSRR = LED_GREEN_PORT_PIN;}
#define LED_GREEN_OFF()                          {LED_GREEN_PORT->BSRR = (uint32_t)LED_GREEN_PORT_PIN << 16;}
#define LED_GREEN_TOG()                          {LED_GREEN_PORT->ODR ^= LED_GREEN_PORT_PIN;}
/* Exported functions ------------------------------------------------------- */
void BoardLedInit(void);

#endif /* __BOARDLED_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
