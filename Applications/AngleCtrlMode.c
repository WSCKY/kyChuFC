#include "AngleCtrlMode.h"

static uint8_t _init_flag = 0;
static EulerRPY *pDroneEluer;
static uint8_t _reversal_flag = 0;
static RF_COMMAND_DEF *pRF_CMD;
static IMU_UnitDataDef *pIMU;
static PID pidPitch = {0}, pidRoll = {0};//, pidYaw = {0};

static void CtrlParamInit(uint8_t millis);

void AngleCtrlModeTask(uint8_t millis)
{
	float exp_pit = 0.0f, exp_rol = 0.0f, exp_yaw = 0.0f;
	if(_init_flag == 0) {
		_init_flag = 1;

		CtrlParamInit(millis);
		pRF_CMD = GetRFCommand();
		pIMU = GetIMU_Unit_DATA();
		pDroneEluer = GetDroneEulerAngle();
	}

	exp_pit = LinearMap(((RF_COMMAND_UNIT_MID << 1) - pRF_CMD->Pitch), RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -30.0f, 30.0f);
	exp_rol = LinearMap(pRF_CMD->Roll, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -30.0f, 30.0f);
	exp_yaw = LinearMap(((RF_COMMAND_UNIT_MID << 1) - pRF_CMD->Yaw), RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -180.0f, 180.0f);
	if(_reversal_flag == 1) {
		exp_pit = -exp_pit;
//		exp_rol = -exp_rol;
		exp_yaw = -exp_yaw;
	}

	PID_LOOP(&pidPitch, exp_pit, pDroneEluer->Pitch);
	PID_LOOP(&pidRoll, exp_rol, pDroneEluer->Roll);

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

	SetInnerLoopExpParam(pidRoll.Output, pidPitch.Output, exp_yaw);//(0, 0, 0);//
	SetDroneThrottle(pRF_CMD->Throttle - 1000);
	if(_reversal_flag == 0)
		SetMotorRunDir(DIR_FORWARD);
	else
		SetMotorRunDir(DIR_NEGATER);
}

static void CtrlParamInit(uint8_t millis)
{
	pidPitch.kp = pidRoll.kp = 5.0f;
	pidPitch.ki = pidRoll.ki = 0.0f;
	pidPitch.kd = pidRoll.kd = 0.0f;

	pidPitch.Imax = pidRoll.Imax = 30.0f;
	pidPitch.Isum = pidRoll.Isum = 0.0f;
	pidPitch.preErr = pidRoll.preErr = 0.0f;

	pidPitch.dt = pidRoll.dt = (float)millis / 1000.0f;
}
