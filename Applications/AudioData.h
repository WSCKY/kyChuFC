#ifndef __AUDIODATA_H
#define __AUDIODATA_H

#include "SystemTypes.h"

#define AUDIO_NUMBER                             (3)

static const ToneDataDef PowerOn[3] = {
	{.Freq = 880, .Time = 200, .Volume = 50, .Times = 1, .Delay = 0},
	{.Freq = 1175, .Time = 150, .Volume = 90, .Times = 1, .Delay = 150},
	{.Freq = 1760, .Time = 250, .Volume = 50, .Times = 1, .Delay = 200}
};

static const ToneDataDef PowerOff[3] = {
	{.Freq = 1760, .Time = 175, .Volume = 50, .Times = 1, .Delay = 0},
	{.Freq = 1175, .Time = 125, .Volume = 50, .Times = 1, .Delay = 150},
	{.Freq = 880, .Time = 225, .Volume = 50, .Times = 1, .Delay = 200}
};

static const ToneDataDef FlyUnlock[1] = {
	{.Freq = 880, .Time = 225, .Volume = 50, .Times = 3, .Delay = 200}
};

static const ToneBuffDef AllTones[AUDIO_NUMBER] = {
	{PowerOn, 3},
	{PowerOff, 3},
	{FlyUnlock, 1}
};

typedef enum {
	Audio_PowerOn = 0,
	Audio_PowerOff = 1,
	Audio_FlyUnlock = 2,
	Audio_RF_Got = 3,
	Audio_RF_Lost = 4,
	Audio_DroneReady = 5,
} AudioTypeDef;

#endif /* __AUDIODATA_H */

