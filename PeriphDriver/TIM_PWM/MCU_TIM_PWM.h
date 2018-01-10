/**
  ******************************************************************************
  * @file    MCU_TIM_PWM.h
  * @author  kyChu
  * @version V0.1
  * @date    05-January-2018
  * @brief   Header file for MCU Timer PWM Driver.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_TIM_PWM_H
#define __MCU_TIM_PWM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "../PeriphDefine.h"

/* Exported types ------------------------------------------------------------*/
#define TIM_POLARITY_CHANNEL_1                   (0x0002)
#define TIM_POLARITY_CHANNEL_2                   (0x0020)
#define TIM_POLARITY_CHANNEL_3                   (0x0200)
#define TIM_POLARITY_CHANNEL_4                   (0x2000)

typedef enum {
	PolarityHigh = 0,
	PolarityLow = 1
} PWM_PolarityState;
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported definitions ------------------------------------------------------*/
#define TIM2_ENABLE                              (1)
#if (TIM2_ENABLE)
  #define TIM2_PWM_OUTPUT_ENABLE                 (1)
  #define TIM2_PWM_CHANNEL1_ENABLE               (1)
  #define TIM2_PWM_CHANNEL2_ENABLE               (1)
  #define TIM2_PWM_CHANNEL3_ENABLE               (1)
  #define TIM2_PWM_CHANNEL4_ENABLE               (1)
#endif /* TIM2_ENABLE */

#define TIM5_ENABLE                              (1)
#if (TIM5_ENABLE)
  #define TIM5_PWM_OUTPUT_ENABLE                 (1)
  #define TIM5_PWM_CHANNEL1_ENABLE               (1)
  #define TIM5_PWM_CHANNEL2_ENABLE               (1)
  #define TIM5_PWM_CHANNEL3_ENABLE               (1)
  #define TIM5_PWM_CHANNEL4_ENABLE               (1)
#endif /* TIM5_ENABLE */
/* Exported macros -----------------------------------------------------------*/
#if (TIM2_ENABLE)
  /* Definition for TIM2 peripheral configuration */
  #define TIM2_COUNT_CLOCK_RATE                  (27000000)
  #define TIM2_OUTPUT_CLOCK_RATE                 (27000)
  #define TIM2_PERIOD_SET_DEFAULT()              (TIM2->ARR = (TIM2_COUNT_CLOCK_RATE / TIM2_OUTPUT_CLOCK_RATE) - 1)

  #define TIM2_COUNTERMODE                       TIM_COUNTERMODE_CENTERALIGNED3
  #define TIM2_AUTORELOAD_PRELOAD                (1)
  /* Definition for TIM2 clock resources */
  #define TIM2_CLK_ENABLE()                      __TIM2_CLK_ENABLE()
#if (TIM2_PWM_OUTPUT_ENABLE)
  #define TIM2_PWM_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOA_CLK_ENABLE(); __HAL_RCC_GPIOB_CLK_ENABLE()

  /* Definition for TIM2 Channel Pins */
#if (TIM2_PWM_CHANNEL1_ENABLE)
  #define TIM2_GPIO_PORT_CHANNEL1                GPIOA
  #define TIM2_GPIO_PIN_CHANNEL1                 GPIO_PIN_15
  #define TIM2_GPIO_AF_CHANNEL1                  GPIO_AF1_TIM2
  #define TIM2_PWM_DEFAUTL_PULSE_1               0
#endif /* TIM2_PWM_CHANNEL1_ENABLE */
#if (TIM2_PWM_CHANNEL2_ENABLE)
  #define TIM2_GPIO_PORT_CHANNEL2                GPIOB
  #define TIM2_GPIO_PIN_CHANNEL2                 GPIO_PIN_3
  #define TIM2_GPIO_AF_CHANNEL2                  GPIO_AF1_TIM2
  #define TIM2_PWM_DEFAUTL_PULSE_2               0
#endif /* TIM2_PWM_CHANNEL2_ENABLE */
#if (TIM2_PWM_CHANNEL3_ENABLE)
  #define TIM2_GPIO_PORT_CHANNEL3                GPIOB
  #define TIM2_GPIO_PIN_CHANNEL3                 GPIO_PIN_10
  #define TIM2_GPIO_AF_CHANNEL3                  GPIO_AF1_TIM2
  #define TIM2_PWM_DEFAUTL_PULSE_3               0
#endif /* TIM2_PWM_CHANNEL3_ENABLE */
#if (TIM2_PWM_CHANNEL4_ENABLE)
  #define TIM2_GPIO_PORT_CHANNEL4                GPIOB
  #define TIM2_GPIO_PIN_CHANNEL4                 GPIO_PIN_11
  #define TIM2_GPIO_AF_CHANNEL4                  GPIO_AF1_TIM2
  #define TIM2_PWM_DEFAUTL_PULSE_4               0
#endif /* TIM2_PWM_CHANNEL4_ENABLE */
#endif /* TIM2_PWM_OUTPUT_ENABLE */
#endif /* TIM2_ENABLE */
#if (TIM5_ENABLE)
  /* Definition for TIM5 peripheral configuration */
  #define TIM5_COUNT_CLOCK_RATE                  (27000000)
  #define TIM5_OUTPUT_CLOCK_RATE                 (27000)
  #define TIM5_PERIOD_SET_DEFAULT()              (TIM5->ARR = (TIM5_COUNT_CLOCK_RATE / TIM5_OUTPUT_CLOCK_RATE) - 1)

  #define TIM5_COUNTERMODE                       TIM_COUNTERMODE_CENTERALIGNED3
  #define TIM5_AUTORELOAD_PRELOAD                (1)
  /* Definition for TIM5 clock resources */
  #define TIM5_CLK_ENABLE()                      __TIM5_CLK_ENABLE()
#if (TIM5_PWM_OUTPUT_ENABLE)
  #define TIM5_PWM_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOA_CLK_ENABLE()

  /* Definition for TIM5 Channel Pins */
#if (TIM5_PWM_CHANNEL1_ENABLE)
  #define TIM5_GPIO_PORT_CHANNEL1                GPIOA
  #define TIM5_GPIO_PIN_CHANNEL1                 GPIO_PIN_0
  #define TIM5_GPIO_AF_CHANNEL1                  GPIO_AF2_TIM5
  #define TIM5_PWM_DEFAUTL_PULSE_1               0
#endif /* TIM5_PWM_CHANNEL1_ENABLE */
#if (TIM5_PWM_CHANNEL2_ENABLE)
  #define TIM5_GPIO_PORT_CHANNEL2                GPIOA
  #define TIM5_GPIO_PIN_CHANNEL2                 GPIO_PIN_1
  #define TIM5_GPIO_AF_CHANNEL2                  GPIO_AF2_TIM5
  #define TIM5_PWM_DEFAUTL_PULSE_2               0
#endif /* TIM5_PWM_CHANNEL2_ENABLE */
#if (TIM5_PWM_CHANNEL3_ENABLE)
  #define TIM5_GPIO_PORT_CHANNEL3                GPIOA
  #define TIM5_GPIO_PIN_CHANNEL3                 GPIO_PIN_2
  #define TIM5_GPIO_AF_CHANNEL3                  GPIO_AF2_TIM5
  #define TIM5_PWM_DEFAUTL_PULSE_3               0
#endif /* TIM5_PWM_CHANNEL3_ENABLE */
#if (TIM5_PWM_CHANNEL4_ENABLE)
  #define TIM5_GPIO_PORT_CHANNEL4                GPIOA
  #define TIM5_GPIO_PIN_CHANNEL4                 GPIO_PIN_3
  #define TIM5_GPIO_AF_CHANNEL4                  GPIO_AF2_TIM5
  #define TIM5_PWM_DEFAUTL_PULSE_4               0
#endif /* TIM5_PWM_CHANNEL4_ENABLE */
#endif /* TIM5_PWM_OUTPUT_ENABLE */
#endif /* TIM5_ENABLE */
/* Exported functions ------------------------------------------------------- */
HAL_StatusTypeDef MCU_TIMs_Init(void);
#if (TIM2_ENABLE)
void TIM2_SetPeriod(uint32_t Period);
#if (TIM2_PWM_OUTPUT_ENABLE)
  void TIM2_PWM_SetPolarity(uint32_t PolarityChannel, PWM_PolarityState Polarity);
  void TIM2_PWM_SetPulse(uint16_t CCR1, uint16_t CCR2, uint16_t CCR3, uint16_t CCR4);
#endif /* TIM2_PWM_OUTPUT_ENABLE */
#endif /* TIM2_ENABLE */

#if (TIM5_ENABLE)
  void TIM5_SetPeriod(uint32_t Period);
#if (TIM5_PWM_OUTPUT_ENABLE)
  void TIM5_PWM_SetPolarity(uint32_t PolarityChannel, PWM_PolarityState Polarity);
  void TIM5_PWM_SetPulse(uint16_t CCR1, uint16_t CCR2, uint16_t CCR3, uint16_t CCR4);
#endif /* TIM5_PWM_OUTPUT_ENABLE */
#endif /* TIM5_ENABLE */

#endif /* __MCU_TIM_PWM_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
