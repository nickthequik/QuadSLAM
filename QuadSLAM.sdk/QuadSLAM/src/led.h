
#ifndef LED_H_
#define LED_H_

#include "gpio.h"

#define NUM_LEDS		  ( 1 )
#define DIRECTION_OUTPUT  ( 1 )
#define OUTPUT_ENABLED	  ( 1 )
#define LED_OUTPUT		  ( 1 )
#define LED_INPUT		  ( 2 )

#define LED_ON            ( 1 )
#define LED_OFF           ( 0 )

void LED_init(void);
void LED_set(uint32_t led, uint32_t led_state);
void clk_wiz_locked(void);
void video_out_locked(void);


#endif /* SRC_LED_H_ */
