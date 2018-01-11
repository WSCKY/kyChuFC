#include "IMU_DataProcess.h"

static uint8_t _init_flag = 0;

static IMU_RawDataDef *_imu_raw;
static _3AxisFloat *_gyro_calib;
static IMU_RawDataDef _calib_raw = {0};
static IMU_UnitDataDef _imu_unit = {0};

static Quaternion DroneQuaternion = {1, 0, 0, 0};
static EulerRPY DroneEulerAngle = {0};

void IMU_DataPreProcessTask(uint8_t millis)
{
	float kp = 5.0f;
	if(_init_flag == 0) {
		_init_flag = 1;

		_imu_raw = GetIMU_RAW_DATA();
		_gyro_calib = GetGyroCalibData();
	}
	IMU_ICM20602_Read();

	if(GyroIsCalibrated()) {
		_calib_raw.AccX = _imu_raw->AccX;
		_calib_raw.AccY = _imu_raw->AccY;
		_calib_raw.AccZ = _imu_raw->AccZ;
		_calib_raw.GyrX = _imu_raw->GyrX - _gyro_calib->X;
		_calib_raw.GyrY = _imu_raw->GyrY - _gyro_calib->Y;
		_calib_raw.GyrZ = _imu_raw->GyrZ - _gyro_calib->Z;
	} else {
		_calib_raw.AccX = _imu_raw->AccX;
		_calib_raw.AccY = _imu_raw->AccY;
		_calib_raw.AccZ = _imu_raw->AccZ;
		_calib_raw.GyrX = _imu_raw->GyrX;
		_calib_raw.GyrY = _imu_raw->GyrY;
		_calib_raw.GyrZ = _imu_raw->GyrZ;
	}
	IMU_ConvertRawToUnit(&_calib_raw, &_imu_unit);

	if(GetFlyEnableState() == Enabled) kp = 0.5f;
	else kp = 5.0f;
	FusionQuaternion6Axis(&_imu_unit, &DroneQuaternion, millis, kp, 0.0f);
	ConvertQuaternion2Euler(&DroneQuaternion, &DroneEulerAngle);
}

inline IMU_UnitDataDef *GetIMU_Unit_DATA(void) { return &_imu_unit; }
inline Quaternion *GetDroneQuaternion(void) { return &DroneQuaternion; }
inline EulerRPY *GetDroneEulerAngle(void) { return &DroneEulerAngle; }
