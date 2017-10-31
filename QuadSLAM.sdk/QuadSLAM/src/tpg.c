/*
 * tpg.c
 *
 *  Created on: Oct 27, 2017
 *      Author: NicholasKimball
 */

//Xilinx includes
#include "xv_tpg.h"
#include "xvidc.h"

//Custom includes
#include "tpg.h"

XV_tpg tpg;

void TPG_init(void)
{
	XV_tpg_Config* tpg_config;
	int status;

	tpg_config = XV_tpg_LookupConfig(XPAR_V_TPG_0_DEVICE_ID);
	status = XV_tpg_CfgInitialize(&tpg, tpg_config, XPAR_V_TPG_0_S_AXI_CTRL_BASEADDR);

	XV_tpg_Set_height(&tpg, 1024);
	XV_tpg_Set_width(&tpg, 1280);
	//XV_tpg_Set_height(&tpg, 768);
	//XV_tpg_Set_width(&tpg, 1024);
	XV_tpg_Set_colorFormat(&tpg, XVIDC_CSF_RGB);
	//XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_CHECKER_BOARD);
	//XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_RAINBOW_COLOR);
	//XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_TARTAN_COLOR_BARS);
	XV_tpg_Set_bckgndId(&tpg, XTPG_BKGND_CROSS_HATCH);

	XV_tpg_EnableAutoRestart(&tpg);


	XV_tpg_Start(&tpg);

}

