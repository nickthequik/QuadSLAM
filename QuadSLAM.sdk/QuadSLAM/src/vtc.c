
#include "vtc.h"

XVtc VTC_gen, VTC_det;

void VTC_init(void)
{
	int status;
	XVtc_Config *VTC_config;
	XVtc_Signal VGA_signal;
	XVtc_Timing VGA_timing;
	XVtc_HoriOffsets VGA_offsets;
	XVtc_Polarity VGA_polarity;

	// Video Out Generator VTC
	VTC_config= XVtc_LookupConfig(XPAR_V_TC_0_DEVICE_ID);
	status = XVtc_CfgInitialize(&VTC_gen, VTC_config, VTC_config->BaseAddress);

	if (status != XST_SUCCESS)
		while(1);

	XVtc_ConvVideoMode2Timing(&VTC_gen, XVTC_VMODE_XGA, &VGA_timing);
	XVtc_ConvTiming2Signal(&VTC_gen, &VGA_timing, &VGA_signal, &VGA_offsets, &VGA_polarity);

	XVtc_SetGenerator(&VTC_gen, &VGA_signal);
	XVtc_SetPolarity(&VTC_gen, &VGA_polarity);
	XVtc_SetGeneratorHoriOffset(&VTC_gen, &VGA_offsets);
	XVtc_EnableGenerator(&VTC_gen);

	// Video In Detector VTC
	VTC_config= XVtc_LookupConfig(XPAR_V_TC_1_DEVICE_ID);
	status = XVtc_CfgInitialize(&VTC_det, VTC_config, VTC_config->BaseAddress);

	XVtc_EnableDetector(&VTC_det);
}
int VTC_detector_locked(void)
{
	return (XVtc_GetDetectionStatus(&VTC_det) | XVTC_STAT_LOCKED_MASK);
}


