#ifndef __GYROSTABLECHECK_H
#define __GYROSTABLECHECK_H

#include "stm32f7xx.h"

#include "Maths.h"
#include "IMU_ICM20602.h"
#include "IMU_DataProcess.h"

void IMU_StableCalibrationTask(uint8_t Ticks);

float GetGravity(void);
void SetGravity(float g);
uint8_t GetGyrPeaceFlag(void);
uint8_t GetAccStableFlag(void);
uint8_t AccIsStabled(void);
uint8_t GyroIsCalibrated(void);
_3AxisFloat *GetGyroCalibData(void);

#endif /* __GYROSTABLECHECK_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
