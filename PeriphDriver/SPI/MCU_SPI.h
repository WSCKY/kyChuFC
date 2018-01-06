/**
  ******************************************************************************
  * @file    MCU_SPI.h
  * @author  kyChu
  * @version V0.1
  * @date    06-January-2018
  * @brief   Header file for MCU SPI Driver.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MCU_SPI_H
#define __MCU_SPI_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"
#include "../PeriphDefine.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported definitions ------------------------------------------------------*/
#define SPI2_ENABLE                              (1)
#define SPI4_ENABLE                              (0)

#if (SPI2_ENABLE)
  #define SPI2_MODE_MASTER                       (1)
  #define SPI2_TX_DMA_ENABLE                     (1)
  #define SPI2_RX_DMA_ENABLE                     (1)
#endif /* SPI2_ENABLE */
#if (SPI4_ENABLE)
  #define SPI4_MODE_MASTER                       (1)
  #define SPI4_TX_DMA_ENABLE                     (1)
  #define SPI4_RX_DMA_ENABLE                     (1)
#endif /* SPI4_ENABLE */
/* Exported macros -----------------------------------------------------------*/
#if (SPI2_ENABLE)
  /* Definition for SPI2 clock resources */
  #define SPI2_CLK_ENABLE()                        __HAL_RCC_SPI2_CLK_ENABLE()
  #define SPI2_FORCE_RESET()                       __HAL_RCC_SPI2_FORCE_RESET()
  #define SPI2_RELEASE_RESET()                     __HAL_RCC_SPI2_RELEASE_RESET()
  #define SPI2_SCK_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOB_CLK_ENABLE()
  #define SPI2_MISO_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()
  #define SPI2_MOSI_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOB_CLK_ENABLE()
  #define SPI2_DMA_CLK_ENABLE()                    __HAL_RCC_DMA1_CLK_ENABLE()

  /* Definition for SPI2 Pins */
  #define SPI2_SCK_PIN                             GPIO_PIN_13
  #define SPI2_SCK_GPIO_PORT                       GPIOB
  #define SPI2_SCK_AF                              GPIO_AF5_SPI2
  #define SPI2_MISO_PIN                            GPIO_PIN_14
  #define SPI2_MISO_GPIO_PORT                      GPIOB
  #define SPI2_MISO_AF                             GPIO_AF5_SPI2
  #define SPI2_MOSI_PIN                            GPIO_PIN_15
  #define SPI2_MOSI_GPIO_PORT                      GPIOB
  #define SPI2_MOSI_AF                             GPIO_AF5_SPI2

  /* Definition for SPI2's DMA */
#if (SPI2_TX_DMA_ENABLE)
  #define SPI2_TX_DMA_STREAM                       DMA1_Stream4
  #define SPI2_TX_DMA_CHANNEL                      DMA_CHANNEL_0
  #define SPI2_DMA_TX_IRQn                         DMA1_Stream4_IRQn
//  #define SPI2_DMA_TX_IRQHandler                   DMA1_Stream4_IRQHandler
#endif /* SPI2_TX_DMA_ENABLE */
#if (SPI2_RX_DMA_ENABLE)
  #define SPI2_RX_DMA_STREAM                       DMA1_Stream3
  #define SPI2_RX_DMA_CHANNEL                      DMA_CHANNEL_0
  #define SPI2_DMA_RX_IRQn                         DMA1_Stream3_IRQn
//  #define SPI2_DMA_RX_IRQHandler                   DMA1_Stream3_IRQHandler
#endif /* SPI2_RX_DMA_ENABLE */
#endif /* SPI2_ENABLE */

#if (SPI4_ENABLE)
  /* Definition for SPI4 clock resources */
  #define SPI4_CLK_ENABLE()                        __HAL_RCC_SPI4_CLK_ENABLE()
  #define SPI4_FORCE_RESET()                       __HAL_RCC_SPI4_FORCE_RESET()
  #define SPI4_RELEASE_RESET()                     __HAL_RCC_SPI4_RELEASE_RESET()
  #define SPI4_SCK_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOE_CLK_ENABLE()
  #define SPI4_MISO_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOE_CLK_ENABLE()
  #define SPI4_MOSI_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOE_CLK_ENABLE()
  #define SPI4_DMA_CLK_ENABLE()                    __HAL_RCC_DMA2_CLK_ENABLE()

  /* Definition for SPI4 Pins */
  #define SPI4_SCK_PIN                             GPIO_PIN_12
  #define SPI4_SCK_GPIO_PORT                       GPIOE
  #define SPI4_SCK_AF                              GPIO_AF5_SPI4
  #define SPI4_MISO_PIN                            GPIO_PIN_13
  #define SPI4_MISO_GPIO_PORT                      GPIOE
  #define SPI4_MISO_AF                             GPIO_AF5_SPI4
  #define SPI4_MOSI_PIN                            GPIO_PIN_14
  #define SPI4_MOSI_GPIO_PORT                      GPIOE
  #define SPI4_MOSI_AF                             GPIO_AF5_SPI4

  /* Definition for SPI4's DMA */
#if (SPI4_TX_DMA_ENABLE)
  #define SPI4_TX_DMA_STREAM                       DMA2_Stream1
  #define SPI4_TX_DMA_CHANNEL                      DMA_CHANNEL_4
  #define SPI4_DMA_TX_IRQn                         DMA2_Stream1_IRQn
//  #define SPI4_DMA_TX_IRQHandler                   DMA2_Stream1_IRQHandler
#endif /* SPI4_TX_DMA_ENABLE */
#if (SPI4_RX_DMA_ENABLE)
  #define SPI4_RX_DMA_STREAM                       DMA2_Stream0
  #define SPI4_RX_DMA_CHANNEL                      DMA_CHANNEL_4
  #define SPI4_DMA_RX_IRQn                         DMA2_Stream0_IRQn
//  #define SPI4_DMA_RX_IRQHandler                   DMA2_Stream0_IRQHandler
#endif /* SPI4_RX_DMA_ENABLE */
#endif /* SPI4_ENABLE */
/* Exported functions ------------------------------------------------------- */
HAL_StatusTypeDef MCU_SPIs_Init(void);

#if (SPI2_ENABLE)
  void MCU_SPI2_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
  void MCU_SPI2_ErrorCallback(SPI_HandleTypeDef *hspi);
#if (SPI2_RX_DMA_ENABLE)
  void SPI2_DMA_RX_IRQHandler(void);
#endif /* SPI2_RX_DMA_ENABLE */
#if (SPI2_TX_DMA_ENABLE)
  void SPI2_DMA_TX_IRQHandler(void);
#endif /* SPI2_TX_DMA_ENABLE */
#if (SPI2_TX_DMA_ENABLE & SPI2_RX_DMA_ENABLE)
  HAL_StatusTypeDef SPI2_TxRxData_DMA(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
#endif /* SPI2_TX_DMA_ENABLE & SPI2_RX_DMA_ENABLE */
#endif /* SPI2_ENABLE */

#if (SPI4_ENABLE)
  void MCU_SPI4_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
  void MCU_SPI4_ErrorCallback(SPI_HandleTypeDef *hspi);
#if (SPI4_RX_DMA_ENABLE)
  void SPI4_DMA_RX_IRQHandler(void);
#endif /* SPI4_RX_DMA_ENABLE */
#if (SPI4_TX_DMA_ENABLE)
  void SPI4_DMA_TX_IRQHandler(void);
#endif /* SPI4_TX_DMA_ENABLE */
#if (SPI4_TX_DMA_ENABLE & SPI4_RX_DMA_ENABLE)
  HAL_StatusTypeDef SPI4_TxRxData_DMA(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
#endif /* SPI4_TX_DMA_ENABLE & SPI4_RX_DMA_ENABLE */
#endif /* SPI4_ENABLE */

#endif /* __MCU_SPI_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
