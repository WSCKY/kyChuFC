#include "GyroStableCheck.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUF_SIZE                                 (100)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t _init_flag = 0;
/* data from sensor */
static IMU_RawDataDef *pIMU_RAW;
static IMU_UnitDataDef *pIMU_UNIT;

/* Compute result */
static float Gravity = 9.8f;//Initial value.
static uint8_t GyrInPeaceFlag = 0;
static uint8_t GyrCalibratedFlag = 0;
static uint8_t AccInPeaceFlag = 0;
static uint8_t AccWasStabledFlag = 0;

/* compute needed resource */
static _3AxisINT GyrPeaceBuf[BUF_SIZE];
static float AccModelBuf[BUF_SIZE] = {0};
static uint8_t PeaceDataCnt = 0, PeaceDataIndex = 0;
static uint8_t _GryoCalibInitFlag = 0;
static float GyroCalibData[3] = {0};

/* Private function prototypes -----------------------------------------------*/
static uint8_t GyrStableCheck(uint8_t millis);
static uint8_t AccStableCheck(uint8_t millis);

/* Private functions ---------------------------------------------------------*/

void IMU_StableCalibrationTask(uint8_t millis)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		pIMU_RAW = GetIMU_RAW_DATA();
		pIMU_UNIT = GetIMU_Unit_DATA();
	}
	GyrInPeaceFlag = GyrStableCheck(millis);
	AccStableCheck(millis);
}

static uint8_t GyrStableCheck(uint8_t millis)
{
	static uint32_t PeaceTimeCnt = 0;
	static int16_t oldGyroX = 0, oldGyroY = 0, oldGyroZ = 0;

	unsigned int turbulen = ABS(pIMU_RAW->GyrX - oldGyroX) + \
							ABS(pIMU_RAW->GyrY - oldGyroY) + \
							ABS(pIMU_RAW->GyrZ - oldGyroZ);
	float AccMod = sqrtf(SQUARE(pIMU_UNIT->Acc.X) + SQUARE(pIMU_UNIT->Acc.Y) + SQUARE(pIMU_UNIT->Acc.Z));

	oldGyroX = pIMU_RAW->GyrX; oldGyroY = pIMU_RAW->GyrY; oldGyroZ = pIMU_RAW->GyrZ;

	if(turbulen < 25) {
		if(PeaceTimeCnt < (1000 / millis)) { //keep 1 second.
			PeaceTimeCnt ++;
			return 0;
		} else {
			if(_GryoCalibInitFlag == 0) {
				_GryoCalibInitFlag = 1;
				GyroCalibData[0] = oldGyroX;
				GyroCalibData[1] = oldGyroY;
				GyroCalibData[2] = oldGyroZ;
//				SetGyrCalibdata(GyroCalibData); //update calibration data.
				Gravity = AccMod;
				AccWasStabledFlag = 1;//accelerator are Stabled.
				GyrCalibratedFlag = 1;//gyroscope are calibrated.
			} else {
				if(PeaceDataCnt < BUF_SIZE) {
					GyrPeaceBuf[PeaceDataCnt].X = oldGyroX;
					GyrPeaceBuf[PeaceDataCnt].Y = oldGyroY;
					GyrPeaceBuf[PeaceDataCnt].Z = oldGyroZ;
					AccModelBuf[PeaceDataCnt] = AccMod;
					PeaceDataCnt ++;
				} else {
					GyrPeaceBuf[PeaceDataIndex].X = oldGyroX;
					GyrPeaceBuf[PeaceDataIndex].Y = oldGyroY;
					GyrPeaceBuf[PeaceDataIndex].Z = oldGyroZ;
					AccModelBuf[PeaceDataIndex] = AccMod;
					PeaceDataIndex ++;

					if(PeaceDataIndex == BUF_SIZE)
						PeaceDataIndex = 0;

					GyroCalibData[0] = GyroCalibData[0] * 0.9f + GyrPeaceBuf[PeaceDataIndex].X * 0.1f;
					GyroCalibData[1] = GyroCalibData[1] * 0.9f + GyrPeaceBuf[PeaceDataIndex].Y * 0.1f;
					GyroCalibData[2] = GyroCalibData[2] * 0.9f + GyrPeaceBuf[PeaceDataIndex].Z * 0.1f;
//					SetGyrCalibdata(GyroCalibData); //update calibration data.
					Gravity = Gravity * 0.99f + AccModelBuf[PeaceDataIndex] * 0.01f;
//					SetGravityCalibData(&Gravity); //update calibration data.
				}
			}
			return 1;
		}
	} else {
		PeaceDataCnt = 0;
		PeaceTimeCnt = 0;
		PeaceDataIndex = 0;
		return 0;
	}
}

static uint32_t AccStableCnt = 0, AccSampleCnt = 0;
static float AccModRaw = 0, LastAccRaw = 0;
static float AccSum = 0;

static uint8_t AccStableCheck(uint8_t millis)
{
	uint8_t Flag = 0;
	AccModRaw = sqrtf(SQUARE(pIMU_RAW->AccX) + SQUARE(pIMU_RAW->AccY) + SQUARE(pIMU_RAW->AccZ));
	if(fabs(LastAccRaw - AccModRaw) < 60) {
		if(AccStableCnt < (1000 / millis)) //keep  1 sencond.
			AccStableCnt ++;
		else {
			Flag = 1;
			if(AccWasStabledFlag == 0) {
				float AccMod = sqrtf(SQUARE(pIMU_UNIT->Acc.X) + SQUARE(pIMU_UNIT->Acc.Y) + SQUARE(pIMU_UNIT->Acc.Z));
				if(AccSampleCnt < 100)
					AccSum += AccMod;
				else {
					Gravity = AccSum / 100;
					AccWasStabledFlag = 1;
				}
				AccSampleCnt ++;
			}
//			else {
//				Gravity = Gravity * 0.99f + sqrtf(accUnit.accX * accUnit.accX + accUnit.accY * accUnit.accY + accUnit.accZ * accUnit.accZ) * 0.01f;
//			}
		}
	} else {
		AccStableCnt = 0;
		AccSampleCnt = 0;
		AccSum = 0;
	}
	LastAccRaw = AccModRaw;
	return Flag;
}

inline void SetGravity(float g) { Gravity = g; }
inline float GetGravity(void) { return Gravity; }
inline uint8_t GetGyrPeaceFlag(void) { return GyrInPeaceFlag; }
inline uint8_t GetAccStableFlag(void) { return AccInPeaceFlag; }
inline uint8_t GyroIsCalibrated(void) { return GyrCalibratedFlag; }
inline uint8_t AccIsStabled(void) { return AccWasStabledFlag; }

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
