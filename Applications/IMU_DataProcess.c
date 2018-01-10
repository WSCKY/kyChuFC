#include "IMU_DataProcess.h"

static uint8_t _init_flag = 0;

static IMU_RawDataDef *_imu_raw;
static _3AxisFloat *_gyro_calib;
static IMU_RawDataDef _calib_raw = {0};
static IMU_UnitDataDef _imu_unit = {0};

void IMU_DataPreProcessTask(uint8_t millis)
{
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
}

IMU_UnitDataDef *GetIMU_Unit_DATA(void) { return &_imu_unit; }
