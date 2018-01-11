#include "MotorControl.h"

static uint8_t MotorBeepReadyFlag = 0;
static uint32_t MotorBeepReadyTimeCnt = 0;
static float PitchOut = 0, RollOut = 0, YawOut = 0, ThrottleOut = 0;

static void StopMotorBeep(void);
static void MotorBeepTask(uint8_t millis);
static void StartMotorBeep(const ToneDataDef *pTone);

void MotorControlTask(uint8_t millis)
{
	float max_val = 0, min_val = 0;
	float MotorExp_1 = + PitchOut + RollOut - YawOut + ThrottleOut;
	float MotorExp_2 = + PitchOut - RollOut + YawOut + ThrottleOut;
	float MotorExp_3 = - PitchOut - RollOut - YawOut + ThrottleOut;
	float MotorExp_4 = - PitchOut + RollOut + YawOut + ThrottleOut;

	if(MotorExp_1 > MOTOR_CONTROL_RANGE_MAX || MotorExp_2 > MOTOR_CONTROL_RANGE_MAX || \
	   MotorExp_3 > MOTOR_CONTROL_RANGE_MAX || MotorExp_4 > MOTOR_CONTROL_RANGE_MAX) {
		max_val = MAX(MotorExp_1, MotorExp_2);
		max_val = MAX(max_val, MotorExp_3);
		max_val = MAX(max_val, MotorExp_4);
		max_val -= MOTOR_CONTROL_RANGE_MAX;
		MotorExp_1 -= max_val; MotorExp_2 -= max_val; MotorExp_3 -= max_val; MotorExp_4 -= max_val;
	}
	if(MotorExp_1 < MOTOR_CONTROL_RANGE_MIN || MotorExp_2 < MOTOR_CONTROL_RANGE_MIN || \
	   MotorExp_3 < MOTOR_CONTROL_RANGE_MIN || MotorExp_4 < MOTOR_CONTROL_RANGE_MIN) {
		min_val = MIN(MotorExp_1, MotorExp_2);
		min_val = MIN(min_val, MotorExp_3);
		min_val = MIN(min_val, MotorExp_4);
		min_val = MOTOR_CONTROL_RANGE_MIN - min_val;
		MotorExp_1 += min_val; MotorExp_2 += min_val; MotorExp_3 += min_val; MotorExp_4 += min_val;
	}

	if(GetFlyEnableState() == Enabled) {
		if(MotorBeepReadyFlag == 1) {
			MOTOR_BEEP_EXIT(); /* EXIT BEEP MODE */
			MotorBeepReadyFlag = 0;
			MotorBeepReadyTimeCnt = 0;
		}
		MOTOR_FORWARD_SPEED(MotorExp_1, MotorExp_2, MotorExp_3, MotorExp_4);
	} else {
		if(MotorBeepReadyFlag == 0) {
			MOTOR_FORWARD_SPEED(0, 0, 0, 0);
			if(MotorBeepReadyTimeCnt * millis < MOTOR_BEEP_READY_TIME) {
				MotorBeepReadyTimeCnt ++;
			} else {
				MOTOR_BEEP_INIT(); /* ENTRY BEEP MODE */
				MotorBeepReadyFlag = 1;
			}
		} else {
			MotorBeepTask(millis);
		}
	}
}

static uint8_t MotorInBeepFlag = 0;
static uint32_t UsrBeepReq = 0x00000000;

static const ToneDataDef *pTone;
static uint8_t ToneNum = 0;
static uint8_t ToneNumCnt = 0;
static uint16_t ToneTimeCnt = 0;
static uint16_t ToneTimesCnt = 0;
static uint16_t ToneDelayCnt = 0;
static BEEP_DELAY_STATE _beep_sta = Beeping;
static void MotorBeepTask(uint8_t millis)
{
	uint8_t i = 0;
	if(MotorInBeepFlag == 0) {
		if(UsrBeepReq == 0x0) return;
		for(i = 0; i < AUDIO_NUMBER; i ++) {
			if((UsrBeepReq & (1 << i)) != 0) {
				pTone = AllTones[i].pTone;
				ToneNum = AllTones[i].NumTones;
				UsrBeepReq &= ~(1 << i);
				MotorInBeepFlag = 1;
				break;
			}
		}
	} else {
		if(ToneNumCnt < ToneNum) {
			if(ToneTimesCnt < pTone[ToneNumCnt].Times) {
				if(_beep_sta == Beeping) {
					if(ToneTimeCnt == 0) {
						// start beep.
						StartMotorBeep(pTone);
					}
					ToneTimeCnt ++;
					if(ToneTimeCnt * millis >= pTone[ToneNumCnt].Time) {
						//stop beep.
						StopMotorBeep();
						ToneTimeCnt = 0;
						_beep_sta = Delaying;
					}
				} else if(_beep_sta == Delaying) {
					ToneDelayCnt ++;
					if(ToneDelayCnt * millis >= pTone[ToneNumCnt].Delay) {
						ToneDelayCnt = 0;
						_beep_sta = Beeping;
						ToneTimesCnt ++;
					}
				}
			} else {
				ToneTimesCnt = 0;
				ToneNumCnt ++;
			}
		} else {
			ToneNumCnt = 0;
			MotorInBeepFlag = 0;
		}
	}
}

static void StopMotorBeep(void)
{
	MOTOR_BEEP_VOLUME(0);
}

static void StartMotorBeep(const ToneDataDef *pTone)
{
	uint16_t v = pTone->Volume * (TIM2->ARR + 1) / 200;
	MOTOR_BEEP_FREQ(pTone->Freq << 1);
	MOTOR_BEEP_VOLUME(v);
}

void SendMotorBeepRequest(AudioTypeDef req)
{
	UsrBeepReq |= (1 << req);
}

void SetMotorControlParam(float pitch, float roll, float yaw, float thr)
{
	PitchOut = pitch;
	RollOut = roll;
	YawOut = yaw;
	ThrottleOut = thr;
}
