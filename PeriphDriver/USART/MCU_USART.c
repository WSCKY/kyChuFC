/**
  ******************************************************************************
  * @file    MCU_USART.c
  * @author  kyChu
  * @version V0.1
  * @date    05-January-2018
  * @brief   MCU USART Port Driver.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "MCU_USART.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* USART handler declaration */
#if (USART3_ENABLE)
  static uint8_t _usart3_init = 0;
  static USART_HandleTypeDef Usart3Handle;
#if (USART3_TX_DMA_ENABLE)
  static DMA_HandleTypeDef hdma_usart3_tx;
#endif /* USART3_TX_DMA_ENABLE */
#if (USART3_RX_DMA_ENABLE)
  static DMA_HandleTypeDef hdma_usart3_rx;
#endif /* USART3_RX_DMA_ENABLE */
#endif /* USART3_ENABLE */
/* Private function prototypes -----------------------------------------------*/
  static HAL_StatusTypeDef USART3_PeriphInit(void);
/* Private functions ---------------------------------------------------------*/

/*
* @brief  Initializes MCU USARTs.
* @param  None
* @retval Initial status.
*/
HAL_StatusTypeDef MCU_USARTs_Init(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
#if (USART3_ENABLE)
	ret = USART3_PeriphInit(); if(ret != HAL_OK) return ret;
	_usart3_init = 1;
#endif /* USART3_ENABLE */
	return ret;
}

#if (USART3_ENABLE)
/*
 * @brief  MCU USART3 Port Initialize.
 * @param  None
 * @retval None
 */
static HAL_StatusTypeDef USART3_PeriphInit(void)
{
	/*##-1- Configure the USART peripheral ######################################*/
	/* Put the USART peripheral in the Asynchronous mode (USART Mode) */
	/* USART configured as follows:
		- Word Length = 8 Bits
		- Stop Bit = One Stop bit
		- Parity = None
		- BaudRate = USART3_BAUDRATE
		- Hardware flow control disabled (RTS and CTS signals) */
	Usart3Handle.Instance        = USART3;

	Usart3Handle.Init.BaudRate   = USART3_BAUDRATE;
	Usart3Handle.Init.WordLength = UART_WORDLENGTH_8B;
	Usart3Handle.Init.StopBits   = UART_STOPBITS_1;
	Usart3Handle.Init.Parity     = UART_PARITY_NONE;
	Usart3Handle.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_USART_DeInit(&Usart3Handle) != HAL_OK) {
		return HAL_ERROR;
	}
	if(HAL_USART_Init(&Usart3Handle) != HAL_OK) {
		return HAL_ERROR;
	}
	return HAL_OK;
}
#endif /* USART3_ENABLE */

#if (USART3_ENABLE)
#if (USART3_TX_DMA_ENABLE)
/*
 * @brief  Send an amount of data in DMA mode.
 * @param  pBuffer: pointer to data buffer.
 * @param  len: amount of data to be sent.
 * @retval Send status.
 */
HAL_StatusTypeDef Usart3SendBuffer_DMA(void *pBuffer, uint16_t len)
{
	return HAL_USART_Transmit_DMA(&Usart3Handle, (uint8_t*)pBuffer, len);
}
#endif /* USART3_TX_DMA_ENABLE */
#endif /* USART3_ENABLE */

/**
  * @brief USART GPIO Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: USART handle pointer
  * @retval None
  */
void HAL_USART_MspInit(USART_HandleTypeDef *husart)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
#if (USART3_ENABLE)
	if(husart->Instance == USART3) {
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		USART3_TX_GPIO_CLK_ENABLE(); /* Enable GPIO TX clock */
		USART3_RX_GPIO_CLK_ENABLE(); /* Enable GPIO RX clock */
		USART3_CLK_ENABLE(); /* Enable USART3 clock */
#if (USART3_TX_DMA_ENABLE | USART3_RX_DMA_ENABLE)
		USART3_DMA_CLK_ENABLE(); /* Enable DMA clock */
#endif /* USART3_TX_DMA_ENABLE | USART3_RX_DMA_ENABLE */
		/*##-2- Configure peripheral GPIO ##########################################*/
		/* USART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = USART3_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = USART3_TX_AF;
		HAL_GPIO_Init(USART3_TX_GPIO_PORT, &GPIO_InitStruct);
		/* USART RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = USART3_RX_PIN;
		GPIO_InitStruct.Alternate = USART3_RX_AF;
		HAL_GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStruct);
#if (USART3_TX_DMA_ENABLE)
		/*##-3- Configure the DMA ##################################################*/
		/* Configure the DMA handler for Transmission process */
		hdma_usart3_tx.Instance                 = USART3_TX_DMA_STREAM;
		hdma_usart3_tx.Init.Channel             = USART3_TX_DMA_CHANNEL;
		hdma_usart3_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		hdma_usart3_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_usart3_tx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart3_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_usart3_tx.Init.Mode                = DMA_NORMAL;
		hdma_usart3_tx.Init.Priority            = DMA_PRIORITY_LOW;
		HAL_DMA_Init(&hdma_usart3_tx);
		/* Associate the initialized DMA handle to the USART handle */
		__HAL_LINKDMA(husart, hdmatx, hdma_usart3_tx);
		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (USART3_TX) */
		HAL_NVIC_SetPriority(USART3_DMA_TX_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USART3_DMA_TX_IRQn);
#endif /* USART3_TX_DMA_ENABLE */
#if (USART3_RX_DMA_ENABLE)
		/*##-3- Configure the DMA ##################################################*/
		/* Configure the DMA handler for Transmission process */
		hdma_usart3_rx.Instance                 = USART3_RX_DMA_STREAM;
		hdma_usart3_rx.Init.Channel             = USART3_RX_DMA_CHANNEL;
		hdma_usart3_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		hdma_usart3_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_usart3_rx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_usart3_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_usart3_rx.Init.Mode                = DMA_NORMAL;
		hdma_usart3_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_usart3_rx);
		/* Associate the initialized DMA handle to the USART handle */
		__HAL_LINKDMA(husart, hdmarx, hdma_usart3_rx);
		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (USART3_RX) */
		HAL_NVIC_SetPriority(USART3_DMA_RX_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART3_DMA_RX_IRQn);
#endif /* USART3_RX_DMA_ENABLE */
		/* NVIC for USART, to catch the TX complete */
		HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
	}
#endif /* USART3_ENABLE */
}

/**
  * @brief USART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: USART handle pointer
  * @retval None
  */
void HAL_USART_MspDeInit(USART_HandleTypeDef *husart)
{
#if (USART3_ENABLE)
	if(husart->Instance == USART3) {
		/*##-1- Reset peripherals ##################################################*/
		USART3_FORCE_RESET();
		USART3_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks #################################*/
		/* Configure USARTx Tx as alternate function  */
		HAL_GPIO_DeInit(USART3_TX_GPIO_PORT, USART3_TX_PIN);
		/* Configure USARTx Rx as alternate function  */
		HAL_GPIO_DeInit(USART3_RX_GPIO_PORT, USART3_RX_PIN);
#if (USART3_TX_DMA_ENABLE)
		/*##-3- Disable the DMA #####################################################*/
		/* De-Initialize the DMA channel associated to transmission process */
		if(husart->hdmatx != 0) {
			HAL_DMA_DeInit(husart->hdmatx);
		}
		/*##-4- Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(USART3_DMA_TX_IRQn);
#endif /* USART3_TX_DMA_ENABLE */
#if (USART3_RX_DMA_ENABLE)
		/*##-3- Disable the DMA #####################################################*/
		/* De-Initialize the DMA channel associated to transmission process */
		if(husart->hdmarx != 0) {
			HAL_DMA_DeInit(husart->hdmarx);
		}
		/*##-4- Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(USART3_DMA_RX_IRQn);
#endif /* USART3_RX_DMA_ENABLE */
	}
#endif /* USART3_ENABLE */
}

#if (USART3_ENABLE)
#if (USART3_TX_DMA_ENABLE)
/**
  * @brief  This function handles USART3 DMA interrupt request.
  * @param  None
  * @retval None
  */
void USART3_DMA_TX_IRQHandler(void)
{
	if(_usart3_init == 1)
		HAL_DMA_IRQHandler(Usart3Handle.hdmatx);
}
#endif /* USART3_TX_DMA_ENABLE */
#if (USART3_RX_DMA_ENABLE)
/**
  * @brief  This function handles USART3 DMA interrupt request.
  * @param  None
  * @retval None
  */
void USART3_DMA_RX_IRQHandler(void)
{
	if(_usart3_init == 1)
		HAL_DMA_IRQHandler(Usart3Handle.hdmarx);
}
#endif /* USART3_RX_DMA_ENABLE */
/**
  * @brief  This function handles UART7 interrupt request.
  * @param  None
  * @retval None
  */
void USART3_IRQHandler(void)
{
	if(_usart3_init == 1)
		HAL_USART_IRQHandler(&Usart3Handle);
}
#endif /* USART3_ENABLE */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
