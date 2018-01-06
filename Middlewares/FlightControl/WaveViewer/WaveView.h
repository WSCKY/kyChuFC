#ifndef __WAVEVIEW_H
#define __WAVEVIEW_H

#include "BoardDefine.h"

#define SUPPORT_WAVE_NUMBER                      9
typedef struct {
	uint8_t HEADER;
	uint8_t TYPE;
	uint8_t LENGTH;
	__packed union {
		uint8_t d[4];
		float f;
	} DATA[SUPPORT_WAVE_NUMBER];
	uint8_t CHKSUM;
} WAVEDATA_TypeDef;

typedef union {
	WAVEDATA_TypeDef WaveData;
	uint8_t RawData[40];
} WAVEDATA_BUFFER_TypeDef;

void SendDataToWaveMonitor(void);

#endif /* __WAVEVIEW_H */

