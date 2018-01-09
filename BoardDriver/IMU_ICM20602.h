#ifndef __IMU_ICM20602_H
#define __IMU_ICM20602_H

#include "stm32f7xx.h"
#include "BoardDefine.h"
#include "SystemTypes.h"

/* Exported types ------------------------------------------------------------*/
typedef struct {
	int16_t AccX;
	int16_t AccY;
	int16_t AccZ;
	int16_t Temp;
	int16_t GyrX;
	int16_t GyrY;
	int16_t GyrZ;
} SensorDataDef;
/* Exported constants --------------------------------------------------------*/
/* Definition for IMU_CS clock resources */
#define IMU_SPI_CS_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()
/* Definition for IMU_CS Pins */
#define IMU_SPI_CS_PIN                           GPIO_PIN_12
#define IMU_SPI_CS_GPIO_PORT                     GPIOB

#define IMU_SPI_CS_HIGH()                        {IMU_SPI_CS_GPIO_PORT->BSRR = IMU_SPI_CS_PIN;}
#define IMU_SPI_CS_LOW()                         {IMU_SPI_CS_GPIO_PORT->BSRR = (uint32_t)IMU_SPI_CS_PIN << 16;}

HAL_StatusTypeDef IMU_ICM20602_Init(void);
HAL_StatusTypeDef IMU_ICM20602_Read(void);
IMU_RawDataDef *GetIMU_RAW_DATA(void);
IMU_UnitDataDef *GetIMU_Unit_DATA(void);

#endif /* __IMU_ICM20602_H */

