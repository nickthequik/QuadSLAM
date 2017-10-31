
#include "led.h"
#include "xgpio.h"
#include "FreeRTOS.h"

//xGpio0 is attached to the leds

XGpio xGpio0, xGpio1, xGpio2;
UBaseType_t led_state = 0;

void LED_init( void )
{
	XGpio_Config *pxConfigPtr;
	BaseType_t xStatus;

	/* Initialise the GPIO driver. */
	pxConfigPtr = XGpio_LookupConfig(XPAR_AXI_GPIO_0_DEVICE_ID);
	xStatus = XGpio_CfgInitialize( &xGpio0, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( xStatus == XST_SUCCESS );

	pxConfigPtr = XGpio_LookupConfig(XPAR_AXI_GPIO_1_DEVICE_ID);
	xStatus = XGpio_CfgInitialize( &xGpio1, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( xStatus == XST_SUCCESS );

	pxConfigPtr = XGpio_LookupConfig(XPAR_AXI_GPIO_2_DEVICE_ID);
	xStatus = XGpio_CfgInitialize( &xGpio2, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( xStatus == XST_SUCCESS );

	XGpio_SetDataDirection(&xGpio0, 1, GPIO_OUTPUTS);
	XGpio_SetDataDirection(&xGpio0, 2, GPIO_OUTPUTS);
	XGpio_DiscreteWrite (&xGpio0, 1, 0);
	XGpio_DiscreteWrite (&xGpio0, 2, 0);

	XGpio_SetDataDirection(&xGpio1, 1, GPIO_INPUTS);
	XGpio_SetDataDirection(&xGpio1, 2, GPIO_INPUTS);

	XGpio_SetDataDirection(&xGpio2, 1, GPIO_INPUTS);
	//XGpio_SetDataDirection(&xGpio2, 2, GPIO_OUTPUTS);
	//XGpio_DiscreteWrite (&xGpio2, 2, 1);
}

void LED_set(uint32_t led, uint32_t led_state)
{
	uint32_t mask = 1 << led;

	if (led_state == LED_ON)
		XGpio_DiscreteSet(&xGpio0, 1, mask);
	else
		XGpio_DiscreteClear(&xGpio0, 1, mask);
}

/*void set_LED(UBaseType_t uxLED, BaseType_t xValue)
{
	uint32_t i;
	UBaseType_t uxMask = 1, uxOnMask = 0, uxOffMask = 0;

	for (i = 0; i < 4; i++)
	{
		if (uxLED & uxMask)
		{
			if (xValue & uxMask)
				uxOnMask |= uxMask;
			else
				uxOffMask |= uxMask;
		}
		uxMask = uxMask << 1;
	}
	//XGpio_DiscreteSet(&xGpio, partstLED_OUTPUT, uxOnMask);
	//XGpio_DiscreteClear(&xGpio, partstLED_OUTPUT, uxOffMask);
}*/

