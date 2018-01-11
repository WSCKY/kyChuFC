#ifndef __BOARDDEFINE_H
#define __BOARDDEFINE_H

#include "MCU_Periph.h"

/* Debug Port Linker */
#define DEBUG_PORT_SEND                          Uart7SendBuffer_DMA

/* Wifi Port Linker */
#define WIFI_PORT_GET_BYTE                       Uart8PullOneByte

/* IMU ICM20602 Linker */
#define IMU_SPI                                  SPI2
#define IMU_TxRxData                             SPI2_TxRxData_DMA
#define IMU_TxRxCpltCallback                     MCU_SPI2_TxRxCpltCallback
#define IMU_TxRxErrorCallback                    MCU_SPI2_ErrorCallback

/* BARO MCP2520 Linker */
#define BARO_SPI                                 SPI4
#define BARO_TxRxData                            SPI4_TxRxData_DMA
#define BARO_TxRxCpltCallback                    MCU_SPI4_TxRxCpltCallback

/* MOTOR Driver Linker */
#define MOTOR_FORWARD_SPEED(a,b,c,d)             do {TIM2_PWM_SetPulse(a, 0, b, 0); TIM5_PWM_SetPulse(c, 0, d, 0);} while(0)
#define MOTOR_NEGATER_SPEED(a,b,c,d)             do {TIM2_PWM_SetPulse(0, a, 0, b); TIM5_PWM_SetPulse(0, c, 0, d);} while(0)
#define MOTOR_BEEP_INIT()                        do {TIM2->CR1 &= 0xFFFE; TIM5->CR1 &= 0xFFFE; \
                                                     TIM2_PWM_SetPolarity(TIM_POLARITY_CHANNEL_1 | TIM_POLARITY_CHANNEL_3, PolarityLow); \
	                                                 TIM5_PWM_SetPolarity(TIM_POLARITY_CHANNEL_1 | TIM_POLARITY_CHANNEL_3, PolarityLow); \
	                                                 TIM2->CCR1 = TIM2->ARR + 1; TIM2->CCR3 = TIM2->ARR + 1; \
	                                                 TIM5->CCR1 = TIM5->ARR + 1; TIM5->CCR3 = TIM5->ARR + 1; \
	                                                 TIM2->CR1 |= 0x0001; TIM5->CR1 |= 0x0001;} while(0)
#define MOTOR_BEEP_EXIT()                        do {TIM2->CR1 &= 0xFFFE; TIM5->CR1 &= 0xFFFE; \
                                                     TIM2_PERIOD_SET_DEFAULT(); TIM5_PERIOD_SET_DEFAULT(); \
                                                     TIM2_PWM_SetPolarity(TIM_POLARITY_CHANNEL_1 | TIM_POLARITY_CHANNEL_3, PolarityHigh); \
	                                                 TIM5_PWM_SetPolarity(TIM_POLARITY_CHANNEL_1 | TIM_POLARITY_CHANNEL_3, PolarityHigh); \
	                                                 TIM2->CCR1 = 0; TIM2->CCR3 = 0; TIM5->CCR1 = 0; TIM5->CCR3 = 0; \
                                                     TIM2->CR1 |= 0x0001; TIM5->CR1 |= 0x0001;} while(0)
#define MOTOR_BEEP_VOLUME(v)                     do {TIM2_PWM_SetPulse(TIM2->ARR - (v), (v), TIM2->ARR - (v), (v)); \
	                                                 TIM5_PWM_SetPulse(TIM5->ARR - (v), (v), TIM5->ARR - (v), (v));} while(0)
#define MOTOR_BEEP_FREQ(f)                       do {TIM2->ARR = (TIM2_COUNT_CLOCK_RATE / ((f) << 1)) - 1; \
	                                                 TIM5->ARR = (TIM5_COUNT_CLOCK_RATE / ((f) << 1)) - 1;} while(0)

#endif /* __BOARDDEFINE_H */
