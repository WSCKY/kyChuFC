#ifndef __BARO_DATAPROCESS_H
#define __BARO_DATAPROCESS_H

#include "BARO_MPC2520.h"
#include "FlyStateManage.h"

#define BARO_READ_DELAY_LIMIT                    (10)
#define BARO_OFFSET_LEVEL_CALIB_TIME             (3000)

void BARO_DataProcessTask(uint8_t millis);

uint8_t GetBaroOffsetedFlag(void);
float GetBaroCalibratedAltitude(void);
float GetBaroMeasureTemperature(void);

#endif /* __BARO_DATAPROCESS_H */
