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

uint8_t camera_registers[] = {
	OV9655_GAIN,    0x00,
	OV9655_BLUE,    0x80,
	OV9655_RED,     0x80,
	//OV9655_VREF,    0x02,
	OV9655_VREF,    0x12,
	OV9655_COM1,    0x00,
	OV9655_COM2,    0x03,  // Output drive capability = x4
	OV9655_VER,     0x57,
	OV9655_COM3,    0x00,
	OV9655_COM5,    0x01,
	OV9655_COM6,    0xc0,

	OV9655_AEC,     0x10,
	//OV9655_AEC,     0x50,

	OV9655_CLKRC,   0x40,  //they set 80 we set 40 to use external clock
	OV9655_COM7,    0x07,  //they set 63 we set 07 for 15fps
	OV9655_COM8,    0xef,
	OV9655_COM9,    0x3a,
	//OV9655_COM10,   0x18,
	OV9655_COM10,   0x00,
	OV9655_REG16,   0x24,
	//OV9655_HSTART,  0x18, // 0b0000_1100_0010 = d194
	//OV9655_HSTOP,   0x04, // 0b0000_0010_0010 = d34
	//OV9655_VSTART,  0x01, // 0b0000_0000_1010 = d10
	//OV9655_VSTOP,   0x81, // 0b0100_0000_1000 = d1032
	OV9655_HSTART,  0x1A,
	OV9655_HSTOP,   0xBA,
	OV9655_VSTART,  0x01,
	OV9655_VSTOP,   0xB1,
	OV9655_MVFP,    0x00,
	OV9655_AEW,     0x3c,
	OV9655_AEB,     0x36,
	OV9655_VPT,     0x72,
	OV9655_BBIAS,   0x08,
	OV9655_GbBIAS,  0x08,
	OV9655_PREGAIN, 0x15,
	OV9655_EXHCH,   0x00,
	OV9655_EXHCL,   0x00,
	OV9655_RBIAS,   0x08,
	//OV9655_HREF,    0x12,
	OV9655_HREF,    0xA4,
	OV9655_CHLF,    0x00,
	OV9655_AREF1,   0x3f,
	OV9655_AREF2,   0x00,
	OV9655_AREF3,   0x3a,
	OV9655_ADC2,    0x72,
	OV9655_AREF4,   0x57,
	//OV9655_TSLB,    0x8c,
	OV9655_TSLB,    0xca,
	OV9655_COM11,   0x04,
	OV9655_COM13,   0x99,
	//OV9655_COM14,   0x02,
	OV9655_COM14,   0x00,
	OV9655_EDGE,    0xc1,
	OV9655_COM15,   0xd0,
	//OV9655_COM16,   0x41, //they turn on scaling down
	OV9655_COM16,   0x40,
	OV9655_COM17,   0xc0,
	0x43, 0x0a,
	0x44, 0xf0,
	0x45, 0x46,
	0x46, 0x62,
	0x47, 0x2a,
	0x48, 0x3c,
	0x4a, 0xfc,
	0x4b, 0xfc,
	0x4c, 0x7f,
	0x4d, 0x7f,
	0x4e, 0x7f,
	OV9655_MTX1,    0x98,
	OV9655_MTX2,    0x98,
	OV9655_MTX3,    0x00,
	OV9655_MTX4,    0x28,
	OV9655_MTX5,    0x70,
	OV9655_MTX6,    0x98,
	OV9655_MTXS,    0x1a,
	OV9655_AWBOP1,  0x85,
	OV9655_AWBOP2,  0xa9,
	OV9655_AWBOP3,  0x64,
	OV9655_AWBOP4,  0x84,
	OV9655_AWBOP5,  0x53,
	OV9655_AWBOP6,  0x0e,
	OV9655_BLMT,    0xf0,
	OV9655_RLMT,    0xf0,
	OV9655_GLMT,    0xf0,
	OV9655_LCC1,    0x00,
	OV9655_LCC2,    0x00,
	OV9655_LCC3,    0x02,
	OV9655_LCC4,    0x20,
	OV9655_LCC5,    0x00,
	0x69, 0x0a,
	OV9655_DBLV,    0x0a,
	0x6c, 0x04,
	0x6d, 0x55,
	0x6e, 0x00,
	0x6f, 0x9d,
	OV9655_DNSTH,   0x21,
	0x71, 0x78,
	//OV9655_POIDX,   0x11,
	OV9655_POIDX,   0x00,
	OV9655_PCKDV,   0x01,
	OV9655_XINDX,   0x10, //THESE ARE SCALING DOWN COEFFICIENTS
	OV9655_YINDX,   0x10,
	//OV9655_XINDX,   0x3A, //THESE ARE SCALING DOWN COEFFICIENTS
	//OV9655_YINDX,   0x35,
	0x76, 0x01,
	0x77, 0x02,
	OV9655_SLOP,    0x12,
	OV9655_GAM1,    0x08,
	OV9655_GAM2,    0x16,
	OV9655_GAM3,    0x30,
	OV9655_GAM4,    0x5e,
	OV9655_GAM5,    0x72,
	OV9655_GAM6,    0x82,
	OV9655_GAM7,    0x8e,
	OV9655_GAM8,    0x9a,
	OV9655_GAM9,    0xa4,
	OV9655_GAM10,   0xac,
	OV9655_GAM11,   0xb8,
	OV9655_GAM12,   0xc3,
	OV9655_GAM13,   0xd6,
	OV9655_GAM14,   0xe6,
	OV9655_GAM15,   0xf2,
	0x8a, 0x24,
	OV9655_COM19,   0x80,
	0x90, 0x7d,
	0x91, 0x7b,
	OV9655_LCC6,    0x02,
	OV9655_LCC7,    0x02,
	0x9f, 0x7a,
	0xa0, 0x79,
	OV9655_AECH,    0x1f,
	OV9655_COM21,   0x50,
	0xa5, 0x68,
	OV9655_GREEN,   0x4a,
	OV9655_REFA8,   0xc1,
	OV9655_REFA9,   0xef,
	0xaa, 0x92,
	0xab, 0x04,
	OV9655_BLC1,    0x80,
	OV9655_BLC2,    0x80,
	OV9655_BLC3,    0x80,
	OV9655_BLC4,    0x80,
	OV9655_BLC7,    0xf2,
	OV9655_BLC8,    0x20,
	OV9655_CTRLB4,  0x20,
	0xb5, 0x00,
	0xb6, 0xaf,
	0xb6, 0xaf,
	0xbb, 0xae,
	OV9655_ADBOFF,  0x7f,
	OV9655_ADROFF,  0x7f,
	OV9655_ADGbOFF, 0x7f,
	OV9655_ADGrOFF, 0x7f,
	OV9655_ADGrOFF, 0x7f,
	0xc0, 0xaa,
	0xc1, 0xc0,
	0xc2, 0x01,
	0xc3, 0x4e,
	0xc6, 0x05,
	//OV9655_COM24,   0x81,
	OV9655_COM24,   0x80,
	0xc9, 0xe0,
	0xca, 0xe8,
	0xcb, 0xf0,
	0xcc, 0xd8,
	0xcd, 0x93
};
/*
uint8_t camera_registers[] = {
		OV9655_CLKRC,   0x40,
		OV9655_COM7,    0x02,
		OV9655_COM15,   0xD0,
		/*
		OV9655_COM2,    0x03,
		OV9655_VER,     0x57,
		OV9655_COM3,    0x80,
		OV9655_COM5,    0x01,
		OV9655_COM6,    0xc0,
		OV9655_AEC,     0x50,
		*/
/*
		OV9655_AEW,     0x3c,
		OV9655_AEB,     0x36,
		OV9655_VPT,     0x72,
		OV9655_BBIAS,   0x08,
		OV9655_GbBIAS,  0x08,
		OV9655_PREGAIN, 0x15,
		OV9655_EXHCH,   0x00,
		OV9655_EXHCL,   0x00,
		OV9655_RBIAS,   0x08,
		OV9655_CHLF,    0x00,
		OV9655_AREF1,   0x3f,
		OV9655_AREF2,   0x00,
		OV9655_AREF3,   0x3a,
		OV9655_ADC2,    0x72,
		OV9655_AREF4,   0x57,
		OV9655_TSLB,    0xca,
		OV9655_COM11,   0x04,
		OV9655_COM13,   0x99,
		OV9655_COM14,   0x00,
		OV9655_EDGE,    0xc1,

		OV9655_MTX1,    0x98,
		OV9655_MTX2,    0x98,
		OV9655_MTX3,    0x00,
		OV9655_MTX4,    0x28,
		OV9655_MTX5,    0x70,
		OV9655_MTX6,    0x98,
		OV9655_MTXS,    0x1a,
		OV9655_SLOP,    0x12,
		OV9655_GAM1,    0x08,
		OV9655_GAM2,    0x16,
		OV9655_GAM3,    0x30,
		OV9655_GAM4,    0x5e,
		OV9655_GAM5,    0x72,
		OV9655_GAM6,    0x82,
		OV9655_GAM7,    0x8e,
		OV9655_GAM8,    0x9a,
		OV9655_GAM9,    0xa4,
		OV9655_GAM10,   0xac,
		OV9655_GAM11,   0xb8,
		OV9655_GAM12,   0xc3,
		OV9655_GAM13,   0xd6,
		OV9655_GAM14,   0xe6,
		OV9655_GAM15,   0xf2,

		0x8a, 0x24,
		OV9655_COM19,   0x80,
		0x90, 0x7d,
		0x91, 0x7b,
		OV9655_LCC6,    0x02,
		OV9655_LCC7,    0x02,
		0x9f, 0x7a,
		0xa0, 0x79,
		OV9655_AECH,    0x1f,
		OV9655_COM21,   0x50,
		0xa5, 0x68,
		OV9655_GREEN,   0x4a,
		OV9655_REFA8,   0xc1,
		OV9655_REFA9,   0xef,
		0xaa, 0x92,
		0xab, 0x04,
		OV9655_BLC1,    0x80,
		OV9655_BLC2,    0x80,
		OV9655_BLC3,    0x80,
		OV9655_BLC4,    0x80,
		OV9655_BLC7,    0xf2,
		OV9655_BLC8,    0x20,
		OV9655_CTRLB4,  0x20,
		0xb5, 0x00,
		0xb6, 0xaf,
		0xb6, 0xaf,
		0xbb, 0xae,
		OV9655_ADBOFF,  0x7f,
		OV9655_ADROFF,  0x7f,
		OV9655_ADGbOFF, 0x7f,
		OV9655_ADGrOFF, 0x7f,
		OV9655_ADGrOFF, 0x7f,
		0xc0, 0xaa,
		0xc1, 0xc0,
		0xc2, 0x01,
		0xc3, 0x4e,
		0xc6, 0x05,
		0xc9, 0xe0,
		0xca, 0xe8,
		0xcb, 0xf0,
		0xcc, 0xd8,
		0xcd, 0x93
};*/

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

	for(i = 0; i < 136; i+=2) {
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

