#ifndef __IMU_ICM20602_H
#define __IMU_ICM20602_H

#include "stm32f7xx.h"
#include "BoardDefine.h"
#include "SystemTypes.h"

/* Exported types ------------------------------------------------------------*/
typedef struct {
	int16_t AccX;
	int16_t AccY;
	int16_t AccZ;
	int16_t Temp;
	int16_t GyrX;
	int16_t GyrY;
	int16_t GyrZ;
} SensorDataDef;
/* Exported constants --------------------------------------------------------*/

HAL_StatusTypeDef IMU_ICM20602_Init(void);
HAL_StatusTypeDef IMU_ICM20602_Read(void);
IMU_RawDataDef *GetIMU_RAW_DATA(void);
void IMU_ConvertRawToUnit(IMU_RawDataDef *pRaw, IMU_UnitDataDef *pUnit);

#endif /* __IMU_ICM20602_H */

