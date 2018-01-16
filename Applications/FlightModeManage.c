#include "FlightModeManage.h"

static uint8_t _init_flag = 0;
static RF_COMMAND_DEF *pRF_CMD = {0};

static FLIGHT_MODE _flight_mode = GROUP1_MODE1;

void FlightModeManageTask(uint8_t millis)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		pRF_CMD = GetRFCommand();
	}

	if(pRF_CMD->RevTriState[3] == TriState_Low) { /* Mode Group 1 */
		if(pRF_CMD->Mode == FlightMode1) {
			AngleCtrlModeTask(millis);
			_flight_mode = GROUP1_MODE1;
		} else if(pRF_CMD->Mode == FlightMode2) {
			FlipCtrlModeTask(millis);
			_flight_mode = GROUP1_MODE2;
		} else if(pRF_CMD->Mode == FlightMode3) {
			AngleCtrlModeTask(millis);
			_flight_mode = GROUP1_MODE3;
		} else {
			// add warning here ...
			AngleCtrlModeTask(millis);
			_flight_mode = GROUP1_MODE1;
		}
	} else if(pRF_CMD->RevTriState[3] == TriState_Mid) { /* Mode Group 2 */
		if(pRF_CMD->Mode == FlightMode1) {
			AngleCtrlModeTask(millis);
			_flight_mode = GROUP2_MODE1;
		} else if(pRF_CMD->Mode == FlightMode2) {
			AngleCtrlModeTask(millis);
			_flight_mode = GROUP2_MODE2;
		} else if(pRF_CMD->Mode == FlightMode3) {
			AngleCtrlModeTask(millis);
			_flight_mode = GROUP2_MODE3;
		} else {
			// add warning here ...
			AngleCtrlModeTask(millis);
			_flight_mode = GROUP2_MODE1;
		}
	} else if(pRF_CMD->RevTriState[3] == TriState_High) { /* Mode Group 3 */
		if(pRF_CMD->Mode == FlightMode1) {
			AngleCtrlModeTask(millis);
			_flight_mode = GROUP3_MODE1;
		} else if(pRF_CMD->Mode == FlightMode2) {
			AngleCtrlModeTask(millis);
			_flight_mode = GROUP3_MODE2;
		} else if(pRF_CMD->Mode == FlightMode3) {
			AngleCtrlModeTask(millis);
			_flight_mode = GROUP3_MODE3;
		} else {
			// add warning here ...
			AngleCtrlModeTask(millis);
			_flight_mode = GROUP3_MODE1;
		}
	}
}

inline FLIGHT_MODE GetFlightMode(void) { return _flight_mode; }
