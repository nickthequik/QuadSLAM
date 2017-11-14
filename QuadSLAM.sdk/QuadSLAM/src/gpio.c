
#include "gpio.h"

XGpio xGpio0, xGpio1, xGpio2, xGpio3, xGpio4, xGpio5;

void GPIO_init(void)
{
	XGpio_Config *pxConfigPtr;
	int status;

	// Channel 1 = LEDs, Channel 2 = clk_wiz reset
	pxConfigPtr = XGpio_LookupConfig(XPAR_AXI_GPIO_0_DEVICE_ID);
	status = XGpio_CfgInitialize( &xGpio0, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( status == XST_SUCCESS );

	// Channel 1 = clk_wiz locked, Channel 2 = video_out locked
	pxConfigPtr = XGpio_LookupConfig(XPAR_AXI_GPIO_1_DEVICE_ID);
	status = XGpio_CfgInitialize( &xGpio1, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( status == XST_SUCCESS );

	// Channel 1 = video_out status, Channel 2 = video_in enable
	pxConfigPtr = XGpio_LookupConfig(XPAR_AXI_GPIO_2_DEVICE_ID);
	status = XGpio_CfgInitialize( &xGpio2, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( status == XST_SUCCESS );

	// Channel 1 = pwm_manager_0 pwms, Channel 2 = pwm_manager_0 reset
	pxConfigPtr = XGpio_LookupConfig(XPAR_AXI_GPIO_3_DEVICE_ID);
	status = XGpio_CfgInitialize( &xGpio3, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( status == XST_SUCCESS );

	// Channel 1 = pwm_manager_1 pwms, Channel 2 = pwm_manager_1 reset
	pxConfigPtr = XGpio_LookupConfig(XPAR_AXI_GPIO_4_DEVICE_ID);
	status = XGpio_CfgInitialize( &xGpio4, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( status == XST_SUCCESS );

	// Channel 1 = pwm_generator control 1, Channel 2 = pwm_generator control 2
	pxConfigPtr = XGpio_LookupConfig(XPAR_AXI_GPIO_5_DEVICE_ID);
	status = XGpio_CfgInitialize( &xGpio5, pxConfigPtr, pxConfigPtr->BaseAddress );
	configASSERT( status == XST_SUCCESS );

	// Init LEDs off, clk_wiz not reset
	XGpio_SetDataDirection(&xGpio0, GPIO_CHANNEL_1, GPIO_OUTPUTS);
	XGpio_SetDataDirection(&xGpio0, GPIO_CHANNEL_2, GPIO_OUTPUTS);
	XGpio_DiscreteWrite(&xGpio0, GPIO_CHANNEL_1, 0);
	XGpio_DiscreteWrite(&xGpio0, GPIO_CHANNEL_2, 0);

	// Init as clk_wiz locked and video_out locked as inputs
	XGpio_SetDataDirection(&xGpio1, GPIO_CHANNEL_1, GPIO_INPUTS);
	XGpio_SetDataDirection(&xGpio1, GPIO_CHANNEL_2, GPIO_INPUTS);

	// Init video_out status as input and video_in enable as output
	XGpio_SetDataDirection(&xGpio2, GPIO_CHANNEL_1, GPIO_INPUTS);
	XGpio_SetDataDirection(&xGpio2, GPIO_CHANNEL_2, GPIO_OUTPUTS);
	XGpio_DiscreteWrite(&xGpio2, GPIO_CHANNEL_2, 0);

	// Init pwm_detector pwm_read_1 & 2 inputs
	XGpio_SetDataDirection(&xGpio3, GPIO_CHANNEL_1, GPIO_INPUTS);
	XGpio_SetDataDirection(&xGpio3, GPIO_CHANNEL_2, GPIO_INPUTS);

	// Init pwm_detector pwm_read_3 input and pwm_reset to 1
	XGpio_SetDataDirection(&xGpio4, GPIO_CHANNEL_1, GPIO_INPUTS);
	XGpio_SetDataDirection(&xGpio4, GPIO_CHANNEL_2, GPIO_OUTPUTS);
	XGpio_DiscreteWrite(&xGpio4, GPIO_CHANNEL_2, 1);

	// Init pwm_generator and hold it in reset
	XGpio_SetDataDirection(&xGpio5, GPIO_CHANNEL_1, GPIO_OUTPUTS);
	XGpio_SetDataDirection(&xGpio5, GPIO_CHANNEL_2, GPIO_OUTPUTS);
	XGpio_DiscreteWrite(&xGpio5, GPIO_CHANNEL_2, 0x400000);
	XGpio_DiscreteWrite(&xGpio5, GPIO_CHANNEL_2, 0x000000);

}
