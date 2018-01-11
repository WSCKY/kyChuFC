#include "AngleCtrlMode.h"

static uint8_t _init_flag = 0;
static EulerRPY *pDroneEluer;
static RF_COMMAND_DEF *pRF_CMD = {0};
static PID pidPitch = {0}, pidRoll = {0};//, pidYaw = {0};

static void CtrlParamInit(uint8_t millis);

void AngleCtrlModeTask(uint8_t millis)
{
	float exp_pit = 0.0f, exp_rol = 0.0f, exp_yaw = 0.0f;
	if(_init_flag == 0) {
		_init_flag = 1;

		CtrlParamInit(millis);
		pRF_CMD = GetRFCommand();
		pDroneEluer = GetDroneEulerAngle();
	}

	exp_pit = LinearMap(((RF_COMMAND_UNIT_MID << 1) - pRF_CMD->Pitch), RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -30.0f, 30.0f);
	exp_rol = LinearMap(pRF_CMD->Roll, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -30.0f, 30.0f);
	exp_yaw = LinearMap(((RF_COMMAND_UNIT_MID << 1) - pRF_CMD->Yaw), RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX, -180.0f, 180.0f);

	PID_LOOP(&pidPitch, exp_pit, pDroneEluer->Pitch);
	PID_LOOP(&pidRoll, exp_rol, pDroneEluer->Roll);

	if(GetFlyEnableState() == Disabled) {
		pidPitch.Isum = pidRoll.Isum = 0.0f;
	}

	SetInnerLoopExpParam(pidRoll.Output, pidPitch.Output, exp_yaw);//(0, 0, 0);//
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
