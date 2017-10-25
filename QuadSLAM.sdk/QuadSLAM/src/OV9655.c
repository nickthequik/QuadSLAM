/*
 * OV9655.c
 *
 *  Created on: Oct 24, 2017
 *      Author: Ryan
 */

#include "OV9655.h"
#include "xiicps.h"
#include "FreeRTOS.h"

/*
 * Call this function once to initialize a i2c_instance
 * */
void OV9655_i2cPs_init(OV9655_camera* camera) {
	int status;

	XIicPs_Config* i2c_config;

	i2c_config = XIicPs_LookupConfig( camera->i2c_device_id );

	status = XIicPs_CfgInitialize(&camera->i2c_instance, i2c_config, camera->i2c_device_base_address );
	if(status != XST_SUCCESS)
		while(1);

	status = XIicPs_SelfTest(&camera->i2c_instance);
	if (status != XST_SUCCESS)
		while(1);

	status = XIicPs_SetSClk(&camera->i2c_instance, IIC_SCLK_RATE);
	if (status != XST_SUCCESS)
		while(1);
}

/**
 *Camera configuration.
 */
void OV9655_init(OV9655_camera* camera) {
	OV9655_i2cPs_init(camera);
	uint32_t status;

	//resets the camera, all registers set to defaults
	status = OV9655_write_register(camera, OV9655_COM7, 0b10000000);
	while(status != XST_SUCCESS);
	vTaskDelay(pdMS_TO_TICKS(100));

	//configures camera to use external clock with no internal pre-scalers
	status = OV9655_write_register(camera, OV9655_CLKRC, 0b01000000);
	while(status != XST_SUCCESS);

	//Sets output to be full resolution 15fps. Sets output format to be RGB instead of YUV
	status = OV9655_write_register(camera, OV9655_COM7, 0b00000010);
	while(status != XST_SUCCESS);

	//sets RGB 565
	status = OV9655_write_register(camera, OV9655_COM15, 0b11010000);
	while(status != XST_SUCCESS);
}

uint32_t OV9655_write_register(OV9655_camera* camera, uint8_t register_address, uint8_t register_value) {
	uint8_t value[2] = {register_address, register_value};
	uint32_t status;

	status = XIicPs_MasterSendPolled(&camera->i2c_instance, value, 1, camera->i2c_slave_address);

	if (status != XST_SUCCESS)
		return XST_FAILURE;

	while (XIicPs_BusIsBusy(&camera->i2c_instance));

	return XST_SUCCESS;
}
