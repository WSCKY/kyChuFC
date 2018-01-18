#ifndef __BARO_MPC2520_H
#define __BARO_MPC2520_H

#include "BoardDefine.h"
#include "SystemTypes.h"

#define MPC2520_CHIP_ID                          0x10
/* ----- Register Map ----- */
#define PRESSURE_MSB_REG                         0x00 /* r */
#define PRESSURE_LSB_REG                         0x01 /* r */
#define PRESSURE_XLSB_REG                        0x02 /* r */
#define TEMPERATURE_MSB_REG                      0x03 /* r */
#define TEMPERATURE_LSB_REG                      0x04 /* r */
#define TEMPERATURE_XLSB_REG                     0x05 /* r */
#define PRESSURE_CFG_REG                         0x06 /* rw */
#define TEMPERATURE_CFG_REG                      0x07 /* rw */
#define MODE_STATUS_CFG_REG                      0x08 /* r/rw */
#define INTR_FIFO_CFG_REG                        0x09 /* rw */
#define INTR_STATUS_REG                          0x0A /* r */
#define FIFO_STATUS_REG                          0x0B /* r */
#define RESET_FLUSH_REG                          0x0C /* w */
#define PRODUCT_REV_ID_REG                       0x0D /* r */
#define CALIB_COEF_REG_START                     0x10
#define CALIB_COEF_REG_END                       0x21

typedef struct {
	int16_t c0, c1, c01, c11, c20, c21, c30;
	int32_t c00, c10;
} MPC2520_CALIB_PARAM;

typedef struct {
	MPC2520_CALIB_PARAM calib_param;
	int32_t pressure_raw;
	int32_t temperature_raw;
	int32_t kP;
	int32_t kT;
} MPC2520_INIT_STRUCT;

HAL_StatusTypeDef BARO_MPC2520_Init(void);
HAL_StatusTypeDef BARO_MPC2520_Read(void);
BARO_UnitDataDef *GetBARO_UNIT_DATA(void);

#endif /* __BARO_MPC2520_H */
