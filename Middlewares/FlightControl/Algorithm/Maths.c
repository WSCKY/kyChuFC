#include "Maths.h"

void FusionQuaternion6Axis(IMU_UnitDataDef *pIMU, Quaternion *Q, uint8_t millis, float kp, float ki)
{
	float dt = millis * 0.001f;
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;

	float twoKp = kp;    // 2 * proportional gain (Kp)
	float twoKi = ki;    // 2 * integral gain (Ki)

	static float integralFBx = 0.0f;
	static float integralFBy = 0.0f;
	static float integralFBz = 0.0f;  // integral error terms scaled by Ki

	float qw, qx, qy, qz;
	float gx, gy, gz;
	float ax, ay, az;

	qw = Q->qw;
	qx = Q->qx;
	qy = Q->qy;
	qz = Q->qz;

	gx = pIMU->Gyr.X * DEG2RAD; gy = pIMU->Gyr.Y * DEG2RAD; gz = pIMU->Gyr.Z * DEG2RAD;
	ax = pIMU->Acc.X; ay = pIMU->Acc.Y; az = pIMU->Acc.Z;

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
	{
		// Normalise accelerometer measurement
		recipNorm = 1.0f/sqrtf(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Estimated direction of gravity and vector perpendicular to magnetic flux
		halfvx = qx * qz - qw * qy;
		halfvy = qw * qx + qy * qz;
		halfvz = qw * qw - 0.5f + qz * qz;

		// Error is sum of cross product between estimated and measured direction of gravity
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// Compute and apply integral feedback if enabled

		integralFBx += twoKi * halfex * dt;  // integral error scaled by Ki
		integralFBy += twoKi * halfey * dt;
		integralFBz += twoKi * halfez * dt;
		gx += integralFBx;  // apply integral feedback
		gy += integralFBy;
		gz += integralFBz;

		// Apply proportional feedback
		gx += twoKp * halfex;
		gy += twoKp * halfey;
		gz += twoKp * halfez;
	}

	float delta2 = (gx*gx + gy*gy + gz*gz)*dt*dt;

	float qw_last = qw;
	float qx_last = qx;
	float qy_last = qy;
	float qz_last = qz;

	qw = qw_last*(1.0f-delta2*0.125f) + (-qx_last*gx - qy_last*gy - qz_last*gz)*0.5f * dt;
	qx = qx_last*(1.0f-delta2*0.125f) + (qw_last*gx + qy_last*gz - qz_last*gy)*0.5f * dt;
	qy = qy_last*(1.0f-delta2*0.125f) + (qw_last*gy - qx_last*gz + qz_last*gx)*0.5f * dt;
	qz = qz_last*(1.0f-delta2*0.125f) + (qw_last*gz + qx_last*gy - qy_last*gx)*0.5f * dt;

	// Normalise quaternion
	recipNorm = 1.0f/sqrtf(qw * qw + qx * qx + qy * qy + qz * qz);
	qw *= recipNorm;
	qx *= recipNorm;
	qy *= recipNorm;
	qz *= recipNorm;

	Q->qw = qw;
	Q->qx = qx;
	Q->qy = qy;
	Q->qz = qz;
}

void ConvertQuaternion2Euler(Quaternion* Q, EulerRPY* eur)
{
	float qw, qx, qy, qz;

	qw = Q->qw;
	qx = Q->qx;
	qy = Q->qy;
	qz = Q->qz;

	eur->Roll    = atan2f(2 * (qw * qx + qy * qz) , 1 - 2 * (qx * qx + qy * qy))*RAD2DEG;  //+-90
	eur->Pitch   = asinf(2 * (qw * qy - qz * qx))*RAD2DEG;                                 //+-180
	eur->Yaw     = atan2f(2 * (qw * qz + qx * qy) , 1 - 2 * (qy * qy + qz * qz))*RAD2DEG;  //+-180
}

float LinearMap(float in, float in_min, float in_max, float tar_min, float tar_max)
{
	return ((((tar_max - tar_min) * (in - in_min)) / (in_max - in_min)) + tar_min);
}

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
