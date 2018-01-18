#include "BARO_MPC2520.h"
#include <math.h>

static uint8_t _tx_rx_comp = 0;
static uint8_t _sensor_raw_rx[7] = {0};
static BARO_UnitDataDef _baro_unit_data = {0};
static MPC2520_INIT_STRUCT _mpc2520_init_structure = {0};

static const unsigned _msl_pressure = 101325;
/* tropospheric properties (0-11km) for standard atmosphere */
static const double T1 = 15.0 + 273.15;	/* temperature at base height in Kelvin */
static const double a  = -6.5 / 1000;	/* temperature gradient in degrees per metre */
static const double g  = 9.80665;	/* gravity constant in m/s/s */
static const double R  = 287.05;	/* ideal gas constant in J/kg/K */

static HAL_StatusTypeDef _BARO_Configure(void);
static HAL_StatusTypeDef MPC2520_ReadRaw(void);
static HAL_StatusTypeDef _BARO_GetCalibParam(void);
static void BARO_ConvertRawToUnit(void);
static HAL_StatusTypeDef BARO_WriteReg(uint8_t reg, uint8_t val);
static HAL_StatusTypeDef BARO_ReadReg(uint8_t reg, uint8_t num, uint8_t *r);
static HAL_StatusTypeDef BARO_ReadRegUtil(uint8_t reg, uint8_t num, uint8_t *r);

HAL_StatusTypeDef BARO_MPC2520_Init(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	BARO_SPI_CS_DISABLE();
	ret = _BARO_Configure();
	return ret;
}

HAL_StatusTypeDef BARO_MPC2520_Read(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	if(_tx_rx_comp == 1) {
		ret = MPC2520_ReadRaw();
	}
	BARO_ConvertRawToUnit();
	return ret;
}

BARO_UnitDataDef *GetBARO_UNIT_DATA(void) { return &_baro_unit_data; }

static HAL_StatusTypeDef _BARO_Configure(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	if((ret = BARO_WriteReg(RESET_FLUSH_REG, 0x89)) != HAL_OK) return ret; /* FIFO flush, soft reset */
	HAL_Delay(10);
	if((ret = BARO_ReadRegUtil(PRODUCT_REV_ID_REG, 1, _sensor_raw_rx)) != HAL_OK) return ret;
	if(_sensor_raw_rx[1] != MPC2520_CHIP_ID) return HAL_ERROR;
	/*
	 * Wait for PROM contents to be in the device (2.8 ms) in the case we are
	 * called immediately after reset.
	 */
	HAL_Delay(30);
	if((ret = _BARO_GetCalibParam()) != HAL_OK) return ret;
	if((ret = BARO_WriteReg(PRESSURE_CFG_REG, 0x53)) != HAL_OK) return ret; /* sample rate: 32, over sample: 8 */
	if((ret = BARO_WriteReg(TEMPERATURE_CFG_REG, 0xD3)) != HAL_OK) return ret; /* sample rate: 32, over sample: 8 */
	if((ret = BARO_WriteReg(INTR_FIFO_CFG_REG, 0x00)) != HAL_OK) return ret; /* Low, Disable, Disable, Disable, no-shift, no-shift, Disable, 4 */
	if((ret = BARO_WriteReg(MODE_STATUS_CFG_REG, 0x07)) != HAL_OK) return ret; /* Continuous pressure and temperature measurement */
	_mpc2520_init_structure.kP = 7864320;
	_mpc2520_init_structure.kT = 7864320;
	return ret;
}

static HAL_StatusTypeDef MPC2520_ReadRaw(void)
{
	HAL_StatusTypeDef ret = HAL_BUSY;
	int32_t raw;
	ret = BARO_ReadReg(PRESSURE_MSB_REG, 6, _sensor_raw_rx);

	raw = (int32_t)(_sensor_raw_rx[1] << 16) | (int32_t)(_sensor_raw_rx[2] << 8) | (int32_t)(_sensor_raw_rx[3]);
	raw = (raw & 0x800000) ? (0xFF000000 | raw) : raw;
	_mpc2520_init_structure.pressure_raw = raw;

	raw = (int32_t)(_sensor_raw_rx[4] << 16) | (int32_t)(_sensor_raw_rx[5] << 8) | (int32_t)(_sensor_raw_rx[6]);
	raw = (raw & 0x800000) ? (0xFF000000 | raw) : raw;
	_mpc2520_init_structure.temperature_raw = raw;
	return ret;
}

static HAL_StatusTypeDef _BARO_GetCalibParam(void)
{
	HAL_StatusTypeDef ret = HAL_ERROR;

	/* read and convert calibration parameter c0 */
	if((ret = BARO_ReadRegUtil(0x10, 2, _sensor_raw_rx)) != HAL_OK) return ret;
	_mpc2520_init_structure.calib_param.c0 = (int16_t)_sensor_raw_rx[1] << 4 | _sensor_raw_rx[2] >> 4;
	_mpc2520_init_structure.calib_param.c0 = (_mpc2520_init_structure.calib_param.c0 & 0x0800) ? (0xF000 | _mpc2520_init_structure.calib_param.c0) : _mpc2520_init_structure.calib_param.c0;

	/* read and convert calibration parameter c1 */
	if((ret = BARO_ReadRegUtil(0x11, 2, _sensor_raw_rx)) != HAL_OK) return ret;
	_mpc2520_init_structure.calib_param.c1 = (int16_t)(_sensor_raw_rx[1] & 0x0F) << 8 | _sensor_raw_rx[2];
	_mpc2520_init_structure.calib_param.c1 = (_mpc2520_init_structure.calib_param.c1 & 0x0800) ? (0xF000 | _mpc2520_init_structure.calib_param.c1) : _mpc2520_init_structure.calib_param.c1;

	/* read and convert calibration parameter c00 */
	if((ret = BARO_ReadRegUtil(0x13, 3, _sensor_raw_rx)) != HAL_OK) return ret;
	_mpc2520_init_structure.calib_param.c00 = (int32_t)_sensor_raw_rx[1] << 12 | (int32_t)_sensor_raw_rx[2] << 4 | (int32_t)_sensor_raw_rx[3] >> 4;
	_mpc2520_init_structure.calib_param.c00 = (_mpc2520_init_structure.calib_param.c00 & 0x080000) ? (0xFFF00000 | _mpc2520_init_structure.calib_param.c00) : _mpc2520_init_structure.calib_param.c00;

	/* read and convert calibration parameter c10 */
	if((ret = BARO_ReadRegUtil(0x15, 3, _sensor_raw_rx)) != HAL_OK) return ret;
	_mpc2520_init_structure.calib_param.c10 = (int32_t)_sensor_raw_rx[1] << 16 | (int32_t)_sensor_raw_rx[2] << 8 | _sensor_raw_rx[3];
	_mpc2520_init_structure.calib_param.c10 = (_mpc2520_init_structure.calib_param.c10 & 0x080000) ? (0xFFF00000 | _mpc2520_init_structure.calib_param.c10) : _mpc2520_init_structure.calib_param.c10;

	/* read and convert calibration parameter c01 */
	if((ret = BARO_ReadRegUtil(0x18, 2, _sensor_raw_rx)) != HAL_OK) return ret;
	_mpc2520_init_structure.calib_param.c01 = (int16_t)_sensor_raw_rx[1] << 8 | _sensor_raw_rx[2];

	/* read and convert calibration parameter c11 */
	if((ret = BARO_ReadRegUtil(0x1A, 2, _sensor_raw_rx)) != HAL_OK) return ret;
	_mpc2520_init_structure.calib_param.c11 = (int16_t)_sensor_raw_rx[1] << 8 | _sensor_raw_rx[2];

	/* read and convert calibration parameter c20 */
	if((ret = BARO_ReadRegUtil(0x1C, 2, _sensor_raw_rx)) != HAL_OK) return ret;
	_mpc2520_init_structure.calib_param.c20 = (int16_t)_sensor_raw_rx[1] << 8 | _sensor_raw_rx[2];

	/* read and convert calibration parameter c21 */
	if((ret = BARO_ReadRegUtil(0x1E, 2, _sensor_raw_rx)) != HAL_OK) return ret;
	_mpc2520_init_structure.calib_param.c21 = (int16_t)_sensor_raw_rx[1] << 8 | _sensor_raw_rx[2];

	/* read and convert calibration parameter c30 */
	if((ret = BARO_ReadRegUtil(0x20, 2, _sensor_raw_rx)) != HAL_OK) return ret;
	_mpc2520_init_structure.calib_param.c30 = (int16_t)_sensor_raw_rx[1] << 8 | _sensor_raw_rx[2];

	if(_mpc2520_init_structure.calib_param.c0 == 0 && \
	   _mpc2520_init_structure.calib_param.c1 == 0 && \
	   _mpc2520_init_structure.calib_param.c00 == 0 && \
	   _mpc2520_init_structure.calib_param.c10 == 0 && \
	   _mpc2520_init_structure.calib_param.c01 == 0 && \
	   _mpc2520_init_structure.calib_param.c11 == 0 && \
	   _mpc2520_init_structure.calib_param.c20 == 0 && \
	   _mpc2520_init_structure.calib_param.c21 == 0 && \
	   _mpc2520_init_structure.calib_param.c30 == 0) {
		return HAL_ERROR;
	}

	return ret;
}

static void BARO_ConvertRawToUnit(void)
{
	double fTsc, fPsc;
	double qua2, qua3;
	double _fPCompensate;
	fTsc = _mpc2520_init_structure.temperature_raw / (double)_mpc2520_init_structure.kT;
	_baro_unit_data.Temperature = _mpc2520_init_structure.calib_param.c0 * 0.5 + _mpc2520_init_structure.calib_param.c1 * fTsc;
	fPsc = _mpc2520_init_structure.pressure_raw / (double)_mpc2520_init_structure.kP;
	qua2 = _mpc2520_init_structure.calib_param.c10 + fPsc * (_mpc2520_init_structure.calib_param.c20 + fPsc * _mpc2520_init_structure.calib_param.c30);
	qua3 = fTsc * fPsc * (_mpc2520_init_structure.calib_param.c11 + fPsc * _mpc2520_init_structure.calib_param.c21);
	_fPCompensate = _mpc2520_init_structure.calib_param.c00 + fPsc * qua2 + fTsc * _mpc2520_init_structure.calib_param.c01 + qua3;
	_baro_unit_data.Pressure = _fPCompensate / 100.0f;    /* convert to millibar */

	/* altitude calculations based on http://www.kansasflyer.org/index.asp?nav=Avi&sec=Alti&tab=Theory&pg=1 */

	/* current pressure at MSL in kPa */
	double p1 = _msl_pressure / 1000.0;
	/* measured pressure in kPa */
	double p = _fPCompensate / 1000.0;
	/*
	 * Solve:
	 *
	 *     /        -(aR / g)     \
	 *    | (p / p1)          . T1 | - T1
	 *     \                      /
	 * h = -------------------------------  + h1
	 *                   a
	 */
	_baro_unit_data.Attitude = (((pow((p / p1), (-(a * R) / g))) * T1) - T1) / a;
}

static HAL_StatusTypeDef BARO_WriteReg(uint8_t reg, uint8_t val)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	BARO_SPI_CS_ENABLE();
	_tx_rx_comp = 0;
	_sensor_raw_rx[0] = reg;
	_sensor_raw_rx[1] = val;
	ret = BARO_TxRxData((uint8_t *)_sensor_raw_rx, (uint8_t *)_sensor_raw_rx, 2);
	if(ret == HAL_OK) {
		while (_tx_rx_comp == 0);
	}
	return ret;
}

static HAL_StatusTypeDef BARO_ReadReg(uint8_t reg, uint8_t num, uint8_t *r)
{
	BARO_SPI_CS_ENABLE();
	_tx_rx_comp = 0;
	r[0] = reg | 0x80;
	return BARO_TxRxData((uint8_t *)r, (uint8_t *)r, num + 1); //ignore the first byte.
}

static HAL_StatusTypeDef BARO_ReadRegUtil(uint8_t reg, uint8_t num, uint8_t *r)
{
	HAL_StatusTypeDef ret = HAL_ERROR;
	BARO_SPI_CS_ENABLE();
	_tx_rx_comp = 0;
	r[0] = reg | 0x80;
	ret = BARO_TxRxData((uint8_t *)r, (uint8_t *)r, num + 1); //ignore the first byte.
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
