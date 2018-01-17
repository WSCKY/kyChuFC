#include "BARO_MPC2520.h"

static uint8_t _tx_rx_comp = 0;

static MPC2520_INIT_STRUCT _mpc2520_init_structure = {0};

static HAL_StatusTypeDef _BARO_Configure(void);
static HAL_StatusTypeDef _BARO_GetCalibParam(void);
static HAL_StatusTypeDef BARO_WriteReg(uint8_t reg, uint8_t val);
static HAL_StatusTypeDef BARO_ReadReg(uint8_t reg, uint8_t num, uint8_t *t, uint8_t *r);
static HAL_StatusTypeDef BARO_ReadRegUtil(uint8_t reg, uint8_t num, uint8_t *t, uint8_t *r);

HAL_StatusTypeDef BARO_MPC2520_Init(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	BARO_SPI_CS_DISABLE();
	ret = _BARO_Configure();
	return ret;
}

static HAL_StatusTypeDef _BARO_Configure(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	if((ret = BARO_WriteReg(RESET_FLUSH_REG, 0x89)) != HAL_OK) return ret; /* FIFO flush, soft reset */
	HAL_Delay(10);
	if((ret = _BARO_GetCalibParam()) != HAL_OK) return ret;
	if((ret = BARO_WriteReg(PRESSURE_CFG_REG, 0x77)) != HAL_OK) return ret; /* 128, 128 */
	if((ret = BARO_WriteReg(TEMPERATURE_CFG_REG, 0x77)) != HAL_OK) return ret; /* 128, 128 */
	if((ret = BARO_WriteReg(INTR_FIFO_CFG_REG, 0x0E)) != HAL_OK) return ret; /* Low, Disable, Disable, Disable, shift, shift, Enable, 4 */
	if((ret = BARO_WriteReg(MODE_STATUS_CFG_REG, 0x07)) != HAL_OK) return ret; /* Continuous pressure and temperature measurement */
	return ret;
}

static HAL_StatusTypeDef _BARO_GetCalibParam(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	uint8_t _tx_buffer[19];
	uint8_t _rx_buffer[19];
	ret = BARO_ReadRegUtil(CALIB_COEF_REG_START, 18, _tx_buffer, _rx_buffer);
	_mpc2520_init_structure.calib_param.c0 = ((int16_t)_rx_buffer[1] << 4) | ((int16_t)_rx_buffer[2] >> 4);
	_mpc2520_init_structure.calib_param.c1 = ((int16_t)(_rx_buffer[2] & 0x0F) << 8) | ((int16_t)_rx_buffer[3]);
	_mpc2520_init_structure.calib_param.c00 = ((int32_t)_rx_buffer[4] << 12 | (int32_t)_rx_buffer[5] << 4 | (int32_t)_rx_buffer[6] >> 4);
	_mpc2520_init_structure.calib_param.c10 = ((int32_t)_rx_buffer[6] << 16 | (int32_t)_rx_buffer[7] << 8 | (int32_t)_rx_buffer[8]);
	_mpc2520_init_structure.calib_param.c01 = ((int16_t)_rx_buffer[9] << 8 | (int16_t)_rx_buffer[10]);
	_mpc2520_init_structure.calib_param.c11 = ((int16_t)_rx_buffer[11] << 8 | (int16_t)_rx_buffer[12]);
	_mpc2520_init_structure.calib_param.c20 = ((int16_t)_rx_buffer[13] << 8 | (int16_t)_rx_buffer[14]);
	_mpc2520_init_structure.calib_param.c21 = ((int16_t)_rx_buffer[15] << 8 | (int16_t)_rx_buffer[16]);
	_mpc2520_init_structure.calib_param.c30 = ((int16_t)_rx_buffer[17] << 8 | (int16_t)_rx_buffer[18]);
	return ret;
}

static HAL_StatusTypeDef BARO_WriteReg(uint8_t reg, uint8_t val)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	uint8_t _tx_buffer[2];
	uint8_t _rx_buffer[2];
	BARO_SPI_CS_ENABLE();
	_tx_rx_comp = 0;
	_tx_buffer[0] = reg;
	_tx_buffer[1] = val;
	ret = BARO_TxRxData((uint8_t *)_tx_buffer, (uint8_t *)_rx_buffer, 2);
	if(ret == HAL_OK) {
		while (_tx_rx_comp == 0);
	}
	return ret;
}

static HAL_StatusTypeDef BARO_ReadReg(uint8_t reg, uint8_t num, uint8_t *t, uint8_t *r)
{
	BARO_SPI_CS_ENABLE();
	_tx_rx_comp = 0;
	t[0] = reg | 0x80;
	return BARO_TxRxData((uint8_t *)t, (uint8_t *)r, num + 1); //ignore the first byte.
}

static HAL_StatusTypeDef BARO_ReadRegUtil(uint8_t reg, uint8_t num, uint8_t *t, uint8_t *r)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	BARO_SPI_CS_ENABLE();
	_tx_rx_comp = 0;
	t[0] = reg | 0x80;
	ret = BARO_TxRxData((uint8_t *)t, (uint8_t *)r, num + 1); //ignore the first byte.
	if(ret == HAL_OK) {
		while (_tx_rx_comp == 0);
	}
	return ret;
}

void BARO_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	_tx_rx_comp = 1;
	BARO_SPI_CS_DISABLE();
}

void BARO_TxRxErrorCallback(SPI_HandleTypeDef *hspi)
{

}
