/**
  ******************************************************************************
  * @file    MCU_UART.h
  * @author  kyChu
  * @version V0.1
  * @date    05-January-2018
  * @brief   Header file for MCU UART Driver.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_UART_H
#define __MCU_UART_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "../PeriphDefine.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported definitions ------------------------------------------------------*/
#define UART7_ENABLE                             (1)
#define UART8_ENABLE                             (1)

#if (UART7_ENABLE)
  #define UART7_TX_DMA_ENABLE                    (1)
  #define UART7_RX_DMA_ENABLE                    (0)
#endif /* UART7_ENABLE */
#if (UART8_ENABLE)
  #define UART8_TX_DMA_ENABLE                    (1)
  #define UART8_RX_DMA_ENABLE                    (0)
#endif /* UART8_ENABLE */
/* Exported macros -----------------------------------------------------------*/
#if (UART7_ENABLE)
  /* Definition for UART7 peripheral configuration */
  #define UART7_BAUDRATE                         115200
  #define UART7_RX_CACHE_SIZE                    (1 << 8) /* !!! MAX = 256 !!! */
  /* Definition for UART7 clock resources */
  #define UART7_CLK_ENABLE()                     __UART7_CLK_ENABLE()
  #define UART7_FORCE_RESET()                    __UART7_FORCE_RESET()
  #define UART7_RELEASE_RESET()                  __UART7_RELEASE_RESET()
  #define UART7_RX_GPIO_CLK_ENABLE()             __GPIOE_CLK_ENABLE()
  #define UART7_TX_GPIO_CLK_ENABLE()             __GPIOE_CLK_ENABLE()
  #define UART7_DMA_CLK_ENABLE()                 __HAL_RCC_DMA1_CLK_ENABLE()
  /* Definition for UART7 Pins */
  #define UART7_TX_PIN                           GPIO_PIN_8
  #define UART7_TX_GPIO_PORT                     GPIOE
  #define UART7_TX_AF                            GPIO_AF8_UART7
  #define UART7_RX_PIN                           GPIO_PIN_7
  #define UART7_RX_GPIO_PORT                     GPIOE
  #define UART7_RX_AF                            GPIO_AF8_UART7
#if (UART7_TX_DMA_ENABLE)
  /* Definition for UART7's TX_DMA */
  #define UART7_TX_DMA_STREAM                    DMA1_Stream1
  #define UART7_TX_DMA_CHANNEL                   DMA_CHANNEL_5
  /* Definition for UART7's TX_NVIC */
  #define UART7_DMA_TX_IRQn                      DMA1_Stream1_IRQn
//  #define UART7_DMA_TX_IRQHandler                DMA1_Stream1_IRQHandler
#endif /* UART7_TX_DMA_ENABLE */
#if (UART7_RX_DMA_ENABLE)
  /* Definition for UART7's RX_DMA */
  #define UART7_RX_DMA_STREAM                    DMA1_Stream3
  #define UART7_RX_DMA_CHANNEL                   DMA_CHANNEL_5
  /* Definition for UART7's RX_NVIC */
  #define UART7_DMA_RX_IRQn                      DMA1_Stream3_IRQn
//  #define UART7_DMA_RX_IRQHandler                DMA1_Stream3_IRQHandler
#endif /* UART7_RX_DMA_ENABLE */
#endif /* UART7_ENABLE */

#if (UART8_ENABLE)
  /* Definition for UART8 peripheral configuration */
  #define UART8_BAUDRATE                         115200
  #define UART8_RX_CACHE_SIZE                    (1 << 8) /* !!! MAX = 256 !!! */
  /* Definition for UART8 clock resources */
  #define UART8_CLK_ENABLE()                     __UART8_CLK_ENABLE()
  #define UART8_FORCE_RESET()                    __UART8_FORCE_RESET()
  #define UART8_RELEASE_RESET()                  __UART8_RELEASE_RESET()
  #define UART8_RX_GPIO_CLK_ENABLE()             __GPIOE_CLK_ENABLE()
  #define UART8_TX_GPIO_CLK_ENABLE()             __GPIOE_CLK_ENABLE()
  #define UART8_DMA_CLK_ENABLE()                 __HAL_RCC_DMA1_CLK_ENABLE()
  /* Definition for UART8 Pins */
  #define UART8_TX_PIN                           GPIO_PIN_1
  #define UART8_TX_GPIO_PORT                     GPIOE
  #define UART8_TX_AF                            GPIO_AF8_UART8
  #define UART8_RX_PIN                           GPIO_PIN_0
  #define UART8_RX_GPIO_PORT                     GPIOE
  #define UART8_RX_AF                            GPIO_AF8_UART8
#if (UART8_TX_DMA_ENABLE)
  /* Definition for UART8's TX_DMA */
  #define UART8_TX_DMA_STREAM                    DMA1_Stream0
  #define UART8_TX_DMA_CHANNEL                   DMA_CHANNEL_5
  /* Definition for UART8's TX_NVIC */
  #define UART8_DMA_TX_IRQn                      DMA1_Stream0_IRQn
//  #define UART8_DMA_TX_IRQHandler                DMA1_Stream0_IRQHandler
#endif /* UART8_TX_DMA_ENABLE */
#if (UART8_RX_DMA_ENABLE)
  /* Definition for UART8's RX_DMA */
  #define UART8_RX_DMA_STREAM                    DMA1_Stream6
  #define UART8_RX_DMA_CHANNEL                   DMA_CHANNEL_5
  /* Definition for UART8's RX_NVIC */
  #define UART8_DMA_RX_IRQn                      DMA1_Stream6_IRQn
//  #define UART8_DMA_RX_IRQHandler                DMA1_Stream6_IRQHandler
#endif /* UART8_RX_DMA_ENABLE */
#endif /* UART8_ENABLE */

/* Exported functions ------------------------------------------------------- */
HAL_StatusTypeDef MCU_UARTs_Init(void);
#if (UART7_ENABLE)
  HAL_StatusTypeDef Uart7PullOneByte(uint8_t *pByte);
#if (UART7_TX_DMA_ENABLE)
  HAL_StatusTypeDef Uart7SendBuffer_DMA(void *pBuffer, uint16_t len);

  void UART7_DMA_TX_IRQHandler(void);
#endif /* UART7_TX_DMA_ENABLE */
#if (UART7_RX_DMA_ENABLE)
  void UART7_DMA_RX_IRQHandler(void);
#endif /* UART7_RX_DMA_ENABLE */
#endif /* UART7_ENABLE */
#if (UART8_ENABLE)
  HAL_StatusTypeDef Uart8PullOneByte(uint8_t *pByte);
#if (UART8_TX_DMA_ENABLE)
  HAL_StatusTypeDef Uart8SendBuffer_DMA(void *pBuffer, uint16_t len);

  void UART8_DMA_TX_IRQHandler(void);
#endif /* UART8_TX_DMA_ENABLE */
#if (UART8_RX_DMA_ENABLE)
  void UART8_DMA_RX_IRQHandler(void);
#endif /* UART8_RX_DMA_ENABLE */
#endif /* UART8_ENABLE */

#endif /* __MCU_UART_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
