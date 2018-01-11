/**
  ******************************************************************************
  * @file    main.c
  * @author  kyChu
  * @version V0.1
  * @date    04-January-2018
  * @brief   Default main function.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SYSTEM_TIMER_TICK                        (2)
#define HIGH_FREQ_TASK_DIV_FACTOR                (1)
#define MIDA_FREQ_TASK_DIV_FACTOR                (5)
#define MIDB_FREQ_TASK_DIV_FACTOR                (5)
#define SLOW_FREQ_TASK_DIV_FACTOR                (10)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osThreadId HighThreadId, MidThreadAId, MidThreadBId, SlowThreadId, MiniThreadId;
osThreadId SystemTimerId;

static signed portBASE_TYPE xHigherPriorityTaskWoken = pdTRUE;
xSemaphoreHandle xSemaphore_HighFreq = NULL, xSemaphore_MidFreqA = NULL, xSemaphore_MidFreqB = NULL, xSemaphore_SlowFreq = NULL;

static uint32_t SysTimerCnt = 0;
/* Private function prototypes -----------------------------------------------*/
static void SystemTimerCallback(void const *p);
static void SystemHighFreqThread(void const *p);
static void SystemMidFreqAThread(void const *p);
static void SystemMidFreqBThread(void const *p);
static void SystemSlowFreqThread(void const *p);
static void SystemMiniFreqThread(void const *p);
static void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	/* Enable the CPU Cache */
	CPU_CACHE_Enable();
	/* STM32F7xx HAL library initialization:
	       - Configure the Flash ART accelerator
	       - Systick timer is configured by default as source of time base, but user
	         can eventually implement his proper time base source (a general purpose
	         timer for example or other time source), keeping in mind that Time base
	         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and
	         handled in milliseconds basis.
	       - Set NVIC Group Priority to 4
	       - Low Level Initialization
	     */
	HAL_Init();

	/* Configure the system clock to 216 MHz */
	SystemClock_Config();

	PowerControlInit();
	PWR_ON();

	BoardLedInit();
	if(MCU_SPIs_Init() != HAL_OK) {
		while(1);
	}
	if(MCU_UARTs_Init() != HAL_OK) {
		while(1);
	}
	if(MCU_USARTs_Init() != HAL_OK) {
		while(1);
	}
	if(MCU_TIMs_Init() != HAL_OK) {
		while(1);
	}
	if(IMU_ICM20602_Init() != HAL_OK) {
		while(1);
	}

	vSemaphoreCreateBinary( xSemaphore_HighFreq ); if(xSemaphore_HighFreq == NULL) {while(1);}
	vSemaphoreCreateBinary( xSemaphore_MidFreqA ); if(xSemaphore_MidFreqA == NULL) {while(1);}
	vSemaphoreCreateBinary( xSemaphore_MidFreqB ); if(xSemaphore_MidFreqB == NULL) {while(1);}
	vSemaphoreCreateBinary( xSemaphore_SlowFreq ); if(xSemaphore_SlowFreq == NULL) {while(1);}

	osThreadDef(0, SystemHighFreqThread, osPriorityHigh, 0, configMINIMAL_STACK_SIZE * 4);
	HighThreadId = osThreadCreate(osThread(0), NULL);
	osThreadDef(1, SystemMidFreqAThread, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE * 4);
	MidThreadAId = osThreadCreate(osThread(1), NULL);
	osThreadDef(2, SystemMidFreqBThread, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE * 2);
	MidThreadBId = osThreadCreate(osThread(2), NULL);
	osThreadDef(3, SystemSlowFreqThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	SlowThreadId = osThreadCreate(osThread(3), NULL);
	osThreadDef(4, SystemMiniFreqThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 2);
	MiniThreadId = osThreadCreate(osThread(4), NULL);

	osTimerDef(SysTimer, SystemTimerCallback);
	SystemTimerId = osTimerCreate(osTimer(SysTimer), osTimerPeriodic, (void *)0);
	/* Start System Timer */
	osTimerStart(SystemTimerId, SYSTEM_TIMER_TICK);

	DEBUG_PORT_SEND("System Init!\n", 13);
	SendMotorBeepRequest(Audio_PowerOn);

	osKernelStart();

	for(;;);
}

static void SystemTimerCallback(void const *p)
{
	SysTimerCnt ++;
	xSemaphoreGiveFromISR( xSemaphore_HighFreq, &xHigherPriorityTaskWoken );
	if(SysTimerCnt % MIDA_FREQ_TASK_DIV_FACTOR == 1) {
		xSemaphoreGiveFromISR( xSemaphore_MidFreqA, &xHigherPriorityTaskWoken );
	}
	if(SysTimerCnt % MIDB_FREQ_TASK_DIV_FACTOR == 2) {
		xSemaphoreGiveFromISR( xSemaphore_MidFreqB, &xHigherPriorityTaskWoken );
	}
	if(SysTimerCnt % SLOW_FREQ_TASK_DIV_FACTOR == 3) {
		xSemaphoreGiveFromISR( xSemaphore_SlowFreq, &xHigherPriorityTaskWoken );
	}

	if(SysTimerCnt >= 60000) SysTimerCnt = 0;
}

static void SystemHighFreqThread(void const *p)
{
	(void) p;
	uint8_t TaskT = SYSTEM_TIMER_TICK * HIGH_FREQ_TASK_DIV_FACTOR;
	for(;;) {
		if( xSemaphoreTake( xSemaphore_HighFreq, portMAX_DELAY ) == pdTRUE ) {
			IMU_DataPreProcessTask(TaskT);
			InnerLoopCtrlTask(TaskT);
			MotorControlTask(TaskT);
		}
	}
}

static void SystemMidFreqAThread(void const *p)
{
	(void) p;
	uint8_t TaskT = SYSTEM_TIMER_TICK * MIDA_FREQ_TASK_DIV_FACTOR;
	for(;;) {
		if( xSemaphoreTake( xSemaphore_MidFreqA, portMAX_DELAY ) == pdTRUE ) {
			RF_DataProcessTask(TaskT);
			FlyStateManageTask(TaskT);
			AngleCtrlModeTask(TaskT);
		}
	}
}

static void SystemMidFreqBThread(void const *p)
{
	(void) p;
	uint8_t TaskT = SYSTEM_TIMER_TICK * MIDB_FREQ_TASK_DIV_FACTOR;
	for(;;) {
		if( xSemaphoreTake( xSemaphore_MidFreqB, portMAX_DELAY ) == pdTRUE ) {
			IMU_StableCalibrationTask(TaskT);
			SendDataToWaveMonitor();
		}
	}
}

static void SystemSlowFreqThread(void const *p)
{
	(void) p;
	for(;;) {
		if( xSemaphoreTake( xSemaphore_SlowFreq, portMAX_DELAY ) == pdTRUE ) {
			// ...
		}
	}
}

static void SystemMiniFreqThread(void const *p)
{
	(void) p;
	uint32_t PreviousWakeTime = osKernelSysTick();
	for(;;) {
		osDelayUntil(&PreviousWakeTime, 200);
		if(RfSignalIsLost()) {
			LED_BLUE_TOG();
		} else {
			LED_GREEN_TOG();
		}
	}
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 24000000
  *            PLL_M                          = 24
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 24;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);

  if(ret != HAL_OK)
  {
   while(1) {};
  }

  /* Activate the OverDrive to reach the 216 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
   while(1) {};
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);
  if(ret != HAL_OK)
  {
   while(1) {};
  }
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
