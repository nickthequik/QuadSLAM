/*
 * vdma.c
 *
 *  Created on: Oct 26, 2017
 *      Author: Ryan
 */

#include "vdma.h"
#include "xaxivdma.h"

XAxiVdma VDMA;

uint16_t *frame_buffer_1;
uint16_t *frame_buffer_2;
uint16_t *frame_buffer_3;

void VDMA_init(void)
{
	int status;
	XAxiVdma_Config *VDMA_config;
	XAxiVdma_DmaSetup Read_config;

	uint32_t i;
	uint16_t color_1 = 0xF800;
	uint16_t color_2 = 0x001F;
	uint16_t color_3 = 0x07E0;
	/*uint16_t color_1 = 0xDEAD;
	uint16_t color_2 = 0xBEEF;
	uint16_t color_3 = 0xFFFF;*/

	frame_buffer_1 = pvPortMalloc(FRAME_WIDTH * FRAME_HEIGHT * 2); // number of bytes per frame
	frame_buffer_2 = pvPortMalloc(FRAME_WIDTH * FRAME_HEIGHT * 2);
	frame_buffer_3 = pvPortMalloc(FRAME_WIDTH * FRAME_HEIGHT * 2);

	for (i = 0; i < (FRAME_WIDTH * FRAME_HEIGHT)/2; i++)
	{
		// move over 2 bytes each time
		*(frame_buffer_1 + 2*i) = color_1;
		*(frame_buffer_2 + 2*i) = color_2;
		*(frame_buffer_3 + 2*i) = color_3;
	}

	// flush cache
	Xil_L1DCacheFlush();
	Xil_L2CacheFlush();

	VDMA_config = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
	status = XAxiVdma_CfgInitialize(&VDMA, VDMA_config, VDMA_config->BaseAddress);

	if (status != XST_SUCCESS)
		while(1);

	status = XAxiVdma_SetFrmStore(&VDMA, 1, XAXIVDMA_READ);

	Read_config.VertSizeInput = FRAME_HEIGHT;
	Read_config.HoriSizeInput = FRAME_WIDTH * 2;
	Read_config.Stride = FRAME_WIDTH * 2;
	Read_config.FrameDelay = 0;
	Read_config.EnableCircularBuf = 1;
	Read_config.EnableSync = 0;
	Read_config.PointNum = 0;
	Read_config.EnableFrameCounter = 0;
	Read_config.FrameStoreStartAddr[0] = (UINTPTR) frame_buffer_1;
	Read_config.FrameStoreStartAddr[1] = (UINTPTR) frame_buffer_2;
	Read_config.FrameStoreStartAddr[2] = (UINTPTR) frame_buffer_3;
	Read_config.FixedFrameStoreAddr = 2;
	Read_config.GenLockRepeat = 0;

	status = XAxiVdma_StartReadFrame(&VDMA, &Read_config);

	if (status != XST_SUCCESS)
		while(1);
}


