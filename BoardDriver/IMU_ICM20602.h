#ifndef __IMU_ICM20602_H
#define __IMU_ICM20602_H

#include "stm32f7xx.h"
#include "BoardDefine.h"

typedef struct {
	int16_t accX;
	int16_t accY;
	int16_t accZ;
	int16_t Temp;
	int16_t gyrX;
	int16_t gyrY;
	int16_t gyrZ;
} IMU_RAW_DATA_Def;
/* Exported types ------------------------------------------------------------*/
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
IMU_RAW_DATA_Def *GetIMU_RAW_DATA(void);

#endif /* __IMU_ICM20602_H */

