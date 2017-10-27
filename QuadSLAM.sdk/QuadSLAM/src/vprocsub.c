/*
 * vprocsub.c
 *
 *  Created on: Oct 26, 2017
 *      Author: Ryan
 */

#include "vprocsub.h"
#include "xvprocss.h"

XVprocSs vpss;
void XSys_SetStreamParam(XVprocSs *pVprocss, u16 Direction, u16 Width, u16 Height, XVidC_ColorFormat cfmt,u16 IsInterlaced);

void VPSS_init(void)
{
	int status;

	XVprocSs_Config* vpss_config;

	vpss_config = XVprocSs_LookupConfig( XPAR_V_PROC_SS_0_DEVICE_ID );

	status = XVprocSs_CfgInitialize(&vpss, vpss_config, XPAR_V_PROC_SS_0_BASEADDR );
	while(status != XST_SUCCESS);

	XSys_SetStreamParam(&vpss, 0, 1280, 1024, XVIDC_CSF_RGB, 0);
	XSys_SetStreamParam(&vpss, 1, 1024, 768, XVIDC_CSF_RGB, 0);

	XVprocSs_Start(&vpss);
}

/*
 * Courtesy of Xilinx GitHub examples
 * */
void XSys_SetStreamParam(XVprocSs *pVprocss,
		                 u16 Direction,
		                 u16 Width,
		                 u16 Height,
		                 XVidC_ColorFormat cfmt,
		                 u16 IsInterlaced)
{
  XVidC_VideoMode resId;
  XVidC_VideoStream Stream;
  XVidC_VideoTiming const *TimingPtr;

  resId = XVidC_GetVideoModeId(Width, Height, XVIDC_FR_60HZ, IsInterlaced);
  TimingPtr = XVidC_GetTimingInfo(resId);

  //Setup Video Processing Subsystem
  Stream.VmId           = resId;
  Stream.Timing         = *TimingPtr;
  Stream.ColorFormatId  = cfmt;
  Stream.ColorDepth     = pVprocss->Config.ColorDepth;
  Stream.PixPerClk      = pVprocss->Config.PixPerClock;
  Stream.FrameRate      = XVIDC_FR_60HZ;
  Stream.IsInterlaced   = IsInterlaced;

  if(Direction == 0)
  {
    XVprocSs_SetVidStreamIn(pVprocss, &Stream);
  }
  else
  {
    XVprocSs_SetVidStreamOut(pVprocss, &Stream);
  }
}
