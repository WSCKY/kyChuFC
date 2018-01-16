#include "LEDIndicatorTask.h"

static uint8_t _init_flag = 0;
static uint32_t FlashTimeCnt = 0;
static uint8_t _power_on_loop = 0;

static uint8_t _led_ctrl_manually = 0;
static uint8_t _led_ctrl_injected = 0;

static void LedPwerOnLoop(uint8_t millis);
static void LedInjectedLoop(uint8_t millis);

static void _led_set_color(uint8_t color);

void LEDIndicatorTask(uint8_t millis)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		LED_RED_OFF();
		LED_BLUE_OFF();
		LED_GREEN_OFF();
	}

	if(_power_on_loop < 3) {
		LedPwerOnLoop(millis); return;
	}

	if(_led_ctrl_manually == 1) return;

	if(_led_ctrl_injected == 1) {
		LedInjectedLoop(millis);
		return;
	}

	if(FlashTimeCnt * millis < 1000) {
		if(GetBatteryState() >= LOW_POWER_LV2) { // low power level 2.
			if(((FlashTimeCnt * millis) / (500 / LED_FLASH_RATE_FAST)) % 2 == 0)
				_led_set_color(LED_COLOR_BLACK);
			else
				_led_set_color(LED_COLOR_RED);
		} else if(RfSignalIsLost()) {
			if(((FlashTimeCnt * millis) / (500 / LED_FLASH_RATE_FAST)) % 2 == 0)
				_led_set_color(LED_COLOR_BLACK);
			else
				_led_set_color(LED_COLOR_BLUE);
		} else {
			switch(GetFlightMode()) {
				case GROUP1_MODE1: _led_set_color(LED_COLOR_YELLOW); break;
				case GROUP1_MODE2: _led_set_color(LED_COLOR_PURPLE); break;
				case GROUP1_MODE3: _led_set_color(LED_COLOR_RED); break;
				case GROUP2_MODE1: _led_set_color(LED_COLOR_RED); break;
				case GROUP2_MODE2: _led_set_color(LED_COLOR_RED); break;
				case GROUP2_MODE3: _led_set_color(LED_COLOR_RED); break;
				case GROUP3_MODE1: _led_set_color(LED_COLOR_RED); break;
				case GROUP3_MODE2: _led_set_color(LED_COLOR_RED); break;
				case GROUP3_MODE3: _led_set_color(LED_COLOR_RED); break;
			}
		}
	} else if(GetBatteryState() >= LOW_POWER_LV1) {
		if(FlashTimeCnt * millis < 1000 + (500 / LED_FLASH_RATE_FAST)) _led_set_color(LED_COLOR_BLACK);
		else if(FlashTimeCnt * millis < 1000 + (1000 / LED_FLASH_RATE_FAST)) _led_set_color(LED_COLOR_RED);
		else if(FlashTimeCnt * millis < 1000 + (1500 / LED_FLASH_RATE_FAST)) _led_set_color(LED_COLOR_BLACK);
		else if(FlashTimeCnt * millis < 1000 + (2000 / LED_FLASH_RATE_FAST)) _led_set_color(LED_COLOR_RED);
		else FlashTimeCnt = 0;
	} else {
		FlashTimeCnt = 0;
	}
	FlashTimeCnt ++;
}

static void LedPwerOnLoop(uint8_t millis)
{
	FlashTimeCnt ++;
	if(FlashTimeCnt * millis < 150) { LED_RED_OFF(); LED_BLUE_ON(); LED_GREEN_OFF(); return; }
	if(FlashTimeCnt * millis < 300) { LED_RED_ON(); LED_BLUE_OFF(); LED_GREEN_OFF(); return; }
	if(FlashTimeCnt * millis < 450) { LED_RED_OFF(); LED_BLUE_OFF(); LED_GREEN_ON(); return; }
	FlashTimeCnt = 0; _power_on_loop += 1;
}

static void LedInjectedLoop(uint8_t millis)
{
	static uint8_t _time_cnt = 0;
	if(_time_cnt * millis < 80) { _led_set_color(LED_COLOR_YELLOW); return; }
	if(_time_cnt * millis < 250) { _led_set_color(LED_COLOR_BLACK); return; }
	if(_time_cnt * millis < 330) { _led_set_color(LED_COLOR_YELLOW); return; }
	if(_time_cnt * millis < 500) { _led_set_color(LED_COLOR_BLACK); return; }
	_time_cnt = 0; _led_ctrl_injected = 0;
}

static void _led_set_color(uint8_t color)
{
	if(color & LED_COLOR_RED) LED_RED_ON(); else LED_RED_OFF();
	if(color & LED_COLOR_BLUE) LED_BLUE_ON(); else LED_BLUE_OFF();
	if(color & LED_COLOR_GREEN) LED_GREEN_ON(); else LED_GREEN_OFF();
}

inline void LedCtrlInsertNotice(void) { _led_ctrl_injected = 1; }

inline void LedCtrlManuallyEnable(void) { _led_ctrl_manually = 1; }
inline void LedCtrlManuallyDisable(void) { _led_ctrl_manually = 0; }
