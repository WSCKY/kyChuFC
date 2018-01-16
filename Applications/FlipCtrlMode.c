#include "FlipCtrlMode.h"

static uint8_t _init_flag = 0;
static EulerRPY *pDroneEluer;
static uint8_t _reversal_flag = 0;
static RF_COMMAND_DEF *pRF_CMD;
static IMU_UnitDataDef *pIMU;
static PID pidPitch = {0}, pidRoll = {0};//, pidYaw = {0};

static uint8_t FlipReq = 0;
static uint8_t WaitTime = 0;
static uint8_t FlipStartTime = 0;
static uint8_t _in_flip_flag = 0;

static void CtrlParamInit(uint8_t millis);

void FlipCtrlModeTask(uint8_t millis)
{
	float integ_x, integ_y, flip_thr;
	static float exp_pit = 0.0f, exp_rol = 0.0f, exp_yaw = 0.0f;
	if(_init_flag == 0) {
		_init_flag = 1;

		CtrlParamInit(millis);
		pRF_CMD = GetRFCommand();
		pIMU = GetIMU_Unit_DATA();
		pDroneEluer = GetDroneEulerAngle();
	}
	if(pRF_CMD->RevTrigSwitch == DualState_Low) {
		_in_flip_flag = 0;//wait for rcommand.
	}
	if(pRF_CMD->RevTrigSwitch == DualState_High && _in_flip_flag == 0) {
		if((pRF_CMD->Roll > RF_COMMAND_UNIT_MID + 400) && FlipReq == 0) {
			FlipReq = 1;
		} else if(FlipReq == 1 && (pRF_CMD->Roll < RF_COMMAND_UNIT_MID + 100)) {
			FlipReq = 0;
			WaitTime = 0;
			flip_thr = pRF_CMD->Throttle - 1000;
			FlipStartTime = 0;
			_in_flip_flag = 1; //start.
			GyrIntegrationReset();
			exp_pit = exp_rol = exp_yaw = 0;
			pidPitch.Isum = pidRoll.Isum = 0.0f;
		}
	} else if(_in_flip_flag != 1) {
		exp_pit = LinearMap(((RF_COMMAND_UNIT_MID << 1) - pRF_CMD->Pitch), RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -30.0f, 30.0f);
		exp_rol = LinearMap(pRF_CMD->Roll, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -30.0f, 30.0f);
		exp_yaw = LinearMap(((RF_COMMAND_UNIT_MID << 1) - pRF_CMD->Yaw), RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -180.0f, 180.0f);
		if(_reversal_flag == 1) {
			exp_pit = -exp_pit;
//			exp_rol = -exp_rol;
			exp_yaw = -exp_yaw;
		}
	}

	GetGyrIntegration(&integ_x, &integ_y);
	if(_in_flip_flag != 1) {
		PID_LOOP(&pidPitch, exp_pit, pDroneEluer->Pitch);
		PID_LOOP(&pidRoll, exp_rol, pDroneEluer->Roll);
	} else {
		if(FlipStartTime < 50)
			FlipStartTime ++;
		else if(exp_rol < 360.0f)
			exp_rol += 24.0f;
		else {
			if(integ_x < 360) {
				if(WaitTime * millis < 100)
					WaitTime ++;
				else
					_in_flip_flag = 2;
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

	if(_in_flip_flag != 1) {
		SetInnerLoopExpParam(pidRoll.Output, pidPitch.Output, exp_yaw);
		SetDroneThrottle(pRF_CMD->Throttle - 1000);
	} else {
		SetInnerLoopExpParam(pidRoll.Output, pidPitch.Output, 0);
		if(FlipStartTime < 12)
			if(flip_thr < 1000)
				flip_thr += 8;
		if(integ_x <= 50) {
			flip_thr = 1000;//LinearMap(integ_x, 0, 50, pRF_CMD->Throttle - 1000, 1000);
		} else if(integ_x > 50 && integ_x <= 90) {
			flip_thr = LinearMap(integ_x, 50, 90, 1000, 50);
		} else if(integ_x > 90 && integ_x <= 270) {
			flip_thr = 50;
		} else if(integ_x <= 360) {
			flip_thr = LinearMap(integ_x, 270, 360, 50, 1000);
		} else {
			flip_thr = pRF_CMD->Throttle - 1000;
			_in_flip_flag = 2;
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
