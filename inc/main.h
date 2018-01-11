/**
  ******************************************************************************
  * @file    main.h
  * @author  kyChu
  * @version V0.1
  * @date    04-January-2018
  * @brief   Header file for main.c.
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"

#include "cmsis_os.h"

#include "WaveView.h"
#include "BoardLed.h"
#include "BoardPower.h"

#include "MCU_Periph.h"

#include "MotorControl.h"
#include "AngleCtrlMode.h"
#include "FlyStateManage.h"
#include "RF_DataProcess.h"
#include "GyroStableCheck.h"
#include "IMU_DataProcess.h"
#include "InnerControlLoop.h"

/* Macros --------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported definitions ------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
