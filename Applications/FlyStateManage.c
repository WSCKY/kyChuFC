#include "FlyStateManage.h"

static uint8_t _init_flag = 0;

static RF_COMMAND_DEF *pRF_CMD = {0};

static uint8_t DroneReadyFlag = 0;

static FLY_LOCK_STATE FlyEnableLock = 0;
static FLY_ENABLE_STATE FlyEnableFlag = 0;

static uint8_t DroneReadyCheck(void);
static uint8_t FlyEnableCheck(void);

static uint8_t UnlockStep = 0;
static uint32_t UnlockTimeStamp = 0;
static uint8_t FlyEnableTogCmdCnt = 0;
void FlyStateManageTask(uint8_t millis)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		FlyEnableLock = Locked;
		FlyEnableFlag = Disabled;
		pRF_CMD = GetRFCommand();
	}

	DroneReadyFlag = DroneReadyCheck();

	if((FlyEnableLock == Locked) && (DroneReadyFlag = 1)) {
		UnlockTimeStamp ++;
		if(UnlockTimeStamp * millis >= 500) UnlockStep = 0;
		if((UnlockStep == 0 && pRF_CMD->Mode == FlightMode1) || \
		   (UnlockStep == 1 && pRF_CMD->Mode == FlightMode2) || \
		   (UnlockStep == 2 && pRF_CMD->Mode == FlightMode3) || \
		   (UnlockStep == 3 && pRF_CMD->Mode == FlightMode2) || \
		   (UnlockStep == 4 && pRF_CMD->Mode == FlightMode1) || \
		   (UnlockStep == 5 && pRF_CMD->Mode == FlightMode2) || \
		   (UnlockStep == 6 && pRF_CMD->Mode == FlightMode3) || \
		   (UnlockStep == 7 && pRF_CMD->Mode == FlightMode2) || \
		   (UnlockStep == 8 && pRF_CMD->Mode == FlightMode1)) {
			UnlockStep ++; UnlockTimeStamp = 0;
		}
		if(UnlockStep >= 9) {
			FlyEnableLock = UnLock;
			FlyEnableFlag = Disabled;
			SendMotorBeepRequest(Audio_FlyUnlock);
			UnlockStep = 0; UnlockTimeStamp = 0;
		}
	}

	if(pRF_CMD->TrigSwitch == DualState_Low) {
		if(FlyEnableTogCmdCnt == 2) {
			FlyEnableTogCmdCnt = 0;
			if(FlyEnableFlag == Disabled) {
				if(FlyEnableLock == UnLock) {
					if(FlyEnableCheck()) {
						FlyEnableFlag = Enabled;
					}
				}
			} else {
				FlyEnableFlag = Disabled;
			}
		} else {
			FlyEnableTogCmdCnt = 1;
		}
	} else if(FlyEnableTogCmdCnt == 1) {
		FlyEnableTogCmdCnt = 2;
	}
}

static uint8_t DroneReadyCheck(void)
{
	if(!GyroIsCalibrated()) return 0;
	if(!AccIsStabled()) return 0;
	if(!GetBaroOffsetedFlag()) return 0;
	return 1;
}

static uint8_t FlyEnableCheck(void)
{
	if(DroneReadyFlag == 0) return 0;
	if(RfSignalIsLost()) return 0;
	if(GetBatteryState() >= LOW_POWER_LV2) return 0;
	return 1;
}

inline uint8_t GetDroneReadyFlag(void) { return DroneReadyFlag; }
inline FLY_LOCK_STATE GetFlyLockState(void) { return FlyEnableLock; }
inline FLY_ENABLE_STATE GetFlyEnableState(void) { return FlyEnableFlag; }
