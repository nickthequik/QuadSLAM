
#include "vprocsub.h"
#include "xvprocss.h"
#include "xvprocss_log.h"
#include "xvidc.h"

typedef struct {
  u16 width_in;
  u16 height_in;
  XVidC_ColorFormat Cformat_in;
  u16 IsInterlaced;
  u16 width_out;
  u16 height_out;
  XVidC_ColorFormat Cformat_out;
} vpssVidio;

typedef enum
{
  XSYS_VPSS_STREAM_IN = 0,
  XSYS_VPSS_STREAM_OUT
}XSys_StreamDirection;

void XSys_SetStreamParam(XVprocSs *pVprocss, u16 Direction, u16 Width, u16 Height,
		                 XVidC_ColorFormat cfmt, u16 IsInterlaced);

XVprocSs vprocss;
vpssVidio thisCase;

/*
 * 1. XVprocSs_LookupConfig
 * 2. XVprocSs_CfgInitialize
 * 3. XSys_SetStreamParam for input
 * 4. XSys_SetStreamParam for output
 * 5. XVprocSs_SetSubsystemConfig
 * 6. XVprocSs_ReportSubsystemConfig
 */

void VPSS_init(void)
{
	int status;
	XVprocSs_Config* vprocss_config;
	
	vprocss_config = XVprocSs_LookupConfig(XPAR_V_PROC_SS_0_DEVICE_ID);

	if(vprocss_config == NULL)
		while(1);

	/* Start capturing event log. */
	XVprocSs_LogReset(&vprocss);

	memset(&vprocss, 0, sizeof(XVprocSs));
	
	status = XVprocSs_CfgInitialize(&vprocss, vprocss_config, vprocss_config->BaseAddress);

	if(status != XST_SUCCESS)
		while(1);

	thisCase.width_in = 1280;
	thisCase.height_in = 1024;
	thisCase.Cformat_in = XVIDC_CSF_RGB;
	thisCase.IsInterlaced = FALSE;
	thisCase.width_out = 1024;
	thisCase.height_out = 768;
	thisCase.Cformat_out = XVIDC_CSF_RGB;
	
	//Stream In
	XSys_SetStreamParam(&vprocss, XSYS_VPSS_STREAM_IN, thisCase.width_in, thisCase.height_in, 
						thisCase.Cformat_in, thisCase.IsInterlaced);

    //Stream Out
    XSys_SetStreamParam(&vprocss, XSYS_VPSS_STREAM_OUT, thisCase.width_out, thisCase.height_out, 
    					thisCase.Cformat_out, thisCase.IsInterlaced);
    
    status = XVprocSs_SetSubsystemConfig(&vprocss);

	//Query video processing subsystem configuration
	XVprocSs_ReportSubsystemConfig(&vprocss);
	XVprocSs_ReportSubcoreStatus(&vprocss, XVPROCSS_SUBCORE_SCALER_V);
	XVprocSs_ReportSubcoreStatus(&vprocss, XVPROCSS_SUBCORE_SCALER_H);
	
	XVprocSs_LogDisplay(&vprocss);
}

void XSys_SetStreamParam(XVprocSs *pVprocss, u16 Direction, u16 Width, u16 Height,
		                 XVidC_ColorFormat cfmt, u16 IsInterlaced)
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
	Stream.ColorDepth     = pVprocss->Config.ColorDepth;  // Check that this gets set
	Stream.PixPerClk      = pVprocss->Config.PixPerClock; // Check that this gets set
	Stream.FrameRate      = XVIDC_FR_60HZ;
	Stream.IsInterlaced   = IsInterlaced;

	if(Direction == XSYS_VPSS_STREAM_IN)
	{
		XVprocSs_SetVidStreamIn(pVprocss, &Stream);
	}
	else
	{
		XVprocSs_SetVidStreamOut(pVprocss, &Stream);
	}
}
