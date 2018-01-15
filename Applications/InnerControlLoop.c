#include "InnerControlLoop.h"

static uint8_t _init_flag = 0;
static float exp_gx = 0.0f, exp_gy = 0.0f, exp_gz = 0.0f;
static PID pidPitRate = {0}, pidRolRate = {0}, pidYawRate = {0};

static IMU_UnitDataDef *pIMU;

static void CtrlParamInit(uint8_t millis);

void InnerLoopCtrlTask(uint8_t millis)
{
	static float gx = 0.0f, gy = 0.0f, gz = 0.0f;
	if(_init_flag == 0) {
		_init_flag = 1;

		CtrlParamInit(millis);
		pIMU = GetIMU_Unit_DATA();
	}
	gx = gx * 0.6f + pIMU->Gyr.X * 0.4f;
	gy = gy * 0.6f + pIMU->Gyr.Y * 0.4f;
	gz = gz * 0.6f + pIMU->Gyr.Z * 0.4f;

	PID_LOOP(&pidPitRate, exp_gy, gy);
	PID_LOOP(&pidRolRate, exp_gx, gx);
	PID_LOOP(&pidYawRate, exp_gz, gz);

	if(GetFlyEnableState() == Disabled) {
		pidPitRate.Isum = pidRolRate.Isum = pidYawRate.Isum = 0.0f;
	}

	SetMotorControlParam(-pidPitRate.Output, +pidRolRate.Output, -pidYawRate.Output);
}

static void CtrlParamInit(uint8_t millis)
{
	pidPitRate.kp = pidRolRate.kp = 0.5f;
	pidPitRate.ki = pidRolRate.ki = 0.2f;
	pidPitRate.kd = pidRolRate.kd = 0.0f;

	pidPitRate.Imax = pidRolRate.Imax = 100.0f;
	pidPitRate.Isum = pidRolRate.Isum = 0.0f;
	pidPitRate.preErr = pidRolRate.preErr = 0.0f;

	pidYawRate.kp = 3.0f;
	pidYawRate.ki = 0.3f;
	pidYawRate.kd = 0.0f;
	pidYawRate.Imax = 50.0f;
	pidYawRate.Isum = 0.0f;
	pidYawRate.preErr = 0.0f;

	pidPitRate.dt = pidRolRate.dt = pidYawRate.dt = (float)millis / 1000.0f;
}

void SetInnerLoopExpParam(float gx, float gy, float gz)
{
	exp_gx = gx; exp_gy = gy; exp_gz = gz;
}
