#ifndef __MATHS_H
#define __MATHS_H

#include "stm32f7xx.h"

#include <math.h>

/* Exported macros -----------------------------------------------------------*/
#define ABS(x)                                   ((x) >= 0 ? (x) : -(x))
#define SQUARE(x)                                ((x) * (x))

#define LIMIT_MAX(x, max)                        (((x) > (max)) ? (max) : (x))
#define LIMIT_MIN(x, min)                        (((x) < (min)) ? (min) : (x))
#define LIMIT(x, min, max)                       (LIMIT_MAX(LIMIT_MIN(x, min), max))

#endif /* __MATHS_H */

/* ------------------------ (C) COPYRIGHT kyChu ----------- END OF FILE ----- */
