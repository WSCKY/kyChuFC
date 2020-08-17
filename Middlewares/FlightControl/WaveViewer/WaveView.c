#include "WaveView.h"
#include "cpu_utils.h"
#include "BARO_MPC2520.h"
#include "IMU_DataProcess.h"
#include "BARO_DataProcess.h"
#include "SystemPowerManage.h"
#include "SystemTimeCounter.h"

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
static uint16_t *p;
BARO_UnitDataDef *pBARO_Unit;
void SendDataToWaveMonitor(void)
{
	if(_init_flag == 0) {
		_init_flag = 1;
		InitDataStruct(&WAVE_DATA.WaveData);
		pIMU_Unit = GetIMU_Unit_DATA();
		p = GetADC1ConvertedValue();
		pBARO_Unit = GetBARO_UNIT_DATA();
	}

	WAVE_DATA.WaveData.DATA[0].f = pBARO_Unit->Altitude;//GetSystemVoltage();
	WAVE_DATA.WaveData.DATA[1].f = GetBaroCalibratedAltitude();//pBARO_Unit->Pressure;//pIMU_Unit->Acc.Y;
	WAVE_DATA.WaveData.DATA[2].f = pBARO_Unit->Temperature;//pIMU_Unit->Acc.Z;
	WAVE_DATA.WaveData.DATA[3].f = GetDroneFlipEulerAngle()->Pitch;//pIMU_Unit->Gyr.X;
	WAVE_DATA.WaveData.DATA[4].f = GetDroneFlipEulerAngle()->Roll;//pIMU_Unit->Gyr.Y;
	WAVE_DATA.WaveData.DATA[5].f = GetDroneFlipEulerAngle()->Yaw;//pIMU_Unit->Gyr.Z;
	WAVE_DATA.WaveData.DATA[6].f = _Get_Micros();//GetDroneEulerAngle()->Pitch;
	WAVE_DATA.WaveData.DATA[7].f = _Get_Millis();//GetDroneEulerAngle()->Roll;
	WAVE_DATA.WaveData.DATA[8].f = _Get_Seconds();//osGetCPUUsage();//SystemCoreClock / 1000000;//GetDroneEulerAngle()->Yaw;

	DataStructChkSum(&WAVE_DATA.WaveData);

	DEBUG_PORT_SEND(WAVE_DATA.RawData, 40);
}
