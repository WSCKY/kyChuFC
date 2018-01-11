#ifndef __INNERCONTROLLOOP_H
#define __INNERCONTROLLOOP_H

#include "MotorControl.h"
#include "RF_DataProcess.h"
#include "IMU_DataProcess.h"

void InnerLoopCtrlTask(uint8_t millis);
void SetInnerLoopExpParam(float gx, float gy, float gz);

#endif /* __INNERCONTROLLOOP_H */

