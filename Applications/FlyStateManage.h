#ifndef __FLYSTATEMANAGE_H
#define __FLYSTATEMANAGE_H

#include "MotorControl.h"
#include "RF_DataProcess.h"
#include "GyroStableCheck.h"
#include "IMU_DataProcess.h"
#include "BARO_DataProcess.h"
#include "SystemPowerManage.h"

typedef enum {
	Locked = 0,
	UnLock = 1
} FLY_LOCK_STATE;

typedef enum {
	Disabled = 0,
	Enabled = 1
} FLY_ENABLE_STATE;

void FlyStateManageTask(uint8_t millis);

uint8_t GetDroneReadyFlag(void);
FLY_LOCK_STATE GetFlyLockState(void);
FLY_ENABLE_STATE GetFlyEnableState(void);

#endif /* __FLYSTATEMANAGE_H */
