#include "FlightModeManage.h"

static uint8_t _init_flag = 0;
static RF_COMMAND_DEF *pRF_CMD = {0};

void FlightModeManageTask(uint8_t millis)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		pRF_CMD = GetRFCommand();
	}

	if(pRF_CMD->RevTriState[3] == TriState_Low) { /* Mode Group 1 */
		if(pRF_CMD->Mode == FlightMode1) {
			AngleCtrlModeTask(millis);
		} else if(pRF_CMD->Mode == FlightMode2) {
			FlipCtrlModeTask(millis);
		} else if(pRF_CMD->Mode == FlightMode3) {
			AngleCtrlModeTask(millis);
		} else {
			AngleCtrlModeTask(millis);
		}
	} else if(pRF_CMD->RevTriState[3] == TriState_Mid) { /* Mode Group 2 */
		if(pRF_CMD->Mode == FlightMode1) {
			AngleCtrlModeTask(millis);
		} else if(pRF_CMD->Mode == FlightMode2) {
			AngleCtrlModeTask(millis);
		} else if(pRF_CMD->Mode == FlightMode3) {
			AngleCtrlModeTask(millis);
		} else {
			AngleCtrlModeTask(millis);
		}
	} else if(pRF_CMD->RevTriState[3] == TriState_High) { /* Mode Group 3 */
		if(pRF_CMD->Mode == FlightMode1) {
			AngleCtrlModeTask(millis);
		} else if(pRF_CMD->Mode == FlightMode2) {
			AngleCtrlModeTask(millis);
		} else if(pRF_CMD->Mode == FlightMode3) {
			AngleCtrlModeTask(millis);
		} else {
			AngleCtrlModeTask(millis);
		}
	}
}
