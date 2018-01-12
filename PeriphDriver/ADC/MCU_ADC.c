/**
  ******************************************************************************
  * @file    MCU_ADC.c
  * @author  kyChu
  * @version V0.1
  * @date    12-January-2018
  * @brief   MCU ADC Peripheral Driver.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "MCU_ADC.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#if (ADC1_ENABLE)
  /* ADC handler declaration */
  static ADC_HandleTypeDef Adc1Handle;
  static ADC_ChannelConfTypeDef sConfig;
  static DMA_HandleTypeDef hdma_adc1;
  /* Variable used to get converted value */
  static uint16_t uhADC1ConvertedValue = 0;
#endif /* ADC1_ENABLE */
/* Private function prototypes -----------------------------------------------*/
#if (ADC1_ENABLE)
  static HAL_StatusTypeDef ADC1_PeriphInit(void);
#endif /* ADC1_ENABLE */
/* Private functions ---------------------------------------------------------*/

/*
 * @brief  Initializes MCU ADCs.
 * @param  None
 * @retavl HAL Status.
 */
HAL_StatusTypeDef MCU_ADCs_Init(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
#if (ADC1_ENABLE)
	if((ret = ADC1_PeriphInit()) != HAL_OK) return ret;
#endif /* ADC1_ENABLE */
	return ret;
}

#if (ADC1_ENABLE)
/*
 * @brief  MCU ADC1 Initialize.
 * @param  None
 * @retval None
 */
static HAL_StatusTypeDef ADC1_PeriphInit(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
	/*##-1- Configure the ADC peripheral #######################################*/
	Adc1Handle.Instance                   = ADC1;

	Adc1Handle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
	Adc1Handle.Init.Resolution            = ADC_RESOLUTION_12B;
	Adc1Handle.Init.ScanConvMode          = DISABLE;                      /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
	Adc1Handle.Init.ContinuousConvMode    = ENABLE;                       /* Continuous mode enabled to have continuous conversion  */
	Adc1Handle.Init.DiscontinuousConvMode = DISABLE;                      /* Parameter discarded because sequencer is disabled */
	Adc1Handle.Init.NbrOfDiscConversion   = 0;
	Adc1Handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;/* Conversion start trigged at each external event */
	Adc1Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
	Adc1Handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	Adc1Handle.Init.NbrOfConversion       = 1;
	Adc1Handle.Init.DMAContinuousRequests = ENABLE;
	Adc1Handle.Init.EOCSelection          = DISABLE;
	if((ret = HAL_ADC_Init(&Adc1Handle)) != HAL_OK) return ret; /* ADC initialization Error */

	/*##-2- Configure ADC regular channel ######################################*/
	sConfig.Channel      = ADC1_CHANNEL;
	sConfig.Rank         = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	sConfig.Offset       = 0;
	if((ret = HAL_ADC_ConfigChannel(&Adc1Handle, &sConfig)) != HAL_OK) return ret; /* Channel Configuration Error */

	/*##-3- Start the conversion process #######################################*/
	if((ret = HAL_ADC_Start_DMA(&Adc1Handle, (uint32_t *)&uhADC1ConvertedValue, 1)) != HAL_OK) return ret; /* Start Conversation Error */
	return ret;
}
#endif /* ADC1_ENABLE */

#if (ADC1_ENABLE)
uint16_t *GetADC1ConvertedValue(void) { return &uhADC1ConvertedValue; }
#endif /* ADC1_ENABLE */

/**
  * @brief ADC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
#if (ADC1_ENABLE)
	if(hadc->Instance == ADC1) {
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* ADC1 Periph clock enable */
		ADC1_CLK_ENABLE();
		/* Enable GPIO clock ****************************************/
		ADC1_CHANNEL_GPIO_CLK_ENABLE();
		/* Enable DMA2 clock */
		ADC1_DMA_CLK_ENABLE();

		/*##-2- Configure peripheral GPIO ##########################################*/
		/* ADC Channel GPIO pin configuration */
		GPIO_InitStruct.Pin = ADC1_CHANNEL_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(ADC1_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

		/*##-3- Configure the DMA streams ##########################################*/
		/* Set the parameters to be configured */
		hdma_adc1.Instance = ADC1_DMA_STREAM;
		hdma_adc1.Init.Channel  = ADC1_DMA_CHANNEL;
		hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
		hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
		hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_adc1.Init.Mode = DMA_CIRCULAR;
		hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
		hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		hdma_adc1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
		hdma_adc1.Init.MemBurst = DMA_MBURST_SINGLE;
		hdma_adc1.Init.PeriphBurst = DMA_PBURST_SINGLE;

		HAL_DMA_Init(&hdma_adc1);

		/* Associate the initialized DMA handle to the ADC handle */
		__HAL_LINKDMA(hadc, DMA_Handle, hdma_adc1);
#if (ADC1_DMA_IRQ_ENABLE)
		/*##-4- Configure the NVIC for DMA #########################################*/
		/* NVIC configuration for DMA transfer complete interrupt */
		HAL_NVIC_SetPriority(ADC1_DMA_IRQn, 2, 0);
		HAL_NVIC_EnableIRQ(ADC1_DMA_IRQn);
#endif /* ADC1_DMA_IRQ_ENABLE */
	}
#endif /* ADC1_ENABLE */
}

/**
  * @brief ADC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
#if (ADC1_ENABLE)
	if(hadc->Instance == ADC1) {
		/*##-1- Reset peripherals ##################################################*/
		ADC1_FORCE_RESET();
		ADC1_RELEASE_RESET();

		/*##-2- Disable peripherals and GPIO Clocks ################################*/
		/* De-initialize the ADC Channel GPIO pin */
		HAL_GPIO_DeInit(ADC1_CHANNEL_GPIO_PORT, ADC1_CHANNEL_PIN);
	}
#endif /* ADC1_ENABLE */
}

#if (ADC1_ENABLE)
#if (ADC1_DMA_IRQ_ENABLE)
/**
* @brief  This function handles DMA interrupt request.
* @param  None
* @retval None
*/
void ADC1_DMA_IRQHandler(void)
{
	HAL_DMA_IRQHandler(Adc1Handle.DMA_Handle);
}
#endif /* ADC1_DMA_IRQ_ENABLE */
#endif /* ADC1_ENABLE */

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : AdcHandle handle
  * @note   This example shows a simple way to report end of conversion, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
#if (ADC1_ENABLE)
	if(AdcHandle->Instance == ADC1) {
#if (ADC1_DMA_IRQ_ENABLE)
		MCU_ADC1_ConvCpltCallback(AdcHandle);
#endif /* ADC1_DMA_IRQ_ENABLE */
	}
#endif /* ADC1_ENABLE */
}

#if (ADC1_ENABLE)
#if (ADC1_DMA_IRQ_ENABLE)
/**
  * @brief  Regular conversion complete callback in non blocking mode
  * @param  hadc: pointer to a ADC_HandleTypeDef structure that contains
  *         the configuration information for the specified ADC.
  * @retval None
  */
__weak void MCU_ADC1_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(hadc);
  /* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_ADC_ConvCpltCallback could be implemented in the user file
   */
}
#endif /* ADC1_DMA_IRQ_ENABLE */
#endif /* ADC1_ENABLE */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
