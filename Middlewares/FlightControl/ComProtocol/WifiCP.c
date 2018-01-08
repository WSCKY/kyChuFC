#include "WifiCP.h"

static uint8_t GotDataFlag = 0;
static CommPackageDef rxPacket = {0};

static uint8_t _rx_length = 0;
static CommPackageDef _rx_packet = {0};
static DECODE_STATE _decode_state = DECODE_STATE_UNSYNCED;

uint8_t GetWifiPacketUpdateState(void)
{
	uint8_t ret = GotDataFlag;
	GotDataFlag = 0;
	return ret;
}

CommPackageDef *GetWifiPacketPointer(void) { return &rxPacket; }

void Wifi_RX_Decode(uint8_t data)
{
	switch(_decode_state) {
		case DECODE_STATE_UNSYNCED:
			if(data == APP_STX1) {
				_rx_packet.Packet.stx1 = APP_STX1;
				_decode_state = DECODE_STATE_GOT_STX1;
			}
		break;
		case DECODE_STATE_GOT_STX1:
			if(data == APP_STX2) {
				_rx_packet.Packet.stx2 = APP_STX2;
				_decode_state = DECODE_STATE_GOT_STX2;
			} else
				_decode_state = DECODE_STATE_UNSYNCED;
		break;
		case DECODE_STATE_GOT_STX2:
			if(data <= PACKET_MAX_LENGTH) {
				_rx_packet.Packet.len = data;
				_rx_length = 0;
				_decode_state = DECODE_STATE_GOT_LEN;
			} else {
				_decode_state = DECODE_STATE_UNSYNCED;
			}
		break;
		case DECODE_STATE_GOT_LEN:
			_rx_packet.Packet.type = data;
			_rx_length ++;
			_decode_state = DECODE_STATE_GOT_TYPE;
		break;
		case DECODE_STATE_GOT_TYPE:
			_rx_packet.Packet.PacketData.pData[_rx_length - 1] = data;
			_rx_length ++;
			if(_rx_length == _rx_packet.Packet.len - 1)
				_decode_state = DECODE_STATE_GOT_DATA;
		break;
		case DECODE_STATE_GOT_DATA:
			_rx_packet.Packet.crc = data;
			_rx_length ++;
			if(ComputeCRC8(&(_rx_packet.RawData[2]), _rx_length, CRC_INIT) == data) {
				rxPacket = _rx_packet;
				GotDataFlag = 1;
			} else {}
			_decode_state = DECODE_STATE_UNSYNCED;
		break;
		default:
			_decode_state = DECODE_STATE_UNSYNCED;
		break;
	}
}
