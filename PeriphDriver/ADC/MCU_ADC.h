/**
  ******************************************************************************
  * @file    MCU_ADC.h
  * @author  kyChu
  * @version V0.1
  * @date    12-January-2018
  * @brief   Header file for MCU ADC Driver.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_ADC_H
#define __MCU_ADC_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "../PeriphDefine.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported definitions ------------------------------------------------------*/
#define ADC1_ENABLE                              (1)
#define ADC2_ENABLE                              (0)
#define ADC3_ENABLE                              (0)

#if (ADC1_ENABLE)
  #define  ADC1_DMA_IRQ_ENABLE                   (0)
#endif /* ADC1_ENABLE */
/* Exported macros -----------------------------------------------------------*/
#if (ADC1_ENABLE)
  /* Definition for ADC1 clock resources */
  #define ADC1_CLK_ENABLE()                      __HAL_RCC_ADC1_CLK_ENABLE()
  #define ADC1_DMA_CLK_ENABLE()                  __HAL_RCC_DMA2_CLK_ENABLE()
  #define ADC1_CHANNEL_GPIO_CLK_ENABLE()         __HAL_RCC_GPIOC_CLK_ENABLE()
  #define ADC1_FORCE_RESET()                     __HAL_RCC_ADC_FORCE_RESET()
  #define ADC1_RELEASE_RESET()                   __HAL_RCC_ADC_RELEASE_RESET()

  /* Definition for ADC1 Channel Pin */
  #define ADC1_CHANNEL_PIN                       GPIO_PIN_5
  #define ADC1_CHANNEL_GPIO_PORT                 GPIOC

  /* Definition for ADC1's Channel */
  #define ADC1_CHANNEL                           ADC_CHANNEL_15

  /* Definition for ADC1's DMA */
  #define ADC1_DMA_CHANNEL                       DMA_CHANNEL_0
  #define ADC1_DMA_STREAM                        DMA2_Stream4
#if (ADC1_DMA_IRQ_ENABLE)
  /* Definition for ADC1's NVIC */
  #define ADC1_DMA_IRQn                          DMA2_Stream4_IRQn
//  #define ADC1_DMA_IRQHandler                    DMA2_Stream4_IRQHandler
#endif /* ADC1_DMA_IRQ_ENABLE */
#endif /* ADC1_ENABLE */

/* Exported functions ------------------------------------------------------- */
HAL_StatusTypeDef MCU_ADCs_Init(void);

#if (ADC1_ENABLE)
uint16_t *GetADC1ConvertedValue(void);
#if (ADC1_DMA_IRQ_ENABLE)
void ADC1_DMA_IRQHandler(void);
void MCU_ADC1_ConvCpltCallback(ADC_HandleTypeDef* hadc);
#endif /* ADC1_DMA_IRQ_ENABLE */
#endif /* ADC1_ENABLE */

#endif /* __MCU_ADC_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
