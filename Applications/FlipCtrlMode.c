#include "FlipCtrlMode.h"

#define FLIP_START_WAIT_TIME                     (200)

static uint8_t _init_flag = 0;
static EulerRPY *pDroneEluer;
static uint8_t _reversal_flag = 0;
static RF_COMMAND_DEF *pRF_CMD;
static IMU_UnitDataDef *pIMU;
static PID pidPitch = {0}, pidRoll = {0};//, pidYaw = {0};

static uint8_t DirTrigFlag = 0;
static uint8_t FlipStartTimeCnt = 0;
static uint8_t FlipEndWaitTimeCnt = 0;
static FLIP_STATE _flip_state = Flip_Stop;
static FLIP_DIRECTION _flip_dir = Flip_Left;

static void CtrlParamInit(uint8_t millis);

void FlipCtrlModeTask(uint8_t millis)
{
	static float integ_x, integ_y, cur_integ, flip_thr;
	static float exp_pit = 0.0f, exp_rol = 0.0f, exp_yaw = 0.0f;
	if(_init_flag == 0) {
		_init_flag = 1;

		CtrlParamInit(millis);
		pRF_CMD = GetRFCommand();
		pIMU = GetIMU_Unit_DATA();
		pDroneEluer = GetDroneEulerAngle();
	}
	if(pRF_CMD->RevTrigSwitch == DualState_Low && _flip_state == Flip_Ended) {
		_flip_state = Flip_Stop; // reset flip state.
	}
	if(pRF_CMD->RevTrigSwitch == DualState_High && _flip_state == Flip_Stop) {
		exp_pit = exp_rol = exp_yaw = 0;
		if(DirTrigFlag == 0) { // detect direction.
			if(pRF_CMD->Roll > RF_COMMAND_UNIT_MAX - RF_COMMAND_UNIT_DEADBAND) {
				_flip_dir = Flip_Right;
				DirTrigFlag = 1; // mark it.
			} else if(pRF_CMD->Roll < RF_COMMAND_UNIT_MIN + RF_COMMAND_UNIT_DEADBAND) {
				_flip_dir = Flip_Left;
				DirTrigFlag = 1; // mark it.
			} else if(pRF_CMD->Pitch < RF_COMMAND_UNIT_MIN + RF_COMMAND_UNIT_DEADBAND) {
				_flip_dir = Flip_Forward;
				DirTrigFlag = 1; // mark it.
			} else if(pRF_CMD->Pitch > RF_COMMAND_UNIT_MAX - RF_COMMAND_UNIT_DEADBAND) {
				_flip_dir = Flip_Backward;
				DirTrigFlag = 1; // mark it.
			}
		} else if(DirTrigFlag == 1) { // start flip.
			if(ABS((int16_t)pRF_CMD->Roll - (int16_t)RF_COMMAND_UNIT_MID) < 100 && \
			   ABS((int16_t)pRF_CMD->Pitch - (int16_t)RF_COMMAND_UNIT_MID) < 100) {
				DirTrigFlag = 0; // clear flag.
				FlipStartTimeCnt = 0;
				FlipEndWaitTimeCnt = 0;
				GyrIntegrationReset();
				flip_thr = pRF_CMD->Throttle - 1000;
				pidPitch.Isum = pidRoll.Isum = 0.0f;
				_flip_state = Flip_Start; //start.
			}
		}
	} else if(_flip_state != Flip_Start) {
		exp_pit = LinearMap(((RF_COMMAND_UNIT_MID << 1) - pRF_CMD->Pitch), RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -30.0f, 30.0f);
		exp_rol = LinearMap(pRF_CMD->Roll, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -30.0f, 30.0f);
		exp_yaw = LinearMap(((RF_COMMAND_UNIT_MID << 1) - pRF_CMD->Yaw), RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -180.0f, 180.0f);
		if(_reversal_flag == 1) {
			exp_pit = -exp_pit;
//			exp_rol = -exp_rol;
			exp_yaw = -exp_yaw;
		}
	}

	if(_flip_state != Flip_Start) {
		PID_LOOP(&pidPitch, exp_pit, pDroneEluer->Pitch);
		PID_LOOP(&pidRoll, exp_rol, pDroneEluer->Roll);
	} else {
		GetGyrIntegration(&integ_x, &integ_y);
		if(FlipStartTimeCnt * millis < FLIP_START_WAIT_TIME)
			FlipStartTimeCnt ++;
		else {
			switch(_flip_dir) {
			case Flip_Right:
				cur_integ = ABS(integ_x);
				if(exp_rol < 360.0f)
					exp_rol += 24.0f; /* about 150ms. */
				else {
					if(integ_x < 360) {
						if(FlipEndWaitTimeCnt * millis < 100) // wait 100ms.
							FlipEndWaitTimeCnt ++;
						else
							_flip_state = Flip_Ended;
					}
				}
			break;
			case Flip_Left:
				cur_integ = ABS(integ_x);
				if(exp_rol > -360.0f)
					exp_rol -= 24.0f; /* about 150ms. */
				else {
					if(integ_x > -360) {
						if(FlipEndWaitTimeCnt * millis < 100) // wait 100ms.
							FlipEndWaitTimeCnt ++;
						else
							_flip_state = Flip_Ended;
					}
				}
			break;
			case Flip_Forward:
				cur_integ = ABS(integ_y);
				if(exp_pit > -360.0f)
					exp_pit -= 24.0f; /* about 150ms. */
				else {
					if(integ_y > -360) {
						if(FlipEndWaitTimeCnt * millis < 100) // wait 100ms.
							FlipEndWaitTimeCnt ++;
						else
							_flip_state = Flip_Ended;
					}
				}
			break;
			case Flip_Backward:
				cur_integ = ABS(integ_y);
				if(exp_pit < 360.0f)
					exp_pit += 24.0f; /* about 150ms. */
				else {
					if(integ_y < 360) {
						if(FlipEndWaitTimeCnt * millis < 100) // wait 100ms.
							FlipEndWaitTimeCnt ++;
						else
							_flip_state = Flip_Ended;
					}
				}
			break;
			}
		}
		PID_LOOP(&pidPitch, exp_pit, integ_y);
		PID_LOOP(&pidRoll, exp_rol, integ_x);
	}

	if(GetFlyEnableState() == Disabled) {
		pidPitch.Isum = pidRoll.Isum = 0.0f;
		if(pIMU->Acc.Z < -0.0f) {
			_reversal_flag = 1;
			pDroneEluer = GetDroneFlipEulerAngle();
		} else {
			_reversal_flag = 0;
			pDroneEluer = GetDroneEulerAngle();
		}
	}

	if(_flip_state != Flip_Start) {
		SetInnerLoopExpParam(pidRoll.Output, pidPitch.Output, exp_yaw);
		SetDroneThrottle(pRF_CMD->Throttle - 1000);
	} else {
		SetInnerLoopExpParam(pidRoll.Output, pidPitch.Output, 0);
		if(FlipStartTimeCnt * millis < FLIP_START_WAIT_TIME) {
			if(flip_thr < 1000) flip_thr += 10;
		} else if(cur_integ <= 50) {
			flip_thr = 888;//LinearMap(integ_x, 0, 50, pRF_CMD->Throttle - 1000, 1000);
		} else if(cur_integ > 50 && cur_integ <= 90) {
			flip_thr = LinearMap(cur_integ, 50, 90, 888, 50);
		} else if(cur_integ > 90 && cur_integ <= 270) {
			flip_thr = 50;
		} else if(cur_integ <= 360) {
			if(flip_thr < 700) flip_thr = 700;
			else if(flip_thr < 1000) flip_thr += 10;
//			flip_thr = LinearMap(cur_integ, 270, 360, 666, 999); // throttle jump.
		} else { // out of 360deg.
			flip_thr = pRF_CMD->Throttle - 1000;
			_flip_state = Flip_Ended;
			pidPitch.Isum = pidRoll.Isum = 0.0f;
		}
		SetDroneThrottle(flip_thr);
	}

	if(_reversal_flag == 0)
		SetMotorRunDir(DIR_FORWARD);
	else
		SetMotorRunDir(DIR_NEGATER);
}

static void CtrlParamInit(uint8_t millis)
{
	pidPitch.kp = pidRoll.kp = 5.0f;
	pidPitch.ki = pidRoll.ki = 0.2f;
	pidPitch.kd = pidRoll.kd = 0.0f;

	pidPitch.Imax = pidRoll.Imax = 60.0f;
	pidPitch.Isum = pidRoll.Isum = 0.0f;
	pidPitch.preErr = pidRoll.preErr = 0.0f;

	pidPitch.dt = pidRoll.dt = (float)millis / 1000.0f;
}
