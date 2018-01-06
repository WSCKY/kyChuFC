/**
  ******************************************************************************
  * @file    BoardPower.h
  * @author  kyChu
  * @version V0.1
  * @date    04-January-2018
  * @brief   Header file for Board Power Control Driver.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOARDPOWER_H
#define __BOARDPOWER_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"

/* Macros --------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported definitions ------------------------------------------------------*/
#define PWR_CTRL_PORT                            GPIOB
#define PWR_CTRL_PORT_PIN                        GPIO_PIN_1

#define PWR_CTRL_PORT_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE();
#define PWR_CTRL_PORT_CLK_DISABLE()              __HAL_RCC_GPIOB_CLK_DISABLE();
/* Exported macro ------------------------------------------------------------*/
#define PWR_ON()                                 do {PWR_CTRL_PORT->BSRR = PWR_CTRL_PORT_PIN;} while(0)
#define PWR_OFF()                                do {PWR_CTRL_PORT->BSRR = (uint32_t)PWR_CTRL_PORT_PIN << 16;} while(0)
/* Exported functions ------------------------------------------------------- */
void PowerControlInit(void);

#endif /* __BOARDPOWER_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
