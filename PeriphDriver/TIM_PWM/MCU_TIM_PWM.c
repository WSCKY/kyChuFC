/**
  ******************************************************************************
  * @file    MCU_TIM_PWM.c
  * @author  kyChu
  * @version V0.1
  * @date    05-January-2018
  * @brief   MCU Timer PWM Driver.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "MCU_TIM_PWM.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Timer handler declaration */
#if (TIM2_ENABLE)
  static TIM_HandleTypeDef    Tim2Handle;
#if (TIM2_PWM_OUTPUT_ENABLE & (TIM2_PWM_CHANNEL1_ENABLE | TIM2_PWM_CHANNEL2_ENABLE | TIM2_PWM_CHANNEL3_ENABLE | TIM2_PWM_CHANNEL4_ENABLE))
  /* Timer Output Compare Configuration Structure declaration */
  static TIM_OC_InitTypeDef sConfig_Tim2;
#endif /* TIM2_PWM_OUTPUT_ENABLE & (TIM2_PWM_CHANNEL1_ENABLE | TIM2_PWM_CHANNEL2_ENABLE | TIM2_PWM_CHANNEL3_ENABLE | TIM2_PWM_CHANNEL4_ENABLE) */
#endif /* TIM2_ENABLE */
#if (TIM5_ENABLE)
  static TIM_HandleTypeDef    Tim5Handle;
#if (TIM5_PWM_OUTPUT_ENABLE & (TIM5_PWM_CHANNEL1_ENABLE | TIM5_PWM_CHANNEL2_ENABLE | TIM5_PWM_CHANNEL3_ENABLE | TIM5_PWM_CHANNEL4_ENABLE))
  /* Timer Output Compare Configuration Structure declaration */
  static TIM_OC_InitTypeDef sConfig_Tim5;
#endif /* TIM5_PWM_OUTPUT_ENABLE & (TIM5_PWM_CHANNEL1_ENABLE | TIM5_PWM_CHANNEL2_ENABLE | TIM5_PWM_CHANNEL3_ENABLE | TIM5_PWM_CHANNEL4_ENABLE) */
#endif /* TIM5_ENABLE */
/* Private function prototypes -----------------------------------------------*/
#if (TIM2_ENABLE)
  static HAL_StatusTypeDef TIM2_PeriphInit(void);
#endif /* TIM2_ENABLE */
#if (TIM5_ENABLE)
  static HAL_StatusTypeDef TIM5_PeriphInit(void);
#endif /* TIM5_ENABLE */
/* Private functions ---------------------------------------------------------*/

/*
 * @brief  Initializes MCU TIMs.
 * @param  None
 * @retval HAL Status.
 */
HAL_StatusTypeDef MCU_TIMs_Init(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
#if (TIM2_ENABLE)
	ret = TIM2_PeriphInit(); if(ret != HAL_OK) return ret;
#endif /* TIM2_ENABLE */
#if (TIM5_ENABLE)
	ret = TIM5_PeriphInit(); if(ret != HAL_OK) return ret;
#endif /* TIM5_ENABLE */
	return ret;
}

#if (TIM2_ENABLE)
#if (TIM2_PWM_OUTPUT_ENABLE)
/*
 * @brief  set TIM2 CCR Value.
 * @param  CCR Value.
 * @retval None
 */
void TIM2_PWM_SetPulse(uint16_t CCR1, uint16_t CCR2, uint16_t CCR3, uint16_t CCR4)
{
#if(TIM2_PWM_CHANNEL1_ENABLE)
	TIM2->CCR1 = CCR1;
#endif /* TIM2_PWM_CHANNEL1_ENABLE */
#if(TIM2_PWM_CHANNEL2_ENABLE)
	TIM2->CCR2 = CCR2;
#endif /* TIM2_PWM_CHANNEL2_ENABLE */
#if(TIM2_PWM_CHANNEL3_ENABLE)
	TIM2->CCR3 = CCR3;
#endif /* TIM2_PWM_CHANNEL3_ENABLE */
#if(TIM2_PWM_CHANNEL4_ENABLE)
	TIM2->CCR4 = CCR4;
#endif /* TIM2_PWM_CHANNEL4_ENABLE */
}
#endif /* TIM2_PWM_OUTPUT_ENABLE */
#endif /* TIM2_ENABLE */
#if (TIM5_ENABLE)
#if (TIM5_PWM_OUTPUT_ENABLE)
/*
 * @brief  set TIM5 CCR Value.
 * @param  CCR Value.
 * @retval None
 */
void TIM5_PWM_SetPulse(uint16_t CCR1, uint16_t CCR2, uint16_t CCR3, uint16_t CCR4)
{
#if(TIM5_PWM_CHANNEL1_ENABLE)
	TIM5->CCR1 = CCR1;
#endif /* TIM5_PWM_CHANNEL1_ENABLE */
#if(TIM5_PWM_CHANNEL2_ENABLE)
	TIM5->CCR2 = CCR2;
#endif /* TIM5_PWM_CHANNEL2_ENABLE */
#if(TIM5_PWM_CHANNEL3_ENABLE)
	TIM5->CCR3 = CCR3;
#endif /* TIM5_PWM_CHANNEL3_ENABLE */
#if(TIM5_PWM_CHANNEL4_ENABLE)
	TIM5->CCR4 = CCR4;
#endif /* TIM5_PWM_CHANNEL4_ENABLE */
}
#endif /* TIM5_PWM_OUTPUT_ENABLE */
#endif /* TIM5_ENABLE */

#if (TIM2_ENABLE)
/*
 * @brief  MCU TIM2 Peripheral Initialize.
 * @param  None
 * @retval None
 */
static HAL_StatusTypeDef TIM2_PeriphInit(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
	uint32_t uhPrescalerValue = 0;
	/* Compute the prescaler value to have TIM2 counter clock equal to TIM2_COUNT_CLOCK_RATE Hz */
	uhPrescalerValue = (uint32_t)((SystemCoreClock /2) / TIM2_COUNT_CLOCK_RATE) - 1;

	/*##-1- Configure the TIM peripheral #######################################*/
	/* -----------------------------------------------------------------------
		TIM2 Configuration: generate 4 PWM signals with 4 different duty cycles.

		In this example TIM2 input clock (TIM2CLK) is set to APB1 clock x 2,
		since APB1 prescaler is equal to 2.
	      TIM2CLK = APB1CLK*2
	      APB1CLK = HCLK/4
	      => TIM2CLK = HCLK = SystemCoreClock / 2

	    To get TIM2 counter clock at TIM2_COUNT_CLOCK_RATE Hz, the prescaler is computed as follows:
	       Prescaler = (TIM2CLK / TIM2 counter clock) - 1
	       Prescaler = ((SystemCoreClock / 2) /TIM2_COUNT_CLOCK_RATE Hz) - 1

	    To get TIM2 output clock at TIM2_OUTPUT_CLOCK_RATE Hz, the period (ARR)) is computed as follows:
	       ARR = (TIM2 counter clock / TIM2 output clock) - 1

	    TIM2 Channel1 duty cycle = (TIM2_CCR1/ TIM2_ARR + 1)* 100
	    TIM2 Channel2 duty cycle = (TIM2_CCR2/ TIM2_ARR + 1)* 100
	    TIM2 Channel3 duty cycle = (TIM2_CCR3/ TIM2_ARR + 1)* 100
	    TIM2 Channel4 duty cycle = (TIM2_CCR4/ TIM2_ARR + 1)* 100

	    Note:
	     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f7xx.c file.
	     Each time the core clock (HCLK) changes, user had to update SystemCoreClock
	     variable value. Otherwise, any configuration based on this variable will be incorrect.
	     This variable is updated in three ways:
	      1) by calling CMSIS function SystemCoreClockUpdate()
	      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
	      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
	  ----------------------------------------------------------------------- */

	/* Initialize TIM2 peripheral as follows:
	   + Prescaler = ((SystemCoreClock / 2) / TIM2_COUNT_CLOCK_RATE) - 1
	   + Period = (TIM2 counter clock / TIM2 output clock) - 1)
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	Tim2Handle.Instance = TIM2;

	Tim2Handle.Init.Prescaler         = uhPrescalerValue;
	Tim2Handle.Init.Period            = (TIM2_COUNT_CLOCK_RATE / TIM2_OUTPUT_CLOCK_RATE) - 1;
	Tim2Handle.Init.ClockDivision     = 0;
	Tim2Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	Tim2Handle.Init.RepetitionCounter = 0;
	Tim2Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if ((ret = HAL_TIM_PWM_Init(&Tim2Handle)) != HAL_OK) return ret; /* Initialization Error */
#if (TIM2_PWM_OUTPUT_ENABLE & (TIM2_PWM_CHANNEL1_ENABLE | TIM2_PWM_CHANNEL2_ENABLE | TIM2_PWM_CHANNEL3_ENABLE | TIM2_PWM_CHANNEL4_ENABLE))
	/*##-2- Configure the PWM channels #########################################*/
	/* Common configuration for all channels */
	sConfig_Tim2.OCMode       = TIM_OCMODE_PWM1;
	sConfig_Tim2.OCPolarity   = TIM_OCPOLARITY_HIGH;
	sConfig_Tim2.OCFastMode   = TIM_OCFAST_DISABLE;
	sConfig_Tim2.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
	sConfig_Tim2.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	sConfig_Tim2.OCIdleState  = TIM_OCIDLESTATE_RESET;
#if(TIM2_PWM_CHANNEL1_ENABLE)
	sConfig_Tim2.Pulse = TIM2_PWM_DEFAUTL_PULSE_1; /* Set the pulse value for channel 1 */
	if ((ret = HAL_TIM_PWM_ConfigChannel(&Tim2Handle, &sConfig_Tim2, TIM_CHANNEL_1)) != HAL_OK) return ret; /* Configuration Error */
#endif /* TIM2_PWM_CHANNEL1_ENABLE */
#if(TIM2_PWM_CHANNEL2_ENABLE)
	sConfig_Tim2.Pulse = TIM2_PWM_DEFAUTL_PULSE_1; /* Set the pulse value for channel 2 */
	if ((ret = HAL_TIM_PWM_ConfigChannel(&Tim2Handle, &sConfig_Tim2, TIM_CHANNEL_2)) != HAL_OK) return ret;
#endif /* TIM2_PWM_CHANNEL2_ENABLE */
#if(TIM2_PWM_CHANNEL3_ENABLE)
	sConfig_Tim2.Pulse = TIM2_PWM_DEFAUTL_PULSE_1; /* Set the pulse value for channel 3 */
	if ((ret = HAL_TIM_PWM_ConfigChannel(&Tim2Handle, &sConfig_Tim2, TIM_CHANNEL_3)) != HAL_OK) return ret;
#endif /* TIM2_PWM_CHANNEL3_ENABLE */
#if(TIM2_PWM_CHANNEL4_ENABLE)
	sConfig_Tim2.Pulse = TIM2_PWM_DEFAUTL_PULSE_1; /* Set the pulse value for channel 4 */
	if ((ret = HAL_TIM_PWM_ConfigChannel(&Tim2Handle, &sConfig_Tim2, TIM_CHANNEL_4)) != HAL_OK) return ret;
#endif /* TIM2_PWM_CHANNEL4_ENABLE */
	/*##-3- Start PWM signals generation #######################################*/
#if(TIM2_PWM_CHANNEL1_ENABLE) /* Start channel 1 */
	if ((ret = HAL_TIM_PWM_Start(&Tim2Handle, TIM_CHANNEL_1)) != HAL_OK) return ret; /* PWM Generation Error */
#endif /* TIM2_PWM_CHANNEL1_ENABLE */
#if(TIM2_PWM_CHANNEL2_ENABLE) /* Start channel 2 */
	if ((ret = HAL_TIM_PWM_Start(&Tim2Handle, TIM_CHANNEL_2)) != HAL_OK) return ret;
#endif /* TIM2_PWM_CHANNEL2_ENABLE */
#if(TIM2_PWM_CHANNEL3_ENABLE) /* Start channel 3 */
	if ((ret = HAL_TIM_PWM_Start(&Tim2Handle, TIM_CHANNEL_3)) != HAL_OK) return ret;
#endif /* TIM2_PWM_CHANNEL3_ENABLE */
#if(TIM2_PWM_CHANNEL4_ENABLE) /* Start channel 4 */
	if ((ret = HAL_TIM_PWM_Start(&Tim2Handle, TIM_CHANNEL_4)) != HAL_OK) return ret;
#endif /* TIM2_PWM_CHANNEL4_ENABLE */
#endif /* TIM2_PWM_OUTPUT_ENABLE & (TIM2_PWM_CHANNEL1_ENABLE | TIM2_PWM_CHANNEL2_ENABLE | TIM2_PWM_CHANNEL3_ENABLE | TIM2_PWM_CHANNEL4_ENABLE) */
	return ret;
}
#endif /* TIM2_ENABLE */

#if (TIM5_ENABLE)
/*
 * @brief  MCU TIM5 Peripheral Initialize.
 * @param  None
 * @retval None
 */
static HAL_StatusTypeDef TIM5_PeriphInit(void)
{
	HAL_StatusTypeDef ret = HAL_OK;
	uint32_t uhPrescalerValue = 0;
	/* Compute the prescaler value to have TIM5 counter clock equal to TIM5_COUNT_CLOCK_RATE Hz */
	uhPrescalerValue = (uint32_t)((SystemCoreClock /2) / TIM5_COUNT_CLOCK_RATE) - 1;

	/*##-1- Configure the TIM peripheral #######################################*/
	/* -----------------------------------------------------------------------
		TIM5 Configuration: generate 4 PWM signals with 4 different duty cycles.

		In this example TIM5 input clock (TIM5CLK) is set to APB1 clock x 2,
		since APB1 prescaler is equal to 2.
	      TIM5CLK = APB1CLK*2
	      APB1CLK = HCLK/4
	      => TIM5CLK = HCLK = SystemCoreClock / 2

	    To get TIM5 counter clock at TIM5_COUNT_CLOCK_RATE Hz, the prescaler is computed as follows:
	       Prescaler = (TIM5CLK / TIM5 counter clock) - 1
	       Prescaler = ((SystemCoreClock / 2) /TIM5_COUNT_CLOCK_RATE Hz) - 1

	    To get TIM5 output clock at TIM5_OUTPUT_CLOCK_RATE Hz, the period (ARR)) is computed as follows:
	       ARR = (TIM5 counter clock / TIM5 output clock) - 1

	    TIM5 Channel1 duty cycle = (TIM5_CCR1/ TIM5_ARR + 1)* 100
	    TIM5 Channel2 duty cycle = (TIM5_CCR2/ TIM5_ARR + 1)* 100
	    TIM5 Channel3 duty cycle = (TIM5_CCR3/ TIM5_ARR + 1)* 100
	    TIM5 Channel4 duty cycle = (TIM5_CCR4/ TIM5_ARR + 1)* 100

	    Note:
	     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f7xx.c file.
	     Each time the core clock (HCLK) changes, user had to update SystemCoreClock
	     variable value. Otherwise, any configuration based on this variable will be incorrect.
	     This variable is updated in three ways:
	      1) by calling CMSIS function SystemCoreClockUpdate()
	      2) by calling HAL API function HAL_RCC_GetSysClockFreq()
	      3) each time HAL_RCC_ClockConfig() is called to configure the system clock frequency
	  ----------------------------------------------------------------------- */

	/* Initialize TIM5 peripheral as follows:
	   + Prescaler = ((SystemCoreClock / 2) / TIM5_COUNT_CLOCK_RATE) - 1
	   + Period = (TIM5 counter clock / TIM5 output clock) - 1)
	   + ClockDivision = 0
	   + Counter direction = Up
	*/
	Tim5Handle.Instance = TIM5;

	Tim5Handle.Init.Prescaler         = uhPrescalerValue;
	Tim5Handle.Init.Period            = (TIM5_COUNT_CLOCK_RATE / TIM5_OUTPUT_CLOCK_RATE) - 1;
	Tim5Handle.Init.ClockDivision     = 0;
	Tim5Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	Tim5Handle.Init.RepetitionCounter = 0;
	Tim5Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if ((ret = HAL_TIM_PWM_Init(&Tim5Handle)) != HAL_OK) return ret; /* Initialization Error */
#if (TIM5_PWM_OUTPUT_ENABLE & (TIM5_PWM_CHANNEL1_ENABLE | TIM5_PWM_CHANNEL2_ENABLE | TIM5_PWM_CHANNEL3_ENABLE | TIM5_PWM_CHANNEL4_ENABLE))
	/*##-2- Configure the PWM channels #########################################*/
	/* Common configuration for all channels */
	sConfig_Tim5.OCMode       = TIM_OCMODE_PWM1;
	sConfig_Tim5.OCPolarity   = TIM_OCPOLARITY_HIGH;
	sConfig_Tim5.OCFastMode   = TIM_OCFAST_DISABLE;
	sConfig_Tim5.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
	sConfig_Tim5.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	sConfig_Tim5.OCIdleState  = TIM_OCIDLESTATE_RESET;
#if(TIM5_PWM_CHANNEL1_ENABLE)
	sConfig_Tim5.Pulse = TIM5_PWM_DEFAUTL_PULSE_1; /* Set the pulse value for channel 1 */
	if ((ret = HAL_TIM_PWM_ConfigChannel(&Tim5Handle, &sConfig_Tim5, TIM_CHANNEL_1)) != HAL_OK) return ret; /* Configuration Error */
#endif /* TIM5_PWM_CHANNEL1_ENABLE */
#if(TIM5_PWM_CHANNEL2_ENABLE)
	sConfig_Tim5.Pulse = TIM5_PWM_DEFAUTL_PULSE_2; /* Set the pulse value for channel 2 */
	if ((ret = HAL_TIM_PWM_ConfigChannel(&Tim5Handle, &sConfig_Tim5, TIM_CHANNEL_2)) != HAL_OK) return ret;
#endif /* TIM5_PWM_CHANNEL2_ENABLE */
#if(TIM5_PWM_CHANNEL3_ENABLE)
	sConfig_Tim5.Pulse = TIM5_PWM_DEFAUTL_PULSE_3; /* Set the pulse value for channel 3 */
	if ((ret = HAL_TIM_PWM_ConfigChannel(&Tim5Handle, &sConfig_Tim5, TIM_CHANNEL_3)) != HAL_OK) return ret;
#endif /* TIM5_PWM_CHANNEL3_ENABLE */
#if(TIM5_PWM_CHANNEL4_ENABLE)
	sConfig_Tim5.Pulse = TIM5_PWM_DEFAUTL_PULSE_4; /* Set the pulse value for channel 4 */
	if ((ret = HAL_TIM_PWM_ConfigChannel(&Tim5Handle, &sConfig_Tim5, TIM_CHANNEL_4)) != HAL_OK) return ret;
#endif /* TIM5_PWM_CHANNEL4_ENABLE */
	/*##-3- Start PWM signals generation #######################################*/
#if(TIM5_PWM_CHANNEL1_ENABLE) /* Start channel 1 */
	if ((ret = HAL_TIM_PWM_Start(&Tim5Handle, TIM_CHANNEL_1)) != HAL_OK) return ret; /* PWM Generation Error */
#endif /* TIM5_PWM_CHANNEL1_ENABLE */
#if(TIM5_PWM_CHANNEL2_ENABLE) /* Start channel 2 */
	if ((ret = HAL_TIM_PWM_Start(&Tim5Handle, TIM_CHANNEL_2)) != HAL_OK) return ret;
#endif /* TIM5_PWM_CHANNEL2_ENABLE */
#if(TIM5_PWM_CHANNEL3_ENABLE) /* Start channel 3 */
	if ((ret = HAL_TIM_PWM_Start(&Tim5Handle, TIM_CHANNEL_3)) != HAL_OK) return ret;
#endif /* TIM5_PWM_CHANNEL3_ENABLE */
#if(TIM5_PWM_CHANNEL4_ENABLE) /* Start channel 4 */
	if ((ret = HAL_TIM_PWM_Start(&Tim5Handle, TIM_CHANNEL_4)) != HAL_OK) return ret;
#endif /* TIM5_PWM_CHANNEL4_ENABLE */
#endif /* TIM5_PWM_OUTPUT_ENABLE & (TIM5_PWM_CHANNEL1_ENABLE | TIM5_PWM_CHANNEL2_ENABLE | TIM5_PWM_CHANNEL3_ENABLE | TIM5_PWM_CHANNEL4_ENABLE) */
	return ret;
}
#endif /* TIM5_ENABLE */

/**
  * @brief TIM MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  * @param htim: TIM handle pointer
  * @retval None
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef   GPIO_InitStruct;
#if (TIM2_ENABLE)
	if(htim->Instance == TIM2) {
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* TIM2 Peripheral clock enable */
		TIM2_CLK_ENABLE();
#if (TIM2_PWM_OUTPUT_ENABLE & (TIM2_PWM_CHANNEL1_ENABLE | TIM2_PWM_CHANNEL2_ENABLE | TIM2_PWM_CHANNEL3_ENABLE | TIM2_PWM_CHANNEL4_ENABLE))
		/* Enable all GPIO Channels Clock requested */
		TIM2_PWM_GPIO_CLK_ENABLE();

		/* Configure Channel Pin in output, push-pull, alternate function mode */
		/* Common configuration for all channels */
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
#if(TIM2_PWM_CHANNEL1_ENABLE)
		GPIO_InitStruct.Alternate = TIM2_GPIO_AF_CHANNEL1;
		GPIO_InitStruct.Pin = TIM2_GPIO_PIN_CHANNEL1;
		HAL_GPIO_Init(TIM2_GPIO_PORT_CHANNEL1, &GPIO_InitStruct);
#endif /* TIM2_PWM_CHANNEL1_ENABLE */
#if(TIM2_PWM_CHANNEL2_ENABLE)
		GPIO_InitStruct.Alternate = TIM2_GPIO_AF_CHANNEL2;
		GPIO_InitStruct.Pin = TIM2_GPIO_PIN_CHANNEL2;
		HAL_GPIO_Init(TIM2_GPIO_PORT_CHANNEL2, &GPIO_InitStruct);
#endif /* TIM2_PWM_CHANNEL2_ENABLE */
#if(TIM2_PWM_CHANNEL3_ENABLE)
		GPIO_InitStruct.Alternate = TIM2_GPIO_AF_CHANNEL3;
		GPIO_InitStruct.Pin = TIM2_GPIO_PIN_CHANNEL3;
		HAL_GPIO_Init(TIM2_GPIO_PORT_CHANNEL3, &GPIO_InitStruct);
#endif /* TIM2_PWM_CHANNEL3_ENABLE */
#if(TIM2_PWM_CHANNEL4_ENABLE)
		GPIO_InitStruct.Alternate = TIM2_GPIO_AF_CHANNEL4;
		GPIO_InitStruct.Pin = TIM2_GPIO_PIN_CHANNEL4;
		HAL_GPIO_Init(TIM2_GPIO_PORT_CHANNEL4, &GPIO_InitStruct);
#endif /* TIM2_PWM_CHANNEL4_ENABLE */
#endif /* TIM2_PWM_OUTPUT_ENABLE & (TIM2_PWM_CHANNEL1_ENABLE | TIM2_PWM_CHANNEL2_ENABLE | TIM2_PWM_CHANNEL3_ENABLE | TIM2_PWM_CHANNEL4_ENABLE) */
	}
#endif /* TIM2_ENABLE */
#if (TIM5_ENABLE)
	if(htim->Instance == TIM5) {
		/*##-1- Enable peripherals and GPIO Clocks #################################*/
		/* TIM5 Peripheral clock enable */
		TIM5_CLK_ENABLE();
#if (TIM5_PWM_OUTPUT_ENABLE & (TIM5_PWM_CHANNEL1_ENABLE | TIM5_PWM_CHANNEL2_ENABLE | TIM5_PWM_CHANNEL3_ENABLE | TIM5_PWM_CHANNEL4_ENABLE))
		/* Enable all GPIO Channels Clock requested */
		TIM5_PWM_GPIO_CLK_ENABLE();

		/* Configure Channel Pin in output, push-pull, alternate function mode */
		/* Common configuration for all channels */
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
#if (TIM5_PWM_CHANNEL1_ENABLE)
		GPIO_InitStruct.Alternate = TIM5_GPIO_AF_CHANNEL1;
		GPIO_InitStruct.Pin = TIM5_GPIO_PIN_CHANNEL1;
		HAL_GPIO_Init(TIM5_GPIO_PORT_CHANNEL1, &GPIO_InitStruct);
#endif /* TIM5_PWM_CHANNEL1_ENABLE */
#if (TIM5_PWM_CHANNEL2_ENABLE)
		GPIO_InitStruct.Alternate = TIM5_GPIO_AF_CHANNEL2;
		GPIO_InitStruct.Pin = TIM5_GPIO_PIN_CHANNEL2;
		HAL_GPIO_Init(TIM5_GPIO_PORT_CHANNEL2, &GPIO_InitStruct);
#endif /* TIM5_PWM_CHANNEL2_ENABLE */
#if (TIM5_PWM_CHANNEL3_ENABLE)
		GPIO_InitStruct.Alternate = TIM5_GPIO_AF_CHANNEL3;
		GPIO_InitStruct.Pin = TIM5_GPIO_PIN_CHANNEL3;
		HAL_GPIO_Init(TIM5_GPIO_PORT_CHANNEL3, &GPIO_InitStruct);
#endif /* TIM5_PWM_CHANNEL3_ENABLE */
#if (TIM5_PWM_CHANNEL4_ENABLE)
		GPIO_InitStruct.Alternate = TIM5_GPIO_AF_CHANNEL4;
		GPIO_InitStruct.Pin = TIM5_GPIO_PIN_CHANNEL4;
		HAL_GPIO_Init(TIM5_GPIO_PORT_CHANNEL4, &GPIO_InitStruct);
#endif /* TIM5_PWM_CHANNEL4_ENABLE */
#endif /* TIM5_PWM_OUTPUT_ENABLE & (TIM5_PWM_CHANNEL1_ENABLE | TIM5_PWM_CHANNEL2_ENABLE | TIM5_PWM_CHANNEL3_ENABLE | TIM5_PWM_CHANNEL4_ENABLE) */
	}
#endif /* TIM5_ENABLE */
}

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
