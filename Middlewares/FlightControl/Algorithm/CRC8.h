#ifndef __CRC8_H
#define __CRC8_H

#include "stm32f7xx.h"

uint8_t ComputeCRC8(uint8_t *pchMessage, uint32_t dwLength, uint8_t ucCRC8);

#endif /* __CRC8_H */

