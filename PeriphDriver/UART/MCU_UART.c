/**
  ******************************************************************************
  * @file    MCU_UART.c
  * @author  kyChu
  * @version V0.1
  * @date    05-January-2018
  * @brief   MCU UART Port Driver.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "MCU_UART.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
#if (UART7_ENABLE)
  static uint8_t _uart7_init = 0;
  static UART_HandleTypeDef Uart7Handle;
#if (UART7_TX_DMA_ENABLE)
  static DMA_HandleTypeDef hdma_uart7_tx;
#endif /* UART7_TX_DMA_ENABLE */
#if (UART7_RX_DMA_ENABLE)
  static DMA_HandleTypeDef hdma_uart7_rx;
#endif /* UART7_RX_DMA_ENABLE */
#endif /* UART7_ENABLE */
#if (UART8_ENABLE)
  static uint8_t _uart8_init = 0;
  static UART_HandleTypeDef Uart8Handle;
#if (UART8_TX_DMA_ENABLE)
  static DMA_HandleTypeDef hdma_uart8_tx;
#endif /* UART8_TX_DMA_ENABLE */
#if (UART8_RX_DMA_ENABLE)
  static DMA_HandleTypeDef hdma_uart8_rx;
#endif /* UART8_RX_DMA_ENABLE */
#endif /* UART8_ENABLE */
/* Private function prototypes -----------------------------------------------*/
#if (UART7_ENABLE)
  static HAL_StatusTypeDef UART7_PeriphInit(void);
#endif /* UART7_ENABLE */
#if (UART8_ENABLE)
  static HAL_StatusTypeDef UART8_PeriphInit(void);
#endif /* UART8_ENABLE */
/* Private functions ---------------------------------------------------------*/

/*
 * @brief  Initializes MCU UARTs.
 * @param  None
 * @retval Initial status.
 */
HAL_StatusTypeDef MCU_UARTs_Init(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
#if (UART7_ENABLE)
	ret = UART7_PeriphInit(); if(ret != HAL_OK) return ret;
	_uart7_init = 1;
#endif /* UART7_ENABLE */
#if (UART8_ENABLE)
	ret = UART8_PeriphInit(); if(ret != HAL_OK) return ret;
	_uart8_init = 1;
#endif /* UART8_ENABLE */
	return ret;
}

#if (UART7_ENABLE)
/*
 * @brief  MCU UART7 Port Initialize.
 * @param  None
 * @retval None
 */
static HAL_StatusTypeDef UART7_PeriphInit(void)
{
	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the UART peripheral in the Asynchronous mode (UART Mode) */
	/* UART configured as follows:
		- Word Length = 8 Bits
		- Stop Bit = One Stop bit
		- Parity = None
		- BaudRate = UART7_BAUDRATE
		- Hardware flow control disabled (RTS and CTS signals) */
	Uart7Handle.Instance        = UART7;

	Uart7Handle.Init.BaudRate   = UART7_BAUDRATE;
	Uart7Handle.Init.WordLength = UART_WORDLENGTH_8B;
	Uart7Handle.Init.StopBits   = UART_STOPBITS_1;
	Uart7Handle.Init.Parity     = UART_PARITY_NONE;
	Uart7Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	Uart7Handle.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_UART_DeInit(&Uart7Handle) != HAL_OK) {
		return HAL_ERROR;
	}
	if(HAL_UART_Init(&Uart7Handle) != HAL_OK) {
		return HAL_ERROR;
	}
	return HAL_OK;
}
#endif /* UART7_ENABLE */
#if (UART8_ENABLE)
/*
 * @brief  MCU UART8 Port Initialize.
 * @param  None
 * @retval None
 */
static HAL_StatusTypeDef UART8_PeriphInit(void)
{
	/*##-1- Configure the UART peripheral ######################################*/
	/* Put the UART peripheral in the Asynchronous mode (UART Mode) */
	/* UART configured as follows:
		- Word Length = 8 Bits
		- Stop Bit = One Stop bit
		- Parity = None
		- BaudRate = UART8_BAUDRATE
		- Hardware flow control disabled (RTS and CTS signals) */
	Uart8Handle.Instance        = UART8;

	Uart8Handle.Init.BaudRate   = UART8_BAUDRATE;
	Uart8Handle.Init.WordLength = UART_WORDLENGTH_8B;
	Uart8Handle.Init.StopBits   = UART_STOPBITS_1;
	Uart8Handle.Init.Parity     = UART_PARITY_NONE;
	Uart8Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	Uart8Handle.Init.Mode       = UART_MODE_TX_RX;

	if(HAL_UART_DeInit(&Uart8Handle) != HAL_OK) {
		return HAL_ERROR;
	}
	if(HAL_UART_Init(&Uart8Handle) != HAL_OK) {
		return HAL_ERROR;
	}
	return HAL_OK;
}
#endif /* UART8_ENABLE */

#if (UART7_ENABLE)
#if (UART7_TX_DMA_ENABLE)
/*
 * @brief  Send an amount of data in DMA mode.
 * @param  pBuffer: pointer to data buffer.
 * @param  len: amount of data to be sent.
 * @retval Send status.
 */
HAL_StatusTypeDef Uart7SendBuffer_DMA(void *pBuffer, uint16_t len)
{
	return HAL_UART_Transmit_DMA(&Uart7Handle, (uint8_t*)pBuffer, len);
}
#endif /* UART7_TX_DMA_ENABLE */
#endif /* UART7_ENABLE */
#if (UART8_ENABLE)
#if (UART8_TX_DMA_ENABLE)
/*
 * @brief  Send an amount of data in DMA mode.
 * @param  pBuffer: pointer to data buffer.
 * @param  len: amount of data to be sent.
 * @retval Send status.
 */
HAL_StatusTypeDef Uart8SendBuffer_DMA(void *pBuffer, uint16_t len)
{
	return HAL_UART_Transmit_DMA(&Uart8Handle, (uint8_t*)pBuffer, len);
}
#endif /* UART8_TX_DMA_ENABLE */
#endif /* UART8_ENABLE */

/**
  * @brief UART GPIO Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
#if (UART7_ENABLE)
	if(huart->Instance == UART7) {
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		UART7_TX_GPIO_CLK_ENABLE(); /* Enable GPIO TX clock */
		UART7_RX_GPIO_CLK_ENABLE(); /* Enable GPIO RX clock */
		UART7_CLK_ENABLE(); /* Enable UART7 clock */
#if (UART7_TX_DMA_ENABLE | UART7_RX_DMA_ENABLE)
		UART7_DMA_CLK_ENABLE(); /* Enable DMA clock */
#endif /* UART7_TX_DMA_ENABLE | UART7_RX_DMA_ENABLE */
		/*##-2- Configure peripheral GPIO ##########################################*/
		/* UART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = UART7_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = UART7_TX_AF;
		HAL_GPIO_Init(UART7_TX_GPIO_PORT, &GPIO_InitStruct);
		/* UART RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = UART7_RX_PIN;
		GPIO_InitStruct.Alternate = UART7_RX_AF;
		HAL_GPIO_Init(UART7_RX_GPIO_PORT, &GPIO_InitStruct);
#if (UART7_TX_DMA_ENABLE)
		/*##-3- Configure the DMA ##################################################*/
		/* Configure the DMA handler for Transmission process */
		hdma_uart7_tx.Instance                 = UART7_TX_DMA_STREAM;
		hdma_uart7_tx.Init.Channel             = UART7_TX_DMA_CHANNEL;
		hdma_uart7_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		hdma_uart7_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_uart7_tx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_uart7_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_uart7_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_uart7_tx.Init.Mode                = DMA_NORMAL;
		hdma_uart7_tx.Init.Priority            = DMA_PRIORITY_LOW;
		HAL_DMA_Init(&hdma_uart7_tx);
		/* Associate the initialized DMA handle to the UART handle */
		__HAL_LINKDMA(huart, hdmatx, hdma_uart7_tx);
		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (UART7_TX) */
		HAL_NVIC_SetPriority(UART7_DMA_TX_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(UART7_DMA_TX_IRQn);
#endif /* UART7_TX_DMA_ENABLE */
#if (UART7_RX_DMA_ENABLE)
		/*##-3- Configure the DMA ##################################################*/
		/* Configure the DMA handler for Transmission process */
		hdma_uart7_rx.Instance                 = UART7_RX_DMA_STREAM;
		hdma_uart7_rx.Init.Channel             = UART7_RX_DMA_CHANNEL;
		hdma_uart7_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		hdma_uart7_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_uart7_rx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_uart7_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_uart7_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_uart7_rx.Init.Mode                = DMA_NORMAL;
		hdma_uart7_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_uart7_rx);
		/* Associate the initialized DMA handle to the UART handle */
		__HAL_LINKDMA(huart, hdmarx, hdma_uart7_rx);
		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (UART7_RX) */
		HAL_NVIC_SetPriority(UART7_DMA_RX_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(UART7_DMA_RX_IRQn);
#endif /* UART7_RX_DMA_ENABLE */
		/* NVIC for UART, to catch the TX complete */
		HAL_NVIC_SetPriority(UART7_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(UART7_IRQn);
	}
#endif /* UART7_ENABLE */
#if (UART8_ENABLE)
	if(huart->Instance == UART8) {
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		UART8_TX_GPIO_CLK_ENABLE(); /* Enable GPIO TX clock */
		UART8_RX_GPIO_CLK_ENABLE(); /* Enable GPIO RX clock */
		UART8_CLK_ENABLE(); /* Enable UART8 clock */
#if (UART8_TX_DMA_ENABLE | UART8_RX_DMA_ENABLE)
		UART8_DMA_CLK_ENABLE(); /* Enable DMA clock */
#endif /* UART8_TX_DMA_ENABLE | UART8_RX_DMA_ENABLE */
		/*##-2- Configure peripheral GPIO ##########################################*/
		/* UART TX GPIO pin configuration  */
		GPIO_InitStruct.Pin       = UART8_TX_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = UART8_TX_AF;
		HAL_GPIO_Init(UART8_TX_GPIO_PORT, &GPIO_InitStruct);
		/* UART RX GPIO pin configuration  */
		GPIO_InitStruct.Pin = UART8_RX_PIN;
		GPIO_InitStruct.Alternate = UART8_RX_AF;
		HAL_GPIO_Init(UART8_RX_GPIO_PORT, &GPIO_InitStruct);
#if (UART8_TX_DMA_ENABLE)
		/*##-3- Configure the DMA ##################################################*/
		/* Configure the DMA handler for Transmission process */
		hdma_uart8_tx.Instance                 = UART8_TX_DMA_STREAM;
		hdma_uart8_tx.Init.Channel             = UART8_TX_DMA_CHANNEL;
		hdma_uart8_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		hdma_uart8_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_uart8_tx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_uart8_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_uart8_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_uart8_tx.Init.Mode                = DMA_NORMAL;
		hdma_uart8_tx.Init.Priority            = DMA_PRIORITY_LOW;
		HAL_DMA_Init(&hdma_uart8_tx);
		/* Associate the initialized DMA handle to the UART handle */
		__HAL_LINKDMA(huart, hdmatx, hdma_uart8_tx);
		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (UART8_TX) */
		HAL_NVIC_SetPriority(UART8_DMA_TX_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(UART8_DMA_TX_IRQn);
#endif /* UART8_TX_DMA_ENABLE */
#if (UART8_RX_DMA_ENABLE)
		/*##-3- Configure the DMA ##################################################*/
		/* Configure the DMA handler for Transmission process */
		hdma_uart8_rx.Instance                 = UART8_RX_DMA_STREAM;
		hdma_uart8_rx.Init.Channel             = UART8_RX_DMA_CHANNEL;
		hdma_uart8_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		hdma_uart8_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_uart8_rx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_uart8_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_uart8_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_uart8_rx.Init.Mode                = DMA_NORMAL;
		hdma_uart8_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_uart8_rx);
		/* Associate the initialized DMA handle to the UART handle */
		__HAL_LINKDMA(huart, hdmarx, hdma_uart8_rx);
		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (UART8_RX) */
		HAL_NVIC_SetPriority(UART8_DMA_RX_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(UART8_DMA_RX_IRQn);
#endif /* UART8_RX_DMA_ENABLE */
		/* NVIC for UART, to catch the TX complete */
		HAL_NVIC_SetPriority(UART8_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(UART8_IRQn);
	}
#endif /* UART8_ENABLE */
}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
#if (UART7_ENABLE)
	if(huart->Instance == UART7) {
		/*##-1- Reset peripherals ##################################################*/
		UART7_FORCE_RESET();
		UART7_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks #################################*/
		/* Configure UART7 Tx as alternate function  */
		HAL_GPIO_DeInit(UART7_TX_GPIO_PORT, UART7_TX_PIN);
		/* Configure UART7 Rx as alternate function  */
		HAL_GPIO_DeInit(UART7_RX_GPIO_PORT, UART7_RX_PIN);
#if (UART7_TX_DMA_ENABLE)
		/*##-3- Disable the DMA #####################################################*/
		/* De-Initialize the DMA channel associated to transmission process */
		if(huart->hdmatx != 0) {
			HAL_DMA_DeInit(huart->hdmatx);
		}
		/*##-4- Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(UART7_DMA_TX_IRQn);
#endif /* UART7_TX_DMA_ENABLE */
#if (UART7_RX_DMA_ENABLE)
		/*##-3- Disable the DMA #####################################################*/
		/* De-Initialize the DMA channel associated to transmission process */
		if(huart->hdmarx != 0) {
			HAL_DMA_DeInit(huart->hdmarx);
		}
		/*##-4- Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(UART7_DMA_RX_IRQn);
#endif /* UART7_RX_DMA_ENABLE */
	}
#endif /* UART7_ENABLE */
#if (UART8_ENABLE)
	if(huart->Instance == UART8) {
		/*##-1- Reset peripherals ##################################################*/
		UART8_FORCE_RESET();
		UART8_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks #################################*/
		/* Configure UART8 Tx as alternate function  */
		HAL_GPIO_DeInit(UART8_TX_GPIO_PORT, UART8_TX_PIN);
		/* Configure UART8 Rx as alternate function  */
		HAL_GPIO_DeInit(UART8_RX_GPIO_PORT, UART8_RX_PIN);
#if (UART8_TX_DMA_ENABLE)
		/*##-3- Disable the DMA #####################################################*/
		/* De-Initialize the DMA channel associated to transmission process */
		if(huart->hdmatx != 0) {
			HAL_DMA_DeInit(huart->hdmatx);
		}
		/*##-4- Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(UART8_DMA_TX_IRQn);
#endif /* UART8_TX_DMA_ENABLE */
#if (UART8_RX_DMA_ENABLE)
		/*##-3- Disable the DMA #####################################################*/
		/* De-Initialize the DMA channel associated to transmission process */
		if(huart->hdmarx != 0) {
			HAL_DMA_DeInit(huart->hdmarx);
		}
		/*##-4- Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(UART8_DMA_RX_IRQn);
#endif /* UART8_RX_DMA_ENABLE */
	}
#endif /* UART8_ENABLE */
}

#if (UART7_ENABLE)
#if (UART7_TX_DMA_ENABLE)
/**
  * @brief  This function handles UART7 DMA interrupt request.
  * @param  None
  * @retval None
  */
void UART7_DMA_TX_IRQHandler(void)
{
	if(_uart7_init == 1)
		HAL_DMA_IRQHandler(Uart7Handle.hdmatx);
}
#endif /* UART7_TX_DMA_ENABLE */
#if (UART7_RX_DMA_ENABLE)
/**
  * @brief  This function handles UART7 DMA interrupt request.
  * @param  None
  * @retval None
  */
void UART7_DMA_RX_IRQHandler(void)
{
	if(_uart7_init == 1)
		HAL_DMA_IRQHandler(Uart7Handle.hdmarx);
}
#endif /* UART7_RX_DMA_ENABLE */
/**
  * @brief  This function handles UART7 interrupt request.
  * @param  None
  * @retval None
  */
void UART7_IRQHandler(void)
{
	if(_uart7_init == 1)
		HAL_UART_IRQHandler(&Uart7Handle);
}
#endif /* UART7_ENABLE */

#if (UART8_ENABLE)
#if (UART8_TX_DMA_ENABLE)
/**
  * @brief  This function handles UART8 DMA interrupt request.
  * @param  None
  * @retval None
  */
void UART8_DMA_TX_IRQHandler(void)
{
	if(_uart8_init == 1)
		HAL_DMA_IRQHandler(Uart8Handle.hdmatx);
}
#endif /* UART8_TX_DMA_ENABLE */
#if (UART8_RX_DMA_ENABLE)
/**
  * @brief  This function handles UART8 DMA interrupt request.
  * @param  None
  * @retval None
  */
void UART8_DMA_RX_IRQHandler(void)
{
	if(_uart8_init == 1)
		HAL_DMA_IRQHandler(Uart8Handle.hdmarx);
}
#endif /* UART8_RX_DMA_ENABLE */
/**
  * @brief  This function handles UART8 interrupt request.
  * @param  None
  * @retval None
  */
void UART8_IRQHandler(void)
{
	if(_uart8_init == 1)
		HAL_UART_IRQHandler(&Uart8Handle);
}
#endif /* UART8_ENABLE */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
