#include "RF_DataProcess.h"
#include "MotorControl.h"
static uint8_t _init_flag = 0;

static CommPackageDef *pWifiPacket;

static uint8_t SignalLostFlag = 1;
static uint32_t SignalLostTimeCnt = 0;

static RF_COMMAND_DEF _rf_command = {0};

void RF_DataProcessTask(uint8_t millis)
{
	uint8_t data;
	if(_init_flag == 0) {
		_init_flag = 1;
		pWifiPacket = GetWifiPacketPointer();
		_rf_command.Roll = 0; _rf_command.Pitch = 0; _rf_command.Yaw = 0; _rf_command.Throttle = 0;
		_rf_command.Mode = FlightMode_None; _rf_command.TrigSwitch = DualState_Low; _rf_command.CmdSwitch = TriState_Low;
	}
	while(WIFI_PORT_GET_BYTE(&data) == HAL_OK) {
		Wifi_RX_Decode(data);
	}

	if(GetWifiPacketUpdateState()) {
		SignalLostFlag = 0;
		SignalLostTimeCnt = 0;

		_rf_command.Roll = LinearMap(pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[0], RF_CHANNEL_RAW_MIN, RF_CHANNEL_RAW_MAX, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX);
		_rf_command.Pitch = LinearMap(pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[1], RF_CHANNEL_RAW_MIN, RF_CHANNEL_RAW_MAX, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX);
		_rf_command.Throttle = (RF_COMMAND_UNIT_MID << 1) - LinearMap(pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[2], RF_CHANNEL_RAW_MIN, RF_CHANNEL_RAW_MAX, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX);
		_rf_command.Yaw = LinearMap(pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[3], RF_CHANNEL_RAW_MIN, RF_CHANNEL_RAW_MAX, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX);
		_rf_command.Mode = (pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[6] < RF_CHANNEL_RAW_MIN + RF_CHANNEL_RAW_DEADBAND) ? \
		    FlightMode1 : ((pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[6] > RF_CHANNEL_RAW_MAX - RF_CHANNEL_RAW_DEADBAND) ? \
			FlightMode3 : FlightMode2);
		_rf_command.TrigSwitch = (pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[11] > RF_CHANNEL_RAW_MID + RF_CHANNEL_RAW_DEADBAND) ? \
			DualState_High : DualState_Low;
		_rf_command.CmdSwitch = (pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[10] < RF_CHANNEL_RAW_MIN + RF_CHANNEL_RAW_DEADBAND) ? \
				TriState_Low : ((pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[10] > RF_CHANNEL_RAW_MAX - RF_CHANNEL_RAW_DEADBAND) ? \
				TriState_High : TriState_Mid);
		_rf_command.RevChannel[0] = LinearMap(pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[4], RF_CHANNEL_RAW_MIN, RF_CHANNEL_RAW_MAX, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX);
		_rf_command.RevChannel[1] = LinearMap(pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[5], RF_CHANNEL_RAW_MIN, RF_CHANNEL_RAW_MAX, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX);
		_rf_command.RevChannel[2] = LinearMap(pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[7], RF_CHANNEL_RAW_MIN, RF_CHANNEL_RAW_MAX, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX);
		_rf_command.RevChannel[3] = LinearMap(pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[8], RF_CHANNEL_RAW_MIN, RF_CHANNEL_RAW_MAX, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX);
		_rf_command.RevChannel[4] = LinearMap(pWifiPacket->Packet.PacketData.WifiRcRaw.Channel[9], RF_CHANNEL_RAW_MIN, RF_CHANNEL_RAW_MAX, RF_COMMAND_UNIT_MIN, RF_COMMAND_UNIT_MAX);
	} else {
		if(SignalLostTimeCnt * millis < SIGNAL_LOST_CONFIRM_TIME) {
			SignalLostTimeCnt ++;
		} else {
			SignalLostFlag = 1;

			_rf_command.Roll = _rf_command.Pitch = _rf_command.Yaw = RF_COMMAND_UNIT_MID;
			_rf_command.Throttle = RF_COMMAND_UNIT_MIN;
		}
	}
}

inline uint8_t RfSignalIsLost(void) { return SignalLostFlag; }
inline RF_COMMAND_DEF *GetRFCommand(void) { return &_rf_command; }
