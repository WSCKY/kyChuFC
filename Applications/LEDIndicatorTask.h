#ifndef __LEDINDICATORTASK_H
#define __LEDINDICATORTASK_H

#include "BoardDefine.h"
#include "FlightModeManage.h"
#include "SystemPowerManage.h"

#define LED_COLOR_RED                            (0x01)
#define LED_COLOR_BLUE                           (0x02)
#define LED_COLOR_GREEN                          (0x04)

#define LED_COLOR_BLACK                          (0x00)
#define LED_COLOR_PURPLE                         (0x03)
#define LED_COLOR_YELLOW                         (0x05)
#define LED_COLOR_ICEBLUE                        (0x06)
#define LED_COLOR_WHITE                          (0x07)

#define LED_FLASH_RATE_SLOW                      (1)
#define LED_FLASH_RATE_FAST                      (5)

void LEDIndicatorTask(uint8_t millis);

void LedCtrlInsertNotice(void);
void LedCtrlManuallyEnable(void);
void LedCtrlManuallyDisable(void);

#endif /* __LEDINDICATORTASK_H */
