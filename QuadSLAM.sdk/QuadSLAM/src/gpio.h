
#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_

#include "xgpio.h"
#include "FreeRTOSConfig.h"

#define GPIO_CHANNEL_1 (1)
#define GPIO_CHANNEL_2 (2)
#define GPIO_OUTPUTS   (0)
#define GPIO_INPUTS    (0xFFFFFFFF)

extern XGpio xGpio0, xGpio1, xGpio2, xGpio3, xGpio4, xGpio5;

void GPIO_init(void);

#endif /* SRC_GPIO_H_ */
