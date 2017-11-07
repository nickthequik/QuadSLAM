/*
 * vdma.c
 *
 *  Created on: Oct 26, 2017
 *      Author: Ryan
 */

#include "vdma.h"
#include "xaxivdma.h"
#include "FreeRTOS.h"
#include "xil_cache_l.h"

XAxiVdma VDMA;

uint16_t *frame_buffer_1;
uint16_t *frame_buffer_2;
uint16_t *frame_buffer_3;

void VDMA_read_init(XAxiVdma_DmaSetup *Read_config)
{
	Read_config->VertSizeInput = FRAME_HEIGHT;
	Read_config->HoriSizeInput = FRAME_WIDTH * 2;
	Read_config->Stride = FRAME_WIDTH * 2;
	Read_config->FrameDelay = 0;
	Read_config->EnableCircularBuf = 1;
	Read_config->EnableSync = 1;
	Read_config->PointNum = 0;
	Read_config->EnableFrameCounter = 0;
	Read_config->FrameStoreStartAddr[0] = (UINTPTR) frame_buffer_1;
	Read_config->FrameStoreStartAddr[1] = (UINTPTR) frame_buffer_2;
	Read_config->FrameStoreStartAddr[2] = (UINTPTR) frame_buffer_3;
	Read_config->FixedFrameStoreAddr = 0;
	Read_config->GenLockRepeat = 0;
}

void VDMA_write_init(XAxiVdma_DmaSetup *Write_config)
{
	Write_config->VertSizeInput = FRAME_HEIGHT;
	Write_config->HoriSizeInput = FRAME_WIDTH * 2;
	Write_config->Stride = FRAME_WIDTH * 2;
	Write_config->FrameDelay = 0;
	Write_config->EnableCircularBuf = 1;
	Write_config->EnableSync = 1;
	Write_config->PointNum = 0;
	Write_config->EnableFrameCounter = 0;
	Write_config->FrameStoreStartAddr[0] = (UINTPTR) frame_buffer_1;
	Write_config->FrameStoreStartAddr[1] = (UINTPTR) frame_buffer_2;
	Write_config->FrameStoreStartAddr[2] = (UINTPTR) frame_buffer_3;
	Write_config->FixedFrameStoreAddr = 0;
	Write_config->GenLockRepeat = 0;
}

void VDMA_init(void)
{
	int status;
	XAxiVdma_Config *VDMA_config;
	XAxiVdma_DmaSetup Read_config;
	XAxiVdma_DmaSetup Write_config;

	// number of bytes per frame
	frame_buffer_1 = (uint16_t*) pvPortMalloc(FRAME_WIDTH * FRAME_HEIGHT * 2);
	frame_buffer_2 = (uint16_t*) pvPortMalloc(FRAME_WIDTH * FRAME_HEIGHT * 2);
	frame_buffer_3 = (uint16_t*) pvPortMalloc(FRAME_WIDTH * FRAME_HEIGHT * 2);

	/*uint32_t i;
	uint16_t color_1 = 0xF800;
	uint16_t color_2 = 0x001F;
	uint16_t color_3 = 0x07E0;

	for (i = 0; i < (FRAME_WIDTH * FRAME_HEIGHT); i++)
	{
		*(frame_buffer_1 + i) = color_1;
		*(frame_buffer_2 + i) = color_2;
		*(frame_buffer_3 + i) = color_3;
	}

	// flush cache
	Xil_L1DCacheFlush();
	Xil_L2CacheFlush();*/

	VDMA_config = XAxiVdma_LookupConfig(XPAR_AXI_VDMA_0_DEVICE_ID);
	status = XAxiVdma_CfgInitialize(&VDMA, VDMA_config, VDMA_config->BaseAddress);

	if (status != XST_SUCCESS)
		while(1);

	VDMA_write_init(&Write_config);
	VDMA_read_init(&Read_config);

	status = XAxiVdma_StartWriteFrame(&VDMA, &Write_config);

	if (status != XST_SUCCESS)
		while(1);

	status = XAxiVdma_StartReadFrame(&VDMA, &Read_config);

	if (status != XST_SUCCESS)
		while(1);
}


