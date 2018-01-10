#include "WaveView.h"
#include "IMU_ICM20602.h"

static uint8_t _init_flag = 0;
static WAVEDATA_BUFFER_TypeDef WAVE_DATA = {0};

static void InitDataStruct(WAVEDATA_TypeDef *d)
{
	d->HEADER = 0x55;
	d->TYPE = 0xDD;
	d->LENGTH = 36;
	d->CHKSUM = 0;
}

static void DataStructChkSum(WAVEDATA_TypeDef *d)
{
	uint8_t sum = 0, i = 0;
	for(; i < 36; i ++)
		sum += d->DATA[i >> 2].d[i & 0x3];
	d->CHKSUM = sum;
}

static IMU_UnitDataDef *pIMU_Unit;
void SendDataToWaveMonitor(void)
{
	if(_init_flag == 0) {
		_init_flag = 1;
		InitDataStruct(&WAVE_DATA.WaveData);
		pIMU_Unit = GetIMU_Unit_DATA();
	}

	WAVE_DATA.WaveData.DATA[0].f = pIMU_Unit->Acc.X;
	WAVE_DATA.WaveData.DATA[1].f = pIMU_Unit->Acc.Y;
	WAVE_DATA.WaveData.DATA[2].f = pIMU_Unit->Acc.Z;
	WAVE_DATA.WaveData.DATA[3].f = TIM5->CNT;
	WAVE_DATA.WaveData.DATA[4].f = pIMU_Unit->Gyr.X;
	WAVE_DATA.WaveData.DATA[5].f = pIMU_Unit->Gyr.Y;
	WAVE_DATA.WaveData.DATA[6].f = pIMU_Unit->Gyr.Z;
	WAVE_DATA.WaveData.DATA[7].f = TIM5->ARR;
	WAVE_DATA.WaveData.DATA[8].f = TIM5->PSC;

	DataStructChkSum(&WAVE_DATA.WaveData);

	DEBUG_PORT_SEND(WAVE_DATA.RawData, 40);
}
