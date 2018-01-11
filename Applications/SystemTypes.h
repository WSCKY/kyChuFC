#ifndef __SYSTEMTYPES_H
#define __SYSTEMTYPES_H

#include "stm32f7xx.h"

typedef struct {
	float X, Y;
} _2AxisFloat;

typedef struct {
	double X, Y;
} _2AxisDouble;

typedef struct {
	uint16_t X, Y;
} _2AxisUINT;

typedef struct {
	float X, Y, Z;
} _3AxisFloat;

typedef struct {
	double X, Y, Z;
} _3AxisDouble;

typedef struct {
	uint16_t X, Y, Z;
} _3AxisUINT;

typedef struct {
	int16_t X, Y, Z;
} _3AxisINT;

typedef struct {
	int16_t AccX;
	int16_t AccY;
	int16_t AccZ;
	int16_t GyrX;
	int16_t GyrY;
	int16_t GyrZ;
} IMU_RawDataDef;

typedef struct {
	_3AxisFloat Acc;
	_3AxisFloat Gyr;
} IMU_UnitDataDef;

typedef struct {
	float qw, qx, qy, qz;
} Quaternion;

typedef struct {
	float Roll, Pitch, Yaw;
} EulerRPY;

typedef struct {
	float kp, ki, kd, preErr, Pout, Iout, Dout, Output, Imax, Isum, dt;
} PID;

typedef struct {
	uint16_t Freq;
	uint8_t Volume;
	uint16_t Time;
	uint16_t Delay;
	uint16_t Times;
} ToneDataDef;

typedef struct {
	const ToneDataDef *pTone;
	uint8_t NumTones;
} ToneBuffDef;

#endif /* __SYSTEMTYPES */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
