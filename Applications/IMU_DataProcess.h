#ifndef __IMU_DATAPROCESS_H
#define __IMU_DATAPROCESS_H

#include "IMU_ICM20602.h"
#include "GyroStableCheck.h"

void IMU_DataPreProcessTask(uint8_t millis);
IMU_UnitDataDef *GetIMU_Unit_DATA(void);

#endif /* __IMU_DATAPROCESS_H */

