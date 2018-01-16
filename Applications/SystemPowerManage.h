#ifndef __SYSTEMPOWERMANAGE_H
#define __SYSTEMPOWERMANAGE_H

#include "BoardDefine.h"

#define MCU_REF_VOLTAGE                          (3.3f)
#define VOLT_MEASURE_COEF                        (2.0f)

#define BATTERY_BAD_VOLTAGE                      (2.8f)
#define BATTERY_LOW_POWER_VOLTAGE                (3.3f)
#define BATTERY_PRELOW_POWER_VOLTAGE             (3.45f)

#define POWEROFF_CONFIRM_TIME                    (2000)

typedef enum {
	POWER_ENOUGH = 0,
	LOW_POWER_LV1 = 1,
	LOW_POWER_LV2 = 2,
	BAD_BATTERY = 3,
} BATTERY_STATE;

typedef enum {
	Key_Pressed = 0,
	Key_Released = 1
} KEY_STATE;

void BatteryVoltageCheckTask(uint8_t millis);
void DroneAutoPowerOffTask(uint8_t millis);

float GetSystemVoltage(void);
BATTERY_STATE GetBatteryState(void);

void SendSystemPowerOffReq(void);
void SetVoltageCalibParam(float _k, float _b);

#endif /* __SYSTEMPOWERMANAGE_H */
