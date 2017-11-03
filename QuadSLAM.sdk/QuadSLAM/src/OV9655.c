/*
 * OV9655.c
 *
 *  Created on: Oct 24, 2017
 *      Author: Ryan
 */

#include "OV9655.h"
#include "xiicps.h"
#include "FreeRTOS.h"
#include "task.h"

/*
 * Initializes a i2c_instance
 * */
void OV9655_i2cPs_init(OV9655_camera* camera)
{
	int status;

	XIicPs_Config* i2c_config;

	i2c_config = XIicPs_LookupConfig( camera->i2c_device_id );

	status = XIicPs_CfgInitialize(&camera->i2c_instance, i2c_config, camera->i2c_device_base_address );
	while(status != XST_SUCCESS);

	status = XIicPs_SelfTest(&camera->i2c_instance);
	while(status != XST_SUCCESS);

	status = XIicPs_SetSClk(&camera->i2c_instance, IIC_SCLK_RATE);
	while(status != XST_SUCCESS);
}

/**
 *Camera configuration.
 */
void OV9655_init(OV9655_camera* camera)
{
	OV9655_i2cPs_init(camera);
	int status;

	//resets the camera, all registers set to defaults
	//COM7 0x12. they out 63, we put 80
	status = OV9655_write_register(camera, OV9655_COM7, 0b10000000);
	while(status != XST_SUCCESS);
	vTaskDelay(pdMS_TO_TICKS(100));

	OV9655_write_registers(camera);
}

void OV9655_write_registers(OV9655_camera* camera) {
	int i, status;

	for(i = 0; i < 150; i+=2) {
		status = OV9655_write_register(camera, camera_registers[i], camera_registers[i+1]);
		vTaskDelay(pdMS_TO_TICKS(1));
		while(status != XST_SUCCESS);
	}
}

uint32_t OV9655_write_register(OV9655_camera* camera, uint8_t register_address, uint8_t register_value) {
	uint8_t value[2] = {register_address, register_value};
	uint32_t status;

	status = XIicPs_MasterSendPolled(&camera->i2c_instance, value, 2, camera->i2c_slave_address);

	if (status != XST_SUCCESS)
		return XST_FAILURE;

	while (XIicPs_BusIsBusy(&camera->i2c_instance));

	return XST_SUCCESS;
}

