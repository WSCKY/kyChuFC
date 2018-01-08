#ifndef __WIFICP_H
#define __WIFICP_H

#include "CRC8.h"

#define WIFI_PROTOCOL_CRC_INIT                   (0x66)

#define APP_STX1             (0x55)
#define APP_STX2             (0xAA)

#define CRC_INIT             (0x66)

#define FILE_DATA_CACHE      (80) /* 16 * n */
#define PACKET_CACHE         (FILE_DATA_CACHE + 10)
#define PACKET_MAX_LENGTH    (PACKET_CACHE - 3)

typedef enum {
	DECODE_STATE_UNSYNCED = 0,
	DECODE_STATE_GOT_STX1 = 1,
	DECODE_STATE_GOT_STX2 = 2,
	DECODE_STATE_GOT_LEN  = 3,
	DECODE_STATE_GOT_TYPE = 4,
	DECODE_STATE_GOT_DATA = 5
} DECODE_STATE;

typedef enum {
	/* Heartbeat */
	TYPE_FC_APP_HEARTBEAT = 0x01,
	/* Common communication */
	TYPE_FC_Response = 0x11,
	TYPE_FC_VirtualCtrl = 0x15,
	/* Programmable */
	TYPE_ProgrammableTX = 0x22,
	TYPE_ProgrammableACK = 0x23,
	/* device check, see dev_check.txt */
	TYPE_DeviceCheckReq = 0x32,
	TYPE_DeviceCheckAck = 0x33,
	/* Emergency */
	TYPE_USER_ForceCmd = 0x44,
	/* Version & DSN */
	TYPE_VERSION_REQUEST = 0x66,
	TYPE_VERSION_Response = 0x67,
	/* Upgrade */
	TYPE_UPGRADE_REQUEST = 0x80,
	TYPE_UPGRADE_DATA = 0x81,
	TYPE_UPGRADE_FC_ACK = 0x82,
	/* FactoryTest Interface */
	TYPE_DSN_UPDATE = 0xA0,
	TYPE_ADC_CALIBRATE = 0xA1,
	TYPE_ADC_CALIB_ACK = 0xA2,
	TYPE_ESC_BURN_IN_TEST = 0xA3,
	TYPE_ACC_CALIBRATE = 0xA4,
	TYPE_ACC_CALIB_ACK = 0xA5,
	/* OptFlow Check. */
	TYPE_IMG_INFO_Req = 0xB0,
	TYPE_IMG_INFO_Ack = 0xB1,
	TYPE_FLOW_IMG_Req = 0xB2,
	TYPE_FLOW_IMG_Ack = 0xB3,
	/* Repair Support. */
	TYPE_CALIB_MTD_OptReq = 0xC0,
	/* Design for Debug */
	TYPE_DEBUG_CMD        = 0xE0,
	TYPE_WIFI_RC_RAW      = 0xE1,
} PACKET_TYPE;

typedef struct {
	uint16_t ExpRaw_Roll;
	uint16_t ExpRaw_Pitch;
	uint16_t ExpRaw_Throttle;
	uint16_t ExpRaw_Yaw;
	uint16_t ExpRaw_FLY;
	uint16_t ExpRaw_Rev;
} VirtualRC_Def;

typedef enum {
	FW_TYPE_NONE = 0,
	FW_TYPE_FC = 1,
} FW_TYPE;

typedef enum {
	IsReady = 0,
	InErasing = 1,
	Upgrading = 2,
	Refused = 3,
	JumpFailed = 4,
} FCUpgradeState;

typedef enum {
	FC_Busy = 0,
	VersionOld = 1,
	OverSize = 2,
	TypeError = 3,
	LowVoltage = 4,
	FWTypeError = 5,
	UnkonwError = 6,
	Error_None = 0xF,
} UpgradeRefusedType;

typedef enum {
	Program_Hover = 0x00,
	Program_Takeoff = 0x01,
	Program_Land = 0x02,
	Program_Forward = 0x03,
	Program_Backward = 0x04,
	Program_TwLeft = 0x05,
	Program_TwRight = 0x06,
	Program_UpWard = 0x07,
	Program_DownWard = 0x08,
	Program_RotateLeft = 0x09,
	Program_RotateRight = 0x0A
} ProgrammableAction;

/* -------- Heartbeat -------- */
typedef struct {
	uint8_t SignalCount;
} ComHeartbeatDef;

/* -------- Common Communication -------- */
typedef struct {
	uint8_t PackageIndex;
	uint8_t HW_InitRet;
	uint8_t SensorState;
	uint8_t LowPowerState;
	uint8_t FlyState;
	float EulerPitch;
	float EulerRoll;
	uint8_t eQuantity;
	uint8_t Voltage;
	float Altitude;
	float VelocityX;
	float VelocityY;
} FC_CommonResponseDef;

/* -------- Programmable -------- */
typedef struct {
	ProgrammableAction Action;
	float ActionParam1;
	float ActionParam2;
	uint8_t ActionIndex;
	uint8_t reserve[4];
} ProgrammableCmdDef;

typedef struct {
	ProgrammableAction Action;
	uint8_t Progress;
	uint8_t Exception;
	uint8_t ActionIndex;
	uint8_t reserve[4];
} ProgrammableRespDef;

/* -------- Device Check -------- */
typedef enum {
	_dev_Rev = 0x0,
	_dev_IMU = 0x1,
	_dev_Baro = 0x2,
	_dev_TOF = 0x3,
	_dev_Flow = 0x4,
	_dev_ADC = 0x5,
	_dev_ESC = 0x6,
	_dev_MTD = 0x7,
	_dev_LED = 0x8,
} CheckDeviceTypeDef;

typedef struct {
	CheckDeviceTypeDef dev;
	uint8_t Param1;
	uint8_t Param2;
	uint8_t Reserve[5];
} DeviceCheckReqTypeDef;

typedef enum {
	ERR_OK = 0,
	ERR_WARNING = 1,
	ERR_BAD = 2
} ErrCode;

typedef struct {
	CheckDeviceTypeDef dev;
	ErrCode DeviceState;
	float RetVal1;
	float RetVal2;
	float RetVal3;
	float RetVal4;
} DeviceCheckAckTypeDef;

/* -------- ForceCmd -------- */
typedef enum {
	Force_Cutoff = 0x01,
	Force_Poweroff = 0x02,
	Force_Land = 0x03,
} ForceCmdDef;

/* -------- Version & DSN -------- */
typedef struct {
	uint16_t FC_Version;
	uint16_t OP_Version;
	uint8_t DSN[16];
} VersionResponseDef;

/* -------- Upgrade -------- */
typedef struct { /* APP -> FC */
	FW_TYPE FW_Type;
	uint32_t PacketNum;
	uint32_t FileSize;
	uint16_t FW_Version;
	uint8_t DroneType;
	uint32_t FileCRC;
} FWInfoDef;

typedef struct { /* APP -> FC */
	uint32_t PacketID;
	uint8_t PacketLen;
	uint8_t PacketData[FILE_DATA_CACHE];
} UpgradeDataDef;

typedef struct { /* FC -> APP */
	FCUpgradeState FC_State;
	uint8_t reserve[5];
} FCResponseDef;

/* -------- Factory Test -------- */
#define VOLT_VERIFY_DATA 0xAA
typedef struct {
	uint8_t CalibCmd;
	uint8_t CalibCmdVerify;
} VoltCalibReqDef;

typedef struct {
	uint8_t CalibSta;
	uint8_t Progress;
	uint8_t Exception;
} VoltCalibStateDef;

#define DSN_VERIFY_DATA 0xBB
typedef struct {
	uint8_t _wDSN[16];
	uint8_t CheckByte;
} DSN_WriteInReqDef;

#define ESC_VERIFY_DATA 0xCC
typedef struct {
	uint8_t BurnInSpeed;
	uint8_t CheckByte;
} ESC_BurnInReqDef;

#define ACC_CALIB_VERIFY 0x5A
typedef struct {
	uint8_t CalibSides;
	uint8_t CurrentSide;
	uint8_t CurrentProg;
	uint8_t CalibStepInfo;
	uint8_t Reserve;
} AccCalibResponseDef;

#define MTD_VERIFY_DATA 0xDD
typedef struct {
	uint8_t OperationCmd;
	uint8_t CheckByte;
} MTD_OperateReqDef;

/* -------- OptFlow Check -------- */
#define FLOW_INF_REQ_CMD 0xAF
#define FLOW_IMG_REQ_CMD 0xFA
typedef struct {
	uint8_t PackageID;
	uint8_t NumOfPixels;
	uint8_t ImageData[80];
} OptFlowImgAckDef;

/* -------- Raw Wifi RC Data -------- */
#define RC_CHANNEL_NUMBER      12
typedef struct {
	uint16_t Channel[RC_CHANNEL_NUMBER];
} RawWifiRcDataDef;

/* Package type */
typedef union {
	uint8_t pData[PACKET_CACHE - 5];
	/* Heartbeat */
	ComHeartbeatDef Heartbeat;
	/* common communication */
	FC_CommonResponseDef FCStateResp;
	VirtualRC_Def VirtualRC;
	/* upgrade */
	FWInfoDef FileInfo;
	UpgradeDataDef PacketInfo;
	FCResponseDef FCActInfo;
	/* programmable */
	ProgrammableCmdDef ProgramCmd;
	ProgrammableRespDef ProgramResp;
	/* Device Check */
	DeviceCheckReqTypeDef DevChk_Req;
	DeviceCheckAckTypeDef DevChk_Ack;
	/* ForceCmd */
	ForceCmdDef ForceCmd;
	/* Version & DSN */
	VersionResponseDef VersionResp;
	/* Factory test */
	VoltCalibReqDef VoltCalibReq;
	VoltCalibStateDef VoltCalibSta;
	DSN_WriteInReqDef NewDSN;
	ESC_BurnInReqDef ESCBurnInCmd;
	AccCalibResponseDef AccCalibAck;
	/* OptFlow Check */
	OptFlowImgAckDef ImageData;
	/* Repair Support. */
	MTD_OperateReqDef MTDOptCmd;
	/* Wifi Rc Raw Data */
	RawWifiRcDataDef WifiRcRaw;
} PacketDataUnion;
/* Package structure */
typedef struct {
	uint8_t stx1;
	uint8_t stx2;
	uint8_t len;
	uint8_t type;
	PacketDataUnion PacketData;
	uint8_t crc;
} PackageStructDef;
/* Package Data */
typedef union {
	PackageStructDef Packet;
	uint8_t RawData[PACKET_CACHE];
} CommPackageDef;

#define COMMON_ACK_LEN                 (sizeof(FC_CommonResponseDef) + 2)
#define PROGRAM_ACK_LEN                (sizeof(ProgrammableRespDef) + 2)
#define DEV_CHK_ACK_LEN                (sizeof(DeviceCheckAckTypeDef) + 2)
#define UPGRADE_ACK_LEN                (sizeof(FCResponseDef) + 2)
#define VERSION_ACK_LEN                (sizeof(VersionResponseDef) + 2)
#define VOL_CALIB_ACK_LEN              (sizeof(VoltCalibStateDef) + 2)
#define ACC_CALIB_ACK_LEN              (sizeof(AccCalibResponseDef) + 2)

void Wifi_RX_Decode(uint8_t data);
uint8_t GetWifiPacketUpdateState(void);
CommPackageDef *GetWifiPacketPointer(void);

#endif /* __WIFICP_H */
