
#ifndef LED_H_
#define LED_H_

#include "FreeRTOS.h"
#include "xgpio.h"

#define NUM_LEDS		  ( 1 )
#define DIRECTION_OUTPUT  ( 1 )
#define OUTPUT_ENABLED	  ( 1 )
#define LED_OUTPUT		  ( 1 )
#define LED_INPUT		  ( 2 )
#define GPIO_OUTPUTS      ( 0 )
#define GPIO_INPUTS       ( 0xF )

void LED_init(void);
void set_LED(UBaseType_t uxLED, BaseType_t xValue);
void vParTestToggleLED(UBaseType_t uxLED);


#endif /* SRC_LED_H_ */
