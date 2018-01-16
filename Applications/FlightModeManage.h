#ifndef __FLIGHTMODEMANAGE_H
#define __FLIGHTMODEMANAGE_H

#include "AngleCtrlMode.h"
#include "FlipCtrlMode.h"

#include "MotorControl.h"
#include "RF_DataProcess.h"

typedef enum {
	GROUP1_MODE1 = 0,
	GROUP1_MODE2 = 1,
	GROUP1_MODE3 = 2,
	GROUP2_MODE1 = 3,
	GROUP2_MODE2 = 4,
	GROUP2_MODE3 = 5,
	GROUP3_MODE1 = 6,
	GROUP3_MODE2 = 7,
	GROUP3_MODE3 = 8,
} FLIGHT_MODE;

void FlightModeManageTask(uint8_t millis);

FLIGHT_MODE GetFlightMode(void);

#endif /* __FLIGHTMODEMANAGE_H */
