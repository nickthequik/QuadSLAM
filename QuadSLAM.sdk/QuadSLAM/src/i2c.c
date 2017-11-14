/*
 * i2c.c
 *
 *  Created on: Nov 10, 2017
 *      Author: Ryan
 */
#include "i2c.h"

void I2C_write_byte(XIicPs* i2c_instance, uint8_t slave_address, uint8_t register_address, uint8_t register_value)
{
	uint8_t value[2] = {register_address, register_value};
	uint32_t status;
	uint32_t i;

	status = XIicPs_MasterSendPolled(i2c_instance, value, 2, slave_address);

	while (status != XST_SUCCESS);
	while (XIicPs_BusIsBusy(i2c_instance));

}

uint8_t I2C_read_byte(XIicPs* i2c_instance, uint8_t slave_address, uint8_t register_address)
{
	uint8_t r_address[1] = {register_address};
	uint8_t rval[1];
	uint32_t status;

	status = XIicPs_MasterSendPolled(i2c_instance, r_address, 1, slave_address);
	while(status != XST_SUCCESS);
	while (XIicPs_BusIsBusy(i2c_instance));

	status = XIicPs_MasterRecvPolled(i2c_instance, rval, 1, slave_address);
	while(status != XST_SUCCESS);
	while (XIicPs_BusIsBusy(i2c_instance));

	return rval[0];
}

void I2C_read_bytes(XIicPs* i2c_instance, uint8_t slave_address, uint8_t register_address, uint8_t num_bytes, uint8_t* buffer)
{
	uint8_t r_address[1] = {register_address};
	uint32_t status;

	status = XIicPs_MasterSendPolled(i2c_instance, r_address, 1, slave_address);
	while(status != XST_SUCCESS);
	while (XIicPs_BusIsBusy(i2c_instance));

	status = XIicPs_MasterRecvPolled(i2c_instance, buffer, num_bytes, slave_address);
	while(status != XST_SUCCESS);
	while (XIicPs_BusIsBusy(i2c_instance));
}
