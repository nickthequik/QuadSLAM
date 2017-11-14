/*
 * i2c.h
 *
 *  Created on: Nov 10, 2017
 *      Author: Ryan
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include <inttypes.h>

#include "xiicps.h"

void I2C_write_byte(XIicPs* i2c_instance, uint8_t slave_address, uint8_t register_address, uint8_t register_value);
uint8_t I2C_read_byte(XIicPs* i2c_instance, uint8_t slave_address, uint8_t register_address);
void I2C_read_bytes(XIicPs* i2c_instance, uint8_t slave_address, uint8_t register_address, uint8_t num_bytes, uint8_t* buffer);

#endif /* SRC_I2C_H_ */
