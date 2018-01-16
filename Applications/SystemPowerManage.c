#include "SystemPowerManage.h"

static void _shut_down(void);

static float _system_volt = 3.7f;
static uint32_t BatteryLowPowerTimeCnt = 0;
static uint32_t BatteryPreLowPowerTimeCnt = 0;
static BATTERY_STATE BatteryState = POWER_ENOUGH;

static float _volt_calib_k = 1.0f, _volt_calib_b = 0.0f;

void BatteryVoltageCheckTask(uint8_t millis)
{
	uint16_t _raw = BATTERY_VOLT_RAW_VAL;
	float _volt = VOLT_MEASURE_COEF * MCU_REF_VOLTAGE * _raw / 4096.0f;
	/* apply low pass filter */
	_system_volt = _system_volt * 0.95f + (_volt_calib_k * _volt + _volt_calib_b) * 0.05f;

	if(_system_volt > BATTERY_PRELOW_POWER_VOLTAGE) {
		BatteryLowPowerTimeCnt = 0;
		BatteryPreLowPowerTimeCnt = 0;
	} else if(_system_volt > BATTERY_LOW_POWER_VOLTAGE) {
		if(BatteryPreLowPowerTimeCnt * millis < 1000)
			BatteryPreLowPowerTimeCnt ++;
		else if(BatteryState == POWER_ENOUGH)
			BatteryState = LOW_POWER_LV1;
	} else if(_system_volt > BATTERY_BAD_VOLTAGE) {
		if(BatteryLowPowerTimeCnt * millis < 1000)
			BatteryLowPowerTimeCnt ++;
		else
			BatteryState = LOW_POWER_LV2;
	} else {
		BatteryState = BAD_BATTERY;
		_shut_down(); // force to shut down.
	}
}

void SetVoltageCalibParam(float _k, float _b) { _volt_calib_k = _k; _volt_calib_b = _b; }

float GetSystemVoltage(void) { return _system_volt; }

static uint8_t PowerOffRequestFlag = 0;

static KEY_STATE _key_release_init = Key_Pressed;
static KEY_STATE _key_state = Key_Released;
static uint32_t KeyPressTimeCnt = 0;

void DroneAutoPowerOffTask(uint8_t millis)
{
	_key_state = SYSTEM_POWER_KEY_STATE();
	if(_key_state == Key_Pressed) {
		if(_key_release_init == Key_Released) {
			if(KeyPressTimeCnt * millis < POWEROFF_CONFIRM_TIME)
				KeyPressTimeCnt ++;
			else
				PowerOffRequestFlag = 1;
		}
	} else {
		KeyPressTimeCnt = 0;
		_key_release_init = Key_Released;
	}

	if(PowerOffRequestFlag == 1) {
		_shut_down();
	}
}

void SendSystemPowerOffReq(void) { PowerOffRequestFlag = 1; }

static void _shut_down(void)
{
	LED_RED_OFF();
	LED_BLUE_OFF();
	LED_GREEN_OFF();

	SYSTEM_POWER_OFF();
	SYSTEM_ENTER_STOP_MODE();
	for(;;);
}
