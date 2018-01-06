/**
  ******************************************************************************
  * @file    MCU_USART.h
  * @author  kyChu
  * @version V0.1
  * @date    05-January-2018
  * @brief   Header file for MCU USART Driver.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_USART_H
#define __MCU_USART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "../PeriphDefine.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported definitions ------------------------------------------------------*/
#define USART3_ENABLE                            (1)

#if (USART3_ENABLE)
  #define USART3_TX_DMA_ENABLE                   (0)
  #define USART3_RX_DMA_ENABLE                   (0)
#endif /* USART3_ENABLE */
/* Exported macros -----------------------------------------------------------*/

#if (USART3_ENABLE)
  /* Definition for USART3 peripheral configuration */
  #define USART3_BAUDRATE                        115200
  /* Definition for USART3 clock resources */
  #define USART3_CLK_ENABLE()                    __USART3_CLK_ENABLE()
  #define USART3_FORCE_RESET()                   __USART3_FORCE_RESET()
  #define USART3_RELEASE_RESET()                 __USART3_RELEASE_RESET()
  #define USART3_RX_GPIO_CLK_ENABLE()            __GPIOD_CLK_ENABLE()
  #define USART3_TX_GPIO_CLK_ENABLE()            __GPIOD_CLK_ENABLE()
  #define USART3_DMA_CLK_ENABLE()                __HAL_RCC_DMA1_CLK_ENABLE()
  /* Definition for USART3 Pins */
  #define USART3_TX_PIN                          GPIO_PIN_8
  #define USART3_TX_GPIO_PORT                    GPIOD
  #define USART3_TX_AF                           GPIO_AF7_USART3
  #define USART3_RX_PIN                          GPIO_PIN_9
  #define USART3_RX_GPIO_PORT                    GPIOD
  #define USART3_RX_AF                           GPIO_AF7_USART3
#if (USART3_TX_DMA_ENABLE)
  /* Definition for USART3's TX_DMA */
  #define USART3_TX_DMA_STREAM                   DMA1_Stream4
  #define USART3_TX_DMA_CHANNEL                  DMA_CHANNEL_7
  /* Definition for USART3's TX_NVIC */
  #define USART3_DMA_TX_IRQn                     DMA1_Stream4_IRQn
//  #define USART3_DMA_TX_IRQHandler               DMA1_Stream4_IRQHandler
#endif /* USART3_TX_DMA_ENABLE */
#if (USART3_RX_DMA_ENABLE)
  /* Definition for USART3's RX_DMA */
  #define USART3_RX_DMA_STREAM                   DMA1_Stream1
  #define USART3_RX_DMA_CHANNEL                  DMA_CHANNEL_4
  /* Definition for USART3's RX_NVIC */
  #define USART3_DMA_RX_IRQn                     DMA1_Stream1_IRQn
//  #define USART3_DMA_RX_IRQHandler               DMA1_Stream1_IRQHandler
#endif /* USART3_RX_DMA_ENABLE */
#endif /* USART3_ENABLE */
/* Exported functions ------------------------------------------------------- */
HAL_StatusTypeDef MCU_USARTs_Init(void);
#if (USART3_ENABLE)
#if (USART3_TX_DMA_ENABLE)
  HAL_StatusTypeDef Usart3SendBuffer_DMA(void *pBuffer, uint16_t len);

  void USART3_DMA_TX_IRQHandler(void);
#endif /* USART3_TX_DMA_ENABLE */
#if (USART3_RX_DMA_ENABLE)
  void USART3_DMA_RX_IRQHandler(void);
#endif /* USART3_RX_DMA_ENABLE */
#endif /* USART3_ENABLE */

#endif /* __MCU_USART_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
