
#include "uart.h"
#include "xuartps.h"
#include "xparameters.h"

XUartPs UART_usb;

void UART_usb_init() {
	int status;

	XUartPs_Config* UART_usb_config;


	UART_usb_config = XUartPs_LookupConfig(XPAR_PS7_UART_1_DEVICE_ID);
	status = XUartPs_CfgInitialize(&UART_usb, UART_usb_config, XPAR_PS7_UART_1_BASEADDR);

	if(status != XST_SUCCESS)
		while(1);

	status = XUartPs_SetBaudRate(&UART_usb, 115200);

	if(status != XST_SUCCESS)
			while(1);
}

void UART_usb_write(char* message) {
	uint32_t num_bytes = 0;
	uint8_t* buff = (uint8_t*)message;

	while(*message++)
		num_bytes++;

	XUartPs_Send(&UART_usb, buff, num_bytes);
}

int putchar(int c)
{
	XUartPs_SendByte(XPAR_PS7_UART_1_BASEADDR, (uint8_t) c);
	return 0;
}
