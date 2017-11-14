#include "stereo_camera.h"
#include "OV9655.h"
#include "xiicps.h"

OV9655_camera left_camera;
OV9655_camera right_camera;

void STEREO_CAMERA_init() {

	left_camera.i2c_device_id = XPAR_PS7_I2C_0_DEVICE_ID;
	left_camera.i2c_device_base_address = XPAR_PS7_I2C_0_BASEADDR;
	left_camera.i2c_slave_address = OV9655_I2C_ADDRESS;

	OV9655_init(&left_camera);

}


