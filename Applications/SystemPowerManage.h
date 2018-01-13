#ifndef __SYSTEMPOWERMANAGE_H
#define __SYSTEMPOWERMANAGE_H

#include "BoardDefine.h"

#define MCU_REF_VOLTAGE                          (3.3f)
#define VOLT_MEASURE_COEF                        (2.0f)

#define POWEROFF_CONFIRM_TIME                    (2000)

typedef enum {
	Key_Pressed = 0,
	Key_Released = 1
} KEY_STATE;

void BatteryVoltageCheckTask(uint8_t millis);
void DroneAutoPowerOffTask(uint8_t millis);

float GetSystemVoltage(void);
void SendSystemPowerOffReq(void);
void SetVoltageCalibParam(float _k, float _b);

#endif /* __SYSTEMPOWERMANAGE_H */
