/*
 * vtc.c
 *
 *  Created on: Oct 26, 2017
 *      Author: Ryan
 */

#include "vtc.h"
#include "xvtc.h"

void VTC_init(void)
{
	int status;
	XVtc_Config *VTC_config;
	XVtc VTC;
	XVtc_Signal VGA_signal;
	XVtc_Timing VGA_timing;
	XVtc_HoriOffsets VGA_offsets;
	XVtc_Polarity VGA_polarity;

	VTC_config= XVtc_LookupConfig(XPAR_V_TC_0_DEVICE_ID);
	status = XVtc_CfgInitialize(&VTC, VTC_config, VTC_config->BaseAddress);

	if (status != XST_SUCCESS)
		while(1);

	XVtc_ConvVideoMode2Timing(&VTC, XVTC_VMODE_XGA, &VGA_timing);
	XVtc_ConvTiming2Signal(&VTC, &VGA_timing, &VGA_signal, &VGA_offsets, &VGA_polarity);

	XVtc_SetGenerator(&VTC, &VGA_signal);
	XVtc_SetPolarity(&VTC, &VGA_polarity);
	XVtc_SetGeneratorHoriOffset(&VTC, &VGA_offsets);
	XVtc_EnableGenerator(&VTC);
}


