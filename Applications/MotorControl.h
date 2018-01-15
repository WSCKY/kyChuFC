#ifndef __MOTORCONTROL_H
#define __MOTORCONTROL_H

#include "Maths.h"

#include "AudioData.h"
#include "BoardDefine.h"
#include "FlyStateManage.h"

#define MOTOR_CONTROL_RANGE_MIN                  (0)
#define MOTOR_CONTROL_RANGE_MAX                  (1000)

#define MOTOR_BEEP_READY_TIME                    (500)

typedef enum {
	Beeping = 0,
	Delaying = 1
} BEEP_DELAY_STATE;

typedef enum {
	DIR_FORWARD = 0,
	DIR_NEGATER = 1
} MOTOR_RUN_DIR;

void MotorControlTask(uint8_t millis);
void SendMotorBeepRequest(AudioTypeDef req);
void SetDroneThrottle(float thr);
MOTOR_RUN_DIR GetMotorRunDir(void);
void SetMotorRunDir(MOTOR_RUN_DIR _dir);
void SetMotorControlParam(float pitch, float roll, float yaw);

#endif /* __MOTORCONTROL_H */
