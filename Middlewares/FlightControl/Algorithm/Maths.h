#ifndef __MATHS_H
#define __MATHS_H

#include "stm32f7xx.h"

#include <math.h>
#include "SystemTypes.h"

/* Exported macros -----------------------------------------------------------*/
#define ABS(x)                                   ((x) >= 0 ? (x) : -(x))
#define SQUARE(x)                                ((x) * (x))

#define LIMIT_MAX(x, max)                        (((x) > (max)) ? (max) : (x))
#define LIMIT_MIN(x, min)                        (((x) < (min)) ? (min) : (x))
#define LIMIT(x, min, max)                       (LIMIT_MAX(LIMIT_MIN(x, min), max))

#define DEG2RAD                                  (0.017453292519943295769236907684886f)
#define RAD2DEG                                  (57.295779513082320876798154814105f)

void FusionQuaternion6Axis(IMU_UnitDataDef *pIMU, Quaternion *Q, uint8_t millis, float kp, float ki);
void ConvertQuaternion2Euler(Quaternion* Q, EulerRPY* eur);

#endif /* __MATHS_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
