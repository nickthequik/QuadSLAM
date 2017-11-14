
#include "led.h"

uint32_t led_state = 0;
extern XGpio xGpio0, xGpio1;

void LED_set(uint32_t led, uint32_t led_state)
{
	uint32_t mask = 1 << led;

	if (led_state == LED_ON)
		XGpio_DiscreteSet(&xGpio0, GPIO_CHANNEL_2, mask); // this kills pwm output when mask is 1
	else
		XGpio_DiscreteClear(&xGpio0, GPIO_CHANNEL_2, mask);
}

void clk_wiz_locked(void)
{
	uint32_t locked;

	locked = XGpio_DiscreteRead(&xGpio1, GPIO_CHANNEL_1);

	if (locked)
		LED_set(3, LED_ON);
	else
		LED_set(3, LED_OFF);
}

void video_out_locked(void)
{
	uint32_t locked;

	locked = XGpio_DiscreteRead(&xGpio1, GPIO_CHANNEL_2);

	if (locked)
		LED_set(1, LED_ON);
	else
		LED_set(1, LED_OFF);
}


