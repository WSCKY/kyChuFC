#include "BARO_DataProcess.h"

static uint8_t _init_flag = 0;

BARO_UnitDataDef *pBaroUnit;
static uint8_t _baro_read_time_cnt = 0;
static uint8_t _baro_read_init_cnt = 0;

static float _baro_altitude_off = 0.0f;
static float _baro_altitude_calibrated = 0.0f;

static uint8_t _baro_off_calibrated = 0;
static uint32_t _baro_calib_time_cnt = 0;

void BARO_DataProcessTask(uint8_t millis)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		pBaroUnit = GetBARO_UNIT_DATA();
		_baro_read_time_cnt = 0;
		_baro_read_init_cnt = 0;
		_baro_calib_time_cnt = 0;
		_baro_off_calibrated = 0;
	}

	_baro_read_time_cnt ++;
	if(_baro_read_time_cnt * millis < BARO_READ_DELAY_LIMIT) return;
	millis *= _baro_read_time_cnt;
	_baro_read_time_cnt = 0;

	BARO_MPC2520_Read();

	if(_baro_read_init_cnt ++ < 5) return; /* ignore the first few data. */

	if(_baro_off_calibrated == 0) {
		if(_baro_calib_time_cnt == 0)
			_baro_altitude_off = pBaroUnit->Altitude;
		else
			_baro_altitude_off = _baro_altitude_off * 0.9f + pBaroUnit->Altitude * 0.1f;

		if(_baro_calib_time_cnt * millis < BARO_OFFSET_LEVEL_CALIB_TIME)
			_baro_calib_time_cnt ++;
		else
			_baro_off_calibrated = 1;

		return;
	}

	_baro_altitude_calibrated = (pBaroUnit->Altitude - _baro_altitude_off) * 100.0f; /* convert to centimeter */

	if(GetFlyEnableState() == Disabled) {
		_baro_altitude_off = _baro_altitude_off * 0.9f + pBaroUnit->Altitude * 0.1f; /* keep follow the current altitude. */
	}
}

inline uint8_t GetBaroOffsetedFlag(void) { return _baro_off_calibrated; }

inline float GetBaroCalibratedAltitude(void) { return _baro_altitude_calibrated; }
inline float GetBaroMeasureTemperature(void) { return ((_init_flag) ? pBaroUnit->Temperature : 0.0f); }

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
