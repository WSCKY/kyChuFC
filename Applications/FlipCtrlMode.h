#ifndef __FLIPCTRLMODE_H
#define __FLIPCTRLMODE_H

#include "RF_DataProcess.h"
#include "IMU_DataProcess.h"
#include "InnerControlLoop.h"

typedef enum {
	Flip_Stop = 0,
	Flip_Start = 1,
	Flip_Ended = 2,
} FLIP_STATE;

typedef enum {
	Flip_Left = 0,
	Flip_Right = 1,
	Flip_Forward = 2,
	Flip_Backward = 3,
} FLIP_DIRECTION;

void FlipCtrlModeTask(uint8_t millis);

#endif /* __FLIPCTRLMODE_H */

