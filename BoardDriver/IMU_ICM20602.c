#include "IMU_ICM20602.h"

static uint16_t _sensor_id = 0xFFFF;
static IMU_RawDataDef _imu_raw = {0};
static int16_t _sensor_temp = 0;

static uint8_t _tx_rx_comp = 0;
static uint8_t _tx_buffer[15] = {0};
static uint8_t _rx_buffer[15] = {0};

static HAL_StatusTypeDef _IMU_Configure(void);
static HAL_StatusTypeDef IMU_WriteReg(uint8_t reg, uint8_t val);
static HAL_StatusTypeDef IMU_ReadReg(uint8_t reg, uint8_t num, uint8_t *r);
static HAL_StatusTypeDef IMU_ReadRegUtil(uint8_t reg, uint8_t num, uint8_t *r);

HAL_StatusTypeDef IMU_ICM20602_Init(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	IMU_SPI_CS_DISABLE();
	ret = _IMU_Configure();
	return ret;
}

HAL_StatusTypeDef IMU_ICM20602_Read(void)
{
	HAL_StatusTypeDef ret = HAL_BUSY;
//	if(_tx_rx_comp == 1) {
//		_tx_rx_comp = 0;
		ret = IMU_ReadReg(0x3B, 14, _rx_buffer);

		((int8_t *)&_imu_raw.AccX)[0] = _rx_buffer[2];
		((int8_t *)&_imu_raw.AccX)[1] = _rx_buffer[1];
		((int8_t *)&_imu_raw.AccY)[0] = _rx_buffer[4];
		((int8_t *)&_imu_raw.AccY)[1] = _rx_buffer[3];
		((int8_t *)&_imu_raw.AccZ)[0] = _rx_buffer[6];
		((int8_t *)&_imu_raw.AccZ)[1] = _rx_buffer[5];

		((int8_t *)&_sensor_temp)[0] = _rx_buffer[8];
		((int8_t *)&_sensor_temp)[1] = _rx_buffer[7];

		((int8_t *)&_imu_raw.GyrX)[0] = _rx_buffer[10];
		((int8_t *)&_imu_raw.GyrX)[1] = _rx_buffer[9];
		((int8_t *)&_imu_raw.GyrY)[0] = _rx_buffer[12];
		((int8_t *)&_imu_raw.GyrY)[1] = _rx_buffer[11];
		((int8_t *)&_imu_raw.GyrZ)[0] = _rx_buffer[14];
		((int8_t *)&_imu_raw.GyrZ)[1] = _rx_buffer[13];
//	}
	return ret;
}

void IMU_ConvertRawToUnit(IMU_RawDataDef *pRaw, IMU_UnitDataDef *pUnit)
{
	pUnit->Acc.X = pRaw->AccX * 0.002392578125f;
	pUnit->Acc.Y = pRaw->AccY * 0.002392578125f;
	pUnit->Acc.Z = pRaw->AccZ * 0.002392578125f;
	pUnit->Gyr.X = pRaw->GyrX * 0.06103515625f;
	pUnit->Gyr.Y = pRaw->GyrY * 0.06103515625f;
	pUnit->Gyr.Z = pRaw->GyrZ * 0.06103515625f;
}

IMU_RawDataDef *GetIMU_RAW_DATA(void) { return &_imu_raw; }

static HAL_StatusTypeDef _IMU_Configure(void)
{
	uint8_t i = 0;
//	uint16_t _reg_val = 0;
	HAL_StatusTypeDef ret = HAL_ERROR;
	if((ret = IMU_WriteReg(0x6B, 0x80)) != HAL_OK) return ret;
	HAL_Delay(10);
	if((ret = IMU_WriteReg(0x68, 0x03)) != HAL_OK) return ret;
	if((ret = IMU_WriteReg(0x6B, 0x00)) != HAL_OK) return ret;
	if((ret = IMU_WriteReg(0x6C, 0x00)) != HAL_OK) return ret;
	HAL_Delay(10);
	if((ret = IMU_WriteReg(0x70, 0x40)) != HAL_OK) return ret;
//	if((ret = IMU_ReadRegUtil(0x70, 1, (uint8_t *)&_reg_val)) != HAL_OK) return ret;
//	if((_reg_val >> 8) != 0x40) return HAL_ERROR;
	if((ret = IMU_WriteReg(0x37, 0x10)) != HAL_OK) return ret;
	if((ret = IMU_WriteReg(0x19, 0x00)) != HAL_OK) return ret;
	if((ret = IMU_WriteReg(0x1A, 0x03)) != HAL_OK) return ret;
	if((ret = IMU_WriteReg(0x1D, 0x06)) != HAL_OK) return ret;
	if((ret = IMU_WriteReg(0x1C, 0x10)) != HAL_OK) return ret;
	if((ret = IMU_WriteReg(0x1B, 0x18)) != HAL_OK) return ret;
	HAL_Delay(1);
	if((ret = IMU_ReadRegUtil(0x75, 1, (uint8_t *)&_sensor_id)) != HAL_OK) return ret;
	if((_sensor_id >> 8) != 0x12) return HAL_ERROR;
	for(i = 0; i < 15; i ++) _tx_buffer[i] = 0x00;
	return ret;
}

static HAL_StatusTypeDef IMU_WriteReg(uint8_t reg, uint8_t val)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	IMU_SPI_CS_ENABLE();
	_tx_rx_comp = 0;
	_tx_buffer[0] = reg;
	_tx_buffer[1] = val;
	ret = IMU_TxRxData((uint8_t *)_tx_buffer, (uint8_t *)_rx_buffer, 2);
	if(ret == HAL_OK) {
		while (_tx_rx_comp == 0);
	}
	return ret;
}

static HAL_StatusTypeDef IMU_ReadReg(uint8_t reg, uint8_t num, uint8_t *r)
{
	IMU_SPI_CS_ENABLE();
	_tx_rx_comp = 0;
	_tx_buffer[0] = reg | 0x80;
	return IMU_TxRxData((uint8_t *)_tx_buffer, (uint8_t *)r, num + 1); //ignore the first byte.
}

static HAL_StatusTypeDef IMU_ReadRegUtil(uint8_t reg, uint8_t num, uint8_t *r)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	IMU_SPI_CS_ENABLE();
	_tx_rx_comp = 0;
	_tx_buffer[0] = reg | 0x80;
	ret = IMU_TxRxData((uint8_t *)_tx_buffer, (uint8_t *)r, num + 1); //ignore the first byte.
	if(ret == HAL_OK) {
		while (_tx_rx_comp == 0);
	}
	return ret;
}

void IMU_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	_tx_rx_comp = 1;
	IMU_SPI_CS_DISABLE();
}

void IMU_TxRxErrorCallback(SPI_HandleTypeDef *hspi)
{

}
