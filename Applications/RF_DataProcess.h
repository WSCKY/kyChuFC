#ifndef __RF_DATAPROCESS_H
#define __RF_DATAPROCESS_H

#include "Maths.h"

#include "WifiCP.h"
#include "BoardDefine.h"

#define SIGNAL_LOST_CONFIRM_TIME                 (1000)

#define RF_CHANNEL_RAW_MAX                       (1696)
#define RF_CHANNEL_RAW_MID                       (1024)
#define RF_CHANNEL_RAW_MIN                       (352)

#define RF_CHANNEL_RAW_DEADBAND                  (100)

#define RF_COMMAND_UNIT_MAX                      (2000)
#define RF_COMMAND_UNIT_MID                      (1500)
#define RF_COMMAND_UNIT_MIN                      (1000)

typedef enum {
	FlightMode_None = 0,
	FlightMode1 = 1,
	FlightMode2 = 2,
	FlightMode3 = 3,
	FlightMode4 = 4
} FlightModeDef;

typedef enum {
	DualState_Low = 0,
	DualState_High = 1
} DualState;

typedef enum {
	TriState_Low = 0,
	TriState_Mid = 1,
	TriState_High = 2
} TriState;

typedef struct {
	uint16_t Roll;
	uint16_t Pitch;
	uint16_t Throttle;
	uint16_t Yaw;
	FlightModeDef Mode;
	DualState TrigSwitch;
	TriState CmdSwitch;
	DualState RevTrigSwitch;
	TriState RevTriState[4];
} RF_COMMAND_DEF;

void RF_DataProcessTask(uint8_t millis);

uint8_t RfSignalIsLost(void);
RF_COMMAND_DEF *GetRFCommand(void);

#endif /* __RF_DATAPROCESS_H */

