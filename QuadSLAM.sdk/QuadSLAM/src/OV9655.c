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
	status = OV9655_write_register(camera, OV9655_COM7, 0b10000000);
	while(status != XST_SUCCESS);
	vTaskDelay(pdMS_TO_TICKS(100));

	//configures camera to use external clock with no internal pre-scalers
	status = OV9655_write_register(camera, OV9655_CLKRC, 0b01000000);
	while(status != XST_SUCCESS);

	//Sets output to be full resolution 15fps. Sets output format to be RGB instead of YUV
	status = OV9655_write_register(camera, OV9655_COM7, 0b00000111);
	while(status != XST_SUCCESS);

	//sets RGB 565
	status = OV9655_write_register(camera, OV9655_COM15, 0b11010000);
	while(status != XST_SUCCESS);

	//enable color bar
	status = OV9655_write_register(camera, OV9655_COM3, 0b10000000);
	while(status != XST_SUCCESS);

	//change AGC max value to be x8
	//status = OV9655_write_register(camera, OV9655_COM9, 0b00101010);
	//while(status != XST_SUCCESS);

	//increase blue gain
	status = OV9655_write_register(camera, OV9655_BLUE, 0b1111111);
	while(status != XST_SUCCESS);

	//decrease green gain
	status = OV9655_write_register(camera, OV9655_GREEN, 0b00100000);
	while(status != XST_SUCCESS);

	//decrease red gain
	status = OV9655_write_register(camera, OV9655_RED, 0b00100000);
	while(status != XST_SUCCESS);

	//increase gain
	status = OV9655_write_register(camera, OV9655_GAIN, 0b11110000);
	while(status != XST_SUCCESS);
	status = OV9655_write_register(camera, OV9655_VREF, 0b11010010);
	while(status != XST_SUCCESS);

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

