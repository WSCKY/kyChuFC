/**
  ******************************************************************************
  * @file    MCU_SPI.c
  * @author  kyChu
  * @version V0.1
  * @date    06-January-2018
  * @brief   MCU SPI Peripheral Driver.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "MCU_SPI.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if (SPI2_ENABLE)
static uint8_t _spi2_init = 0;
/* SPI handler declaration */
static SPI_HandleTypeDef Spi2Handle;
#if (SPI2_TX_DMA_ENABLE)
static DMA_HandleTypeDef hdma_spi2_tx;
#endif /* SPI2_TX_DMA_ENABLE */
#if (SPI2_RX_DMA_ENABLE)
static DMA_HandleTypeDef hdma_spi2_rx;
#endif /* SPI2_RX_DMA_ENABLE */
#endif /* SPI2_ENABLE */

#if (SPI4_ENABLE)
static uint8_t _spi4_init = 0;
/* SPI handler declaration */
static SPI_HandleTypeDef Spi4Handle;
#if (SPI4_TX_DMA_ENABLE)
static DMA_HandleTypeDef hdma_spi4_tx;
#endif /* SPI4_TX_DMA_ENABLE */
#if (SPI4_RX_DMA_ENABLE)
static DMA_HandleTypeDef hdma_spi4_rx;
#endif /* SPI4_RX_DMA_ENABLE */
#endif /* SPI4_ENABLE */
/* Private function prototypes -----------------------------------------------*/
#if (SPI2_ENABLE)
static HAL_StatusTypeDef SPI2_PeriphInit(void);
#endif /* SPI2_ENABLE */
#if (SPI4_ENABLE)
static HAL_StatusTypeDef SPI4_PeriphInit(void);
#endif /* SPI4_ENABLE */
/* Private functions ---------------------------------------------------------*/

/*
 * @brief  Initializes MCU SPIs.
 * @param  None
 * @retavl HAL Status.
 */
HAL_StatusTypeDef MCU_SPIs_Init(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
#if (SPI2_ENABLE)
	if((ret = SPI2_PeriphInit()) != HAL_OK) return ret;
	_spi2_init = 1;
#endif /* SPI2_ENABLE */
#if (SPI4_ENABLE)
	if((ret = SPI4_PeriphInit()) != HAL_OK) return ret;
	_spi4_init = 1;
#endif /* SPI4_ENABLE */
	return ret;
}

#if (SPI2_ENABLE)
/*
 * @brief  MCU SPI2 Initialize.
 * @param  None
 * @retval None
 */
static HAL_StatusTypeDef SPI2_PeriphInit(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
	/*##-1- Configure the SPI peripheral #######################################*/
	/* Set the SPI parameters */
	Spi2Handle.Instance               = SPI2;
	Spi2Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	Spi2Handle.Init.Direction         = SPI_DIRECTION_2LINES;
	Spi2Handle.Init.CLKPhase          = SPI_PHASE_2EDGE;
	Spi2Handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
	Spi2Handle.Init.DataSize          = SPI_DATASIZE_8BIT;
	Spi2Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	Spi2Handle.Init.TIMode            = SPI_TIMODE_DISABLE;
	Spi2Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	Spi2Handle.Init.CRCPolynomial     = 7;
	Spi2Handle.Init.NSS               = SPI_NSS_SOFT;
#if (SPI2_MODE_MASTER)
	Spi2Handle.Init.Mode              = SPI_MODE_MASTER;
#else
	Spi2Handle.Init.Mode              = SPI_MODE_SLAVE;
#endif /* MASTER_BOARD */

	if((ret = HAL_SPI_Init(&Spi2Handle)) != HAL_OK) return ret; /* Initialization Error */
	return ret;
}
#endif /* SPI2_ENABLE */

#if (SPI4_ENABLE)
/*
 * @brief  MCU SPI4 Initialize.
 * @param  None
 * @retval None
 */
static HAL_StatusTypeDef SPI4_PeriphInit(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
	/*##-1- Configure the SPI peripheral #######################################*/
	/* Set the SPI parameters */
	Spi4Handle.Instance               = SPI4;
	Spi4Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	Spi4Handle.Init.Direction         = SPI_DIRECTION_2LINES;
	Spi4Handle.Init.CLKPhase          = SPI_PHASE_2EDGE;
	Spi4Handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
	Spi4Handle.Init.DataSize          = SPI_DATASIZE_8BIT;
	Spi4Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	Spi4Handle.Init.TIMode            = SPI_TIMODE_DISABLE;
	Spi4Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	Spi4Handle.Init.CRCPolynomial     = 7;
	Spi4Handle.Init.NSS               = SPI_NSS_SOFT;
#if (SPI4_MODE_MASTER)
	Spi4Handle.Init.Mode              = SPI_MODE_MASTER;
#else
	Spi4Handle.Init.Mode              = SPI_MODE_SLAVE;
#endif /* MASTER_BOARD */

	if((ret = HAL_SPI_Init(&Spi4Handle)) != HAL_OK) return ret; /* Initialization Error */
	return ret;
}
#endif /* SPI4_ENABLE */

#if (SPI2_ENABLE)
#if (SPI2_TX_DMA_ENABLE & SPI2_RX_DMA_ENABLE)
/**
  * @brief  Transmit and Receive an amount of data in non-blocking mode with DMA.
  * @param  pTxData: pointer to transmission data buffer
  * @param  pRxData: pointer to reception data buffer
  * @note   When the CRC feature is enabled the pRxData Length must be Size + 1
  * @param  Size: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef SPI2_TxRxData_DMA(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
	return HAL_SPI_TransmitReceive_DMA(&Spi2Handle, pTxData, pRxData, Size);
}
#endif /* SPI2_TX_DMA_ENABLE & SPI2_RX_DMA_ENABLE */
#endif /* SPI2_ENABLE */
#if (SPI4_ENABLE)
#if (SPI4_TX_DMA_ENABLE & SPI4_RX_DMA_ENABLE)
/**
  * @brief  Transmit and Receive an amount of data in non-blocking mode with DMA.
  * @param  pTxData: pointer to transmission data buffer
  * @param  pRxData: pointer to reception data buffer
  * @note   When the CRC feature is enabled the pRxData Length must be Size + 1
  * @param  Size: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef SPI4_TxRxData_DMA(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
	return HAL_SPI_TransmitReceive_DMA(&Spi4Handle, pTxData, pRxData, Size);
}
#endif /* SPI4_TX_DMA_ENABLE & SPI2_RX_DMA_ENABLE */
#endif /* SPI4_ENABLE */

/**
  * @brief SPI MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  *           - NVIC configuration for SPI interrupt request enable
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
#if (SPI2_ENABLE)
	if (hspi->Instance == SPI2) {
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		SPI2_SCK_GPIO_CLK_ENABLE(); /* Enable GPIO SCK clock */
		SPI2_MISO_GPIO_CLK_ENABLE(); /* Enable GPIO MISO clock */
		SPI2_MOSI_GPIO_CLK_ENABLE(); /* Enable GPIO MOSI clock */
		SPI2_CLK_ENABLE(); /* Enable SPI2 clock */
#if (SPI2_TX_DMA_ENABLE | SPI2_RX_DMA_ENABLE)
		SPI2_DMA_CLK_ENABLE(); /* Enable DMA clock */
#endif /* SPI2_TX_DMA_ENABLE | SPI2_RX_DMA_ENABLE */
		/*##-2- Configure peripheral GPIO ##########################################*/
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStruct.Pin       = SPI2_SCK_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = SPI2_SCK_AF;
		HAL_GPIO_Init(SPI2_SCK_GPIO_PORT, &GPIO_InitStruct);
		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI2_MISO_PIN;
		GPIO_InitStruct.Alternate = SPI2_MISO_AF;
		HAL_GPIO_Init(SPI2_MISO_GPIO_PORT, &GPIO_InitStruct);
		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI2_MOSI_PIN;
		GPIO_InitStruct.Alternate = SPI2_MOSI_AF;
		HAL_GPIO_Init(SPI2_MOSI_GPIO_PORT, &GPIO_InitStruct);
#if (SPI2_TX_DMA_ENABLE)
		/*##-3- Configure the DMA ##################################################*/
		/* Configure the DMA handler for Transmission process */
		hdma_spi2_tx.Instance                 = SPI2_TX_DMA_STREAM;
		hdma_spi2_tx.Init.Channel             = SPI2_TX_DMA_CHANNEL;
		hdma_spi2_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		hdma_spi2_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		hdma_spi2_tx.Init.MemBurst            = DMA_MBURST_INC4;
		hdma_spi2_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
		hdma_spi2_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		hdma_spi2_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_spi2_tx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_spi2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_spi2_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_spi2_tx.Init.Mode                = DMA_NORMAL;
		hdma_spi2_tx.Init.Priority            = DMA_PRIORITY_LOW;
		HAL_DMA_Init(&hdma_spi2_tx);
		/* Associate the initialized DMA handle to the the SPI handle */
		__HAL_LINKDMA(hspi, hdmatx, hdma_spi2_tx);
		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (SPI2_TX) */
		HAL_NVIC_SetPriority(SPI2_DMA_TX_IRQn, 1, 1);
		HAL_NVIC_EnableIRQ(SPI2_DMA_TX_IRQn);
#endif /* SPI2_TX_DMA_ENABLE */
#if (SPI2_RX_DMA_ENABLE)
		/* Configure the DMA handler for Transmission process */
		hdma_spi2_rx.Instance                 = SPI2_RX_DMA_STREAM;
		hdma_spi2_rx.Init.Channel             = SPI2_RX_DMA_CHANNEL;
		hdma_spi2_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		hdma_spi2_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		hdma_spi2_rx.Init.MemBurst            = DMA_MBURST_INC4;
		hdma_spi2_rx.Init.PeriphBurst         = DMA_PBURST_INC4;
		hdma_spi2_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		hdma_spi2_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_spi2_rx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_spi2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_spi2_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_spi2_rx.Init.Mode                = DMA_NORMAL;
		hdma_spi2_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_spi2_rx);
		/* Associate the initialized DMA handle to the the SPI handle */
		__HAL_LINKDMA(hspi, hdmarx, hdma_spi2_rx);
		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (SPI2_RX) */
		HAL_NVIC_SetPriority(SPI2_DMA_RX_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(SPI2_DMA_RX_IRQn);
#endif /* SPI2_RX_DMA_ENABLE */
		/*##-5- Configure the NVIC for SPI #########################################*/
		/* NVIC for SPI */
		HAL_NVIC_SetPriority(SPI2_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(SPI2_IRQn);
	}
#endif /* SPI2_ENABLE */
#if (SPI4_ENABLE)
	if (hspi->Instance == SPI4) {
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		SPI4_SCK_GPIO_CLK_ENABLE(); /* Enable GPIO SCK clock */
		SPI4_MISO_GPIO_CLK_ENABLE(); /* Enable GPIO MISO clock */
		SPI4_MOSI_GPIO_CLK_ENABLE(); /* Enable GPIO MOSI clock */
		SPI4_CLK_ENABLE(); /* Enable SPI4 clock */
#if (SPI4_TX_DMA_ENABLE | SPI4_RX_DMA_ENABLE)
		SPI4_DMA_CLK_ENABLE(); /* Enable DMA clock */
#endif /* SPI4_TX_DMA_ENABLE | SPI4_RX_DMA_ENABLE */
		/*##-2- Configure peripheral GPIO ##########################################*/
		/* SPI SCK GPIO pin configuration  */
		GPIO_InitStruct.Pin       = SPI4_SCK_PIN;
		GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull      = GPIO_PULLUP;
		GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = SPI4_SCK_AF;
		HAL_GPIO_Init(SPI4_SCK_GPIO_PORT, &GPIO_InitStruct);
		/* SPI MISO GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI4_MISO_PIN;
		GPIO_InitStruct.Alternate = SPI4_MISO_AF;
		HAL_GPIO_Init(SPI4_MISO_GPIO_PORT, &GPIO_InitStruct);
		/* SPI MOSI GPIO pin configuration  */
		GPIO_InitStruct.Pin = SPI4_MOSI_PIN;
		GPIO_InitStruct.Alternate = SPI4_MOSI_AF;
		HAL_GPIO_Init(SPI4_MOSI_GPIO_PORT, &GPIO_InitStruct);
#if (SPI4_TX_DMA_ENABLE)
		/*##-3- Configure the DMA ##################################################*/
		/* Configure the DMA handler for Transmission process */
		hdma_spi4_tx.Instance                 = SPI4_TX_DMA_STREAM;
		hdma_spi4_tx.Init.Channel             = SPI4_TX_DMA_CHANNEL;
		hdma_spi4_tx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		hdma_spi4_tx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		hdma_spi4_tx.Init.MemBurst            = DMA_MBURST_INC4;
		hdma_spi4_tx.Init.PeriphBurst         = DMA_PBURST_INC4;
		hdma_spi4_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		hdma_spi4_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_spi4_tx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_spi4_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_spi4_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_spi4_tx.Init.Mode                = DMA_NORMAL;
		hdma_spi4_tx.Init.Priority            = DMA_PRIORITY_LOW;
		HAL_DMA_Init(&hdma_spi4_tx);
		/* Associate the initialized DMA handle to the the SPI handle */
		__HAL_LINKDMA(hspi, hdmatx, hdma_spi4_tx);
		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (SPI4_TX) */
		HAL_NVIC_SetPriority(SPI4_DMA_TX_IRQn, 1, 1);
		HAL_NVIC_EnableIRQ(SPI4_DMA_TX_IRQn);
#endif /* SPI4_TX_DMA_ENABLE */
#if (SPI4_RX_DMA_ENABLE)
		/* Configure the DMA handler for Transmission process */
		hdma_spi4_rx.Instance                 = SPI4_RX_DMA_STREAM;
		hdma_spi4_rx.Init.Channel             = SPI4_RX_DMA_CHANNEL;
		hdma_spi4_rx.Init.FIFOMode            = DMA_FIFOMODE_DISABLE;
		hdma_spi4_rx.Init.FIFOThreshold       = DMA_FIFO_THRESHOLD_FULL;
		hdma_spi4_rx.Init.MemBurst            = DMA_MBURST_INC4;
		hdma_spi4_rx.Init.PeriphBurst         = DMA_PBURST_INC4;
		hdma_spi4_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		hdma_spi4_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		hdma_spi4_rx.Init.MemInc              = DMA_MINC_ENABLE;
		hdma_spi4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		hdma_spi4_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		hdma_spi4_rx.Init.Mode                = DMA_NORMAL;
		hdma_spi4_rx.Init.Priority            = DMA_PRIORITY_HIGH;
		HAL_DMA_Init(&hdma_spi4_rx);
		/* Associate the initialized DMA handle to the the SPI handle */
		__HAL_LINKDMA(hspi, hdmarx, hdma_spi4_rx);
		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt (SPI4_RX) */
		HAL_NVIC_SetPriority(SPI4_DMA_RX_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(SPI4_DMA_RX_IRQn);
#endif /* SPI4_RX_DMA_ENABLE */
		/*##-5- Configure the NVIC for SPI #########################################*/
		/* NVIC for SPI */
		HAL_NVIC_SetPriority(SPI4_IRQn, 1, 0);
		HAL_NVIC_EnableIRQ(SPI4_IRQn);
	}
#endif /* SPI4_ENABLE */
}

/**
  * @brief SPI MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hspi: SPI handle pointer
  * @retval None
  */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
#if (SPI2_ENABLE)
	if(hspi->Instance == SPI2) {
		/*##-1- Reset peripherals ##################################################*/
		SPI2_FORCE_RESET();
		SPI2_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks ################################*/
		/* Deconfigure SPI SCK */
		HAL_GPIO_DeInit(SPI2_SCK_GPIO_PORT, SPI2_SCK_PIN);
		/* Deconfigure SPI MISO */
		HAL_GPIO_DeInit(SPI2_MISO_GPIO_PORT, SPI2_MISO_PIN);
		/* Deconfigure SPI MOSI */
		HAL_GPIO_DeInit(SPI2_MOSI_GPIO_PORT, SPI2_MOSI_PIN);
		/*##-3- Disable the DMA ####################################################*/
#if (SPI2_TX_DMA_ENABLE)
		/* De-Initialize the DMA associated to transmission process */
		HAL_DMA_DeInit(&hdma_spi2_tx);
		/*##-4- Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(SPI2_DMA_TX_IRQn);
#endif /* SPI2_TX_DMA_ENABLE */
#if (SPI2_RX_DMA_ENABLE)
		/* De-Initialize the DMA associated to reception process */
		HAL_DMA_DeInit(&hdma_spi2_rx);
		/*##-4- Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(SPI2_DMA_RX_IRQn);
#endif /* SPI2_RX_DMA_ENABLE */
	}
#endif /* SPI2_ENABLE */
#if (SPI4_ENABLE)
	if(hspi->Instance == SPI4) {
		/*##-1- Reset peripherals ##################################################*/
		SPI4_FORCE_RESET();
		SPI4_RELEASE_RESET();
		/*##-2- Disable peripherals and GPIO Clocks ################################*/
		/* Deconfigure SPI SCK */
		HAL_GPIO_DeInit(SPI4_SCK_GPIO_PORT, SPI4_SCK_PIN);
		/* Deconfigure SPI MISO */
		HAL_GPIO_DeInit(SPI4_MISO_GPIO_PORT, SPI4_MISO_PIN);
		/* Deconfigure SPI MOSI */
		HAL_GPIO_DeInit(SPI4_MOSI_GPIO_PORT, SPI4_MOSI_PIN);
		/*##-3- Disable the DMA ####################################################*/
#if (SPI4_TX_DMA_ENABLE)
		/* De-Initialize the DMA associated to transmission process */
		HAL_DMA_DeInit(&hdma_spi4_tx);
		/*##-4- Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(SPI4_DMA_TX_IRQn);
#endif /* SPI4_TX_DMA_ENABLE */
#if (SPI4_RX_DMA_ENABLE)
		/* De-Initialize the DMA associated to reception process */
		HAL_DMA_DeInit(&hdma_spi4_rx);
		/*##-4- Disable the NVIC for DMA ###########################################*/
		HAL_NVIC_DisableIRQ(SPI4_DMA_RX_IRQn);
#endif /* SPI4_RX_DMA_ENABLE */
	}
#endif /* SPI4_ENABLE */
}

#if (SPI2_ENABLE)
#if (SPI2_RX_DMA_ENABLE)
/**
  * @brief  This function handles DMA Rx interrupt request.
  * @param  None
  * @retval None
  */
void SPI2_DMA_RX_IRQHandler(void)
{
	if(_spi2_init == 1)
		HAL_DMA_IRQHandler(Spi2Handle.hdmarx);
}
#endif /* SPI2_RX_DMA_ENABLE */
#if (SPI2_TX_DMA_ENABLE)
/**
  * @brief  This function handles DMA Tx interrupt request.
  * @param  None
  * @retval None
  */
void SPI2_DMA_TX_IRQHandler(void)
{
	if(_spi2_init == 1)
		HAL_DMA_IRQHandler(Spi2Handle.hdmatx);
}
#endif /* SPI2_TX_DMA_ENABLE */
/**
  * @brief  This function handles SPI interrupt request.
  * @param  None
  * @retval None
  */
void SPI2_IRQHandler(void)
{
	if(_spi2_init == 1)
		HAL_SPI_IRQHandler(&Spi2Handle);
}
#endif /* SPI2_ENABLE */
#if (SPI4_ENABLE)
#if (SPI4_RX_DMA_ENABLE)
/**
  * @brief  This function handles DMA Rx interrupt request.
  * @param  None
  * @retval None
  */
void SPI4_DMA_RX_IRQHandler(void)
{
	if(_spi4_init == 1)
		HAL_DMA_IRQHandler(Spi4Handle.hdmarx);
}
#endif /* SPI4_RX_DMA_ENABLE */
#if (SPI4_TX_DMA_ENABLE)
/**
  * @brief  This function handles DMA Tx interrupt request.
  * @param  None
  * @retval None
  */
void SPI4_DMA_TX_IRQHandler(void)
{
	if(_spi4_init == 1)
		HAL_DMA_IRQHandler(Spi4Handle.hdmatx);
}
#endif /* SPI4_TX_DMA_ENABLE */
/**
  * @brief  This function handles SPI interrupt request.
  * @param  None
  * @retval None
  */
void SPI4_IRQHandler(void)
{
	if(_spi4_init == 1)
		HAL_SPI_IRQHandler(&Spi4Handle);
}
#endif /* SPI4_ENABLE */

/**
  * @brief  TxRx Transfer completed callback.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report end of DMA TxRx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
#if (SPI2_ENABLE)
	if (hspi->Instance == SPI2) {
		MCU_SPI2_TxRxCpltCallback(hspi);
	}
#endif /* SPI2_ENABLE */
#if (SPI4_ENABLE)
	if (hspi->Instance == SPI4) {
		MCU_SPI4_TxRxCpltCallback(hspi);
	}
#endif /* SPI4_ENABLE */
}

/**
  * @brief  SPI error callbacks.
  * @param  hspi: SPI handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
#if (SPI2_ENABLE)
	if (hspi->Instance == SPI2) {
		MCU_SPI2_ErrorCallback(hspi);
	}
#endif /* SPI2_ENABLE */
#if (SPI4_ENABLE)
	if (hspi->Instance == SPI4) {
		MCU_SPI4_ErrorCallback(hspi);
	}
#endif /* SPI4_ENABLE */
}

#if (SPI2_ENABLE)
/**
  * @brief Tx and Rx Transfer completed callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void MCU_SPI2_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxRxCpltCallback should be implemented in the user file
   */
}
/**
  * @brief SPI error callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void MCU_SPI2_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_ErrorCallback should be implemented in the user file
   */
  /* NOTE : The ErrorCode parameter in the hspi handle is updated by the SPI processes
            and user can use HAL_SPI_GetError() API to check the latest error occurred
   */
}
#endif /* SPI2_ENABLE */

#if (SPI4_ENABLE)
/**
  * @brief Tx and Rx Transfer completed callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void MCU_SPI4_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_TxRxCpltCallback should be implemented in the user file
   */
}
/**
  * @brief SPI error callback.
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
__weak void MCU_SPI4_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hspi);

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_SPI_ErrorCallback should be implemented in the user file
   */
  /* NOTE : The ErrorCode parameter in the hspi handle is updated by the SPI processes
            and user can use HAL_SPI_GetError() API to check the latest error occurred
   */
}
#endif /* SPI4_ENABLE */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
