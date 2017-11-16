/*
 * mpu9250.c
 *
 *  Created on: Nov 9, 2017
 *      Author: Ryan
 */
#include <stdarg.h>
#include <stdio.h>
#include <math.h>

#include "FreeRTOS.h"
#include "task.h"

#include "xiicps.h"

#include "mpu9250.h"

#include "quaternionFilters.h"
#include "i2c.h"

#include "xtime_l.h"

XIicPs i2c_instance;

enum Ascale {
  AFS_2G = 0,
  AFS_4G,
  AFS_8G,
  AFS_16G
};

enum Gscale {
  GFS_250DPS = 0,
  GFS_500DPS,
  GFS_1000DPS,
  GFS_2000DPS
};

enum Mscale {
  MFS_14BITS = 0, // 0.6 mG per LSB
  MFS_16BITS      // 0.15 mG per LSB
};

    // Specify sensor full scale
uint8_t Gscale = GFS_250DPS;
uint8_t Ascale = AFS_2G;
// Choose either 14-bit or 16-bit magnetometer resolution
uint8_t Mscale = MFS_16BITS;

uint8_t Mmode = 0x02;

float euler_angles[3];
float temperature;   // Stores the real internal chip temperature in Celsius
int16_t tempCount;   // Temperature raw count output
uint32_t delt_t = 0; // Used to control display output rate

uint32_t count = 0, sumCount = 0; // used to control display output rate
float deltat = 0.0f, sum = 0.0f;  // integration interval for both filter schemes
XTime lastUpdate = 0, firstUpdate = 0; // used to calculate integration interval
XTime Now = 0;        // used to calculate integration interval

int16_t gyroCount[3];   // Stores the 16-bit signed gyro sensor output
int16_t magCount[3];    // Stores the 16-bit signed magnetometer sensor output
// Scale resolutions per LSB for the sensors
float aRes, gRes, mRes;
// Variables to hold latest sensor data values
float ax, ay, az, gx, gy, gz, mx, my, mz;
// Factory mag calibration and mag bias
float magCalibration[3] = {0, 0, 0}, magbias[3] = {0, 0, 0};
// Bias corrections for gyro and accelerometer
float gyroBias[3] = {0, 0, 0}, accelBias[3] = {0, 0, 0};
float SelfTest[6];
// Stores the 16-bit signed accelerometer sensor output
int16_t accelCount[3];

void IMU_init(void)
{
	MPU9250_i2cPs_init();

	MPU9250SelfTest(SelfTest);
	calibrateMPU9250(gyroBias, accelBias);
	MPU9250_init();
	initAK8963(magCalibration);
}

void MPU9250_i2cPs_init()
{
	int status;

	XIicPs_Config* i2c_config;

	//i2c_config = XIicPs_LookupConfig( XPAR_PS7_I2C_1_DEVICE_ID );

	//status = XIicPs_CfgInitialize( &i2c_instance, i2c_config, XPAR_PS7_I2C_1_BASEADDR );
	while(status != XST_SUCCESS);

	//status = XIicPs_SelfTest( &i2c_instance );
	while(status != XST_SUCCESS);

	//status = XIicPs_SetSClk( &i2c_instance, MPU9250_IIC_SCLK_RATE );
	while(status != XST_SUCCESS);
}


void getMres() {
  switch (Mscale)
  {
  // Possible magnetometer scales (and their register bit settings) are:
  // 14 bit resolution (0) and 16 bit resolution (1)
    case MFS_14BITS:
          mRes = 10.*4912./8190.; // Proper scale to return milliGauss
          break;
    case MFS_16BITS:
          mRes = 10.*4912./32760.0; // Proper scale to return milliGauss
          break;
  }
}

void getGres() {
  switch (Gscale)
  {
  // Possible gyro scales (and their register bit settings) are:
  // 250 DPS (00), 500 DPS (01), 1000 DPS (10), and 2000 DPS  (11).
        // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
    case GFS_250DPS:
          gRes = 250.0/32768.0;
          break;
    case GFS_500DPS:
          gRes = 500.0/32768.0;
          break;
    case GFS_1000DPS:
          gRes = 1000.0/32768.0;
          break;
    case GFS_2000DPS:
          gRes = 2000.0/32768.0;
          break;
  }
}

void getAres() {
  switch (Ascale)
  {
  // Possible accelerometer scales (and their register bit settings) are:
  // 2 Gs (00), 4 Gs (01), 8 Gs (10), and 16 Gs  (11).
        // Here's a bit of an algorith to calculate DPS/(ADC tick) based on that 2-bit value:
    case AFS_2G:
          aRes = 2.0/32768.0;
          break;
    case AFS_4G:
          aRes = 4.0/32768.0;
          break;
    case AFS_8G:
          aRes = 8.0/32768.0;
          break;
    case AFS_16G:
          aRes = 16.0/32768.0;
          break;
  }
}


void readAccelData(int16_t * destination)
{
  uint8_t rawData[6];  // x/y/z accel register data stored here
  I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]);  // Read the six raw data registers into data array
  destination[0] = ((int16_t)rawData[0] << 8) | rawData[1] ;  // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = ((int16_t)rawData[2] << 8) | rawData[3] ;
  destination[2] = ((int16_t)rawData[4] << 8) | rawData[5] ;
}


void readGyroData(int16_t * destination)
{
  uint8_t rawData[6];  // x/y/z gyro register data stored here
  I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]);  // Read the six raw data registers sequentially into data array
  destination[0] = ((int16_t)rawData[0] << 8) | rawData[1] ;  // Turn the MSB and LSB into a signed 16-bit value
  destination[1] = ((int16_t)rawData[2] << 8) | rawData[3] ;
  destination[2] = ((int16_t)rawData[4] << 8) | rawData[5] ;
}

void readMagData(int16_t * destination)
{
  // x/y/z gyro register data, ST2 register stored here, must read ST2 at end of
  // data acquisition
  uint8_t rawData[7];
  // Wait for magnetometer data ready bit to be set
  if(I2C_read_byte(&i2c_instance, AK8963_ADDRESS, AK8963_ST1) & 0x01)
  {
    // Read the six raw data and ST2 registers sequentially into data array
    I2C_read_bytes(&i2c_instance, AK8963_ADDRESS, AK8963_XOUT_L, 7, &rawData[0]);
    uint8_t c = rawData[6]; // End data read by reading ST2 register
    // Check if magnetic sensor overflow set, if not then report data
    if(!(c & 0x08))
    {
      // Turn the MSB and LSB into a signed 16-bit value
      destination[0] = ((int16_t)rawData[1] << 8) | rawData[0];
      // Data stored as little Endian
      destination[1] = ((int16_t)rawData[3] << 8) | rawData[2];
      destination[2] = ((int16_t)rawData[5] << 8) | rawData[4];
    }
  }
}

int16_t readTempData()
{
  uint8_t rawData[2];  // x/y/z gyro register data stored here
  I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, TEMP_OUT_H, 2, &rawData[0]);  // Read the two raw data registers sequentially into data array
  return ((int16_t)rawData[0] << 8) | rawData[1];  // Turn the MSB and LSB into a 16-bit value
}

// Calculate the time the last update took for use in the quaternion filters
void updateTime()
{
  XTime_GetTime(&Now);

  // Set integration time by time elapsed since last filter update
  deltat = (Now - lastUpdate) / 325000000.0f;
  lastUpdate = Now;

  sum += deltat; // sum for averaging filter update rate
  sumCount++;
}

void initAK8963(float * destination)
{
  // First extract the factory calibration for each magnetometer axis
  uint8_t rawData[3];  // x/y/z gyro calibration data stored here
  I2C_write_byte(&i2c_instance, AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
  vTaskDelay(pdMS_TO_TICKS(10));
  I2C_write_byte(&i2c_instance, AK8963_ADDRESS, AK8963_CNTL, 0x0F); // Enter Fuse ROM access mode
  vTaskDelay(pdMS_TO_TICKS(10));
  I2C_read_bytes(&i2c_instance, AK8963_ADDRESS, AK8963_ASAX, 3, &rawData[0]);  // Read the x-, y-, and z-axis calibration values
  destination[0] =  (float)(rawData[0] - 128)/256. + 1.;   // Return x-axis sensitivity adjustment values, etc.
  destination[1] =  (float)(rawData[1] - 128)/256. + 1.;
  destination[2] =  (float)(rawData[2] - 128)/256. + 1.;
  I2C_write_byte(&i2c_instance, AK8963_ADDRESS, AK8963_CNTL, 0x00); // Power down magnetometer
  vTaskDelay(pdMS_TO_TICKS(10));
  // Configure the magnetometer for continuous read and highest resolution
  // set Mscale bit 4 to 1 (0) to enable 16 (14) bit resolution in CNTL register,
  // and enable continuous mode data acquisition Mmode (bits [3:0]), 0010 for 8 Hz and 0110 for 100 Hz sample rates
  I2C_write_byte(&i2c_instance, AK8963_ADDRESS, AK8963_CNTL, Mscale << 4 | Mmode); // Set magnetometer data resolution and sample ODR
  vTaskDelay(pdMS_TO_TICKS(10));
}

void MPU9250_init()
{

 // wake up device
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors
  vTaskDelay(pdMS_TO_TICKS(100)); // Wait for all registers to reset

 // get stable time source
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, PWR_MGMT_1, 0x01);  // Auto select clock source to be PLL gyroscope reference if ready else
  vTaskDelay(pdMS_TO_TICKS(200));

 // Configure Gyro and Thermometer
 // Disable FSYNC and set thermometer and gyro bandwidth to 41 and 42 Hz, respectively;
 // minimum delay time for this setting is 5.9 ms, which means sensor fusion update rates cannot
 // be higher than 1 / 0.0059 = 170 Hz
 // DLPF_CFG = bits 2:0 = 011; this limits the sample rate to 1000 Hz for both
 // With the MPU9250, it is possible to get gyro sample rates of 32 kHz (!), 8 kHz, or 1 kHz
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, CONFIG, 0x03);

 // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, SMPLRT_DIV, 0x04);  // Use a 200 Hz rate; a rate consistent with the filter update rate
                                    // determined inset in CONFIG above

 // Set gyroscope full scale range
 // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
  uint8_t c = I2C_read_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, GYRO_CONFIG); // get current GYRO_CONFIG register value
 // c = c & ~0xE0; // Clear self-test bits [7:5]
  c = c & ~0x02; // Clear Fchoice bits [1:0]
  c = c & ~0x18; // Clear AFS bits [4:3]
  c = c | Gscale << 3; // Set full scale range for the gyro
 // c =| 0x00; // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, GYRO_CONFIG, c ); // Write new GYRO_CONFIG value to register

 // Set accelerometer full-scale range configuration
  c = I2C_read_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG); // get current ACCEL_CONFIG register value
 // c = c & ~0xE0; // Clear self-test bits [7:5]
  c = c & ~0x18;  // Clear AFS bits [4:3]
  c = c | Ascale << 3; // Set full scale range for the accelerometer
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG, c); // Write new ACCEL_CONFIG register value

 // Set accelerometer sample rate configuration
 // It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
 // accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
  c = I2C_read_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG2); // get current ACCEL_CONFIG2 register value
  c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
  c = c | 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG2, c); // Write new ACCEL_CONFIG2 register value
 // The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
 // but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

  // Configure Interrupts and Bypass Enable
  // Set interrupt pin active high, push-pull, hold interrupt pin level HIGH until interrupt cleared,
  // clear on read of INT_STATUS, and enable I2C_BYPASS_EN so additional chips
  // can join the I2C bus and all can be controlled by the Arduino as master
   I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, INT_PIN_CFG, 0x22);
   I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt
   vTaskDelay(pdMS_TO_TICKS(100));;
}


// Function which accumulates gyro and accelerometer data after device
// initialization. It calculates the average of the at-rest readings and then
// loads the resulting offsets into accelerometer and gyro bias registers.
void calibrateMPU9250(float * gyroBias, float * accelBias)
{
  uint8_t data[12]; // data array to hold accelerometer and gyro x, y, z, data
  uint16_t ii, packet_count, fifo_count;
  int32_t gyro_bias[3]  = {0, 0, 0}, accel_bias[3] = {0, 0, 0};

  // reset device
  // Write a one to bit 7 reset bit; toggle reset device
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, PWR_MGMT_1, 0x80);
  vTaskDelay(pdMS_TO_TICKS(100));

 // get stable time source; Auto select clock source to be PLL gyroscope
 // reference if ready else use the internal oscillator, bits 2:0 = 001
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, PWR_MGMT_1, 0x01);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, PWR_MGMT_2, 0x00);
  vTaskDelay(pdMS_TO_TICKS(200));

  // Configure device for bias calculation
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, INT_ENABLE, 0x00);   // Disable all interrupts
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, FIFO_EN, 0x00);      // Disable FIFO
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, PWR_MGMT_1, 0x00);   // Turn on internal clock source
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, I2C_MST_CTRL, 0x00); // Disable I2C master
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, USER_CTRL, 0x00);    // Disable FIFO and I2C master modes
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, USER_CTRL, 0x0C);    // Reset FIFO and DMP
  vTaskDelay(pdMS_TO_TICKS(15));

// Configure MPU6050 gyro and accelerometer for bias calculation
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, CONFIG, 0x01);      // Set low-pass filter to 188 Hz
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, SMPLRT_DIV, 0x00);  // Set sample rate to 1 kHz
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, GYRO_CONFIG, 0x00);  // Set gyro full-scale to 250 degrees per second, maximum sensitivity
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG, 0x00); // Set accelerometer full-scale to 2 g, maximum sensitivity

  uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
  uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

    // Configure FIFO to capture accelerometer and gyro data for bias calculation
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, USER_CTRL, 0x40);   // Enable FIFO
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, FIFO_EN, 0x78);     // Enable gyro and accelerometer sensors for FIFO  (max size 512 bytes in MPU-9150)
  vTaskDelay(pdMS_TO_TICKS(40)); // accumulate 40 samples in 40 milliseconds = 480 bytes

// At end of sample accumulation, turn off FIFO sensor read
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, FIFO_EN, 0x00);        // Disable gyro and accelerometer sensors for FIFO
  I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, FIFO_COUNTH, 2, &data[0]); // read FIFO sample count
  fifo_count = ((uint16_t)data[0] << 8) | data[1];
  packet_count = fifo_count/12;// How many sets of full gyro and accelerometer data for averaging

  for (ii = 0; ii < packet_count; ii++)
  {
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, FIFO_R_W, 12, &data[0]); // read data for averaging
    accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  );  // Form signed 16-bit integer for each sample in FIFO
    accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  );
    accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  );
    gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  );
    gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  );
    gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]);

    accel_bias[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
    accel_bias[1] += (int32_t) accel_temp[1];
    accel_bias[2] += (int32_t) accel_temp[2];
    gyro_bias[0]  += (int32_t) gyro_temp[0];
    gyro_bias[1]  += (int32_t) gyro_temp[1];
    gyro_bias[2]  += (int32_t) gyro_temp[2];
  }

  accel_bias[0] /= (int32_t) packet_count; // Normalize sums to get average count biases
  accel_bias[1] /= (int32_t) packet_count;
  accel_bias[2] /= (int32_t) packet_count;
  gyro_bias[0]  /= (int32_t) packet_count;
  gyro_bias[1]  /= (int32_t) packet_count;
  gyro_bias[2]  /= (int32_t) packet_count;

  if(accel_bias[2] > 0L) {accel_bias[2] -= (int32_t) accelsensitivity;}  // Remove gravity from the z-axis accelerometer bias calculation
  else {accel_bias[2] += (int32_t) accelsensitivity;}

// Construct the gyro biases for push to the hardware gyro bias registers, which are reset to zero upon device startup
  data[0] = (-gyro_bias[0]/4  >> 8) & 0xFF; // Divide by 4 to get 32.9 LSB per deg/s to conform to expected bias input format
  data[1] = (-gyro_bias[0]/4)       & 0xFF; // Biases are additive, so change sign on calculated average gyro biases
  data[2] = (-gyro_bias[1]/4  >> 8) & 0xFF;
  data[3] = (-gyro_bias[1]/4)       & 0xFF;
  data[4] = (-gyro_bias[2]/4  >> 8) & 0xFF;
  data[5] = (-gyro_bias[2]/4)       & 0xFF;

// Push gyro biases to hardware registers
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, XG_OFFSET_H, data[0]);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, XG_OFFSET_L, data[1]);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, YG_OFFSET_H, data[2]);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, YG_OFFSET_L, data[3]);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ZG_OFFSET_H, data[4]);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ZG_OFFSET_L, data[5]);

// Output scaled gyro biases for display in the main program
  gyroBias[0] = (float) gyro_bias[0]/(float) gyrosensitivity;
  gyroBias[1] = (float) gyro_bias[1]/(float) gyrosensitivity;
  gyroBias[2] = (float) gyro_bias[2]/(float) gyrosensitivity;

// Construct the accelerometer biases for push to the hardware accelerometer bias registers. These registers contain
// factory trim values which must be added to the calculated accelerometer biases; on boot up these registers will hold
// non-zero values. In addition, bit 0 of the lower byte must be preserved since it is used for temperature
// compensation calculations. Accelerometer bias registers expect bias input as 2048 LSB per g, so that
// the accelerometer biases calculated above must be divided by 8.

  int32_t accel_bias_reg[3] = {0, 0, 0}; // A place to hold the factory accelerometer trim biases
  I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, XA_OFFSET_H, 2, &data[0]); // Read factory accelerometer trim values
  accel_bias_reg[0] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, YA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[1] = (int32_t) (((int16_t)data[0] << 8) | data[1]);
  I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, ZA_OFFSET_H, 2, &data[0]);
  accel_bias_reg[2] = (int32_t) (((int16_t)data[0] << 8) | data[1]);

  uint32_t mask = 1uL; // Define mask for temperature compensation bit 0 of lower byte of accelerometer bias registers
  uint8_t mask_bit[3] = {0, 0, 0}; // Define array to hold mask bit for each accelerometer bias axis

  for(ii = 0; ii < 3; ii++) {
    if((accel_bias_reg[ii] & mask)) mask_bit[ii] = 0x01; // If temperature compensation bit is set, record that fact in mask_bit
  }

  // Construct total accelerometer bias, including calculated average accelerometer bias from above
  accel_bias_reg[0] -= (accel_bias[0]/8); // Subtract calculated averaged accelerometer bias scaled to 2048 LSB/g (16 g full scale)
  accel_bias_reg[1] -= (accel_bias[1]/8);
  accel_bias_reg[2] -= (accel_bias[2]/8);

  data[0] = (accel_bias_reg[0] >> 8) & 0xFF;
  data[1] = (accel_bias_reg[0])      & 0xFF;
  data[1] = data[1] | mask_bit[0]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[2] = (accel_bias_reg[1] >> 8) & 0xFF;
  data[3] = (accel_bias_reg[1])      & 0xFF;
  data[3] = data[3] | mask_bit[1]; // preserve temperature compensation bit when writing back to accelerometer bias registers
  data[4] = (accel_bias_reg[2] >> 8) & 0xFF;
  data[5] = (accel_bias_reg[2])      & 0xFF;
  data[5] = data[5] | mask_bit[2]; // preserve temperature compensation bit when writing back to accelerometer bias registers

// Apparently this is not working for the acceleration biases in the MPU-9250
// Are we handling the temperature correction bit properly?
// Push accelerometer biases to hardware registers
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, XA_OFFSET_H, data[0]);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, XA_OFFSET_L, data[1]);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, YA_OFFSET_H, data[2]);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, YA_OFFSET_L, data[3]);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ZA_OFFSET_H, data[4]);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ZA_OFFSET_L, data[5]);

// Output scaled accelerometer biases for display in the main program
   accelBias[0] = (float)accel_bias[0]/(float)accelsensitivity;
   accelBias[1] = (float)accel_bias[1]/(float)accelsensitivity;
   accelBias[2] = (float)accel_bias[2]/(float)accelsensitivity;
}


// Accelerometer and gyroscope self test; check calibration wrt factory settings
void MPU9250SelfTest(float * destination) // Should return percent deviation from factory trim values, +/- 14 or less deviation is a pass
{
  uint8_t rawData[6] = {0, 0, 0, 0, 0, 0};
  uint8_t selfTest[6];
  int16_t gAvg[3], aAvg[3], aSTAvg[3], gSTAvg[3];
  float factoryTrim[6];
  uint8_t FS = 0;
  int ii;

  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, SMPLRT_DIV, 0x00);    // Set gyro sample rate to 1 kHz
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, CONFIG, 0x02);        // Set gyro sample rate to 1 kHz and DLPF to 92 Hz
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, GYRO_CONFIG, 1<<FS);  // Set full scale range for the gyro to 250 dps
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG2, 0x02); // Set accelerometer rate to 1 kHz and bandwidth to 92 Hz
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG, 1<<FS); // Set full scale range for the accelerometer to 2 g

  for( ii = 0; ii < 200; ii++) {  // get average current values of gyro and acclerometer

    I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]);        // Read the six raw data registers into data array
    aAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    aAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    aAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;

    I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]);       // Read the six raw data registers sequentially into data array
    gAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    gAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    gAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
  }

  for (ii =0; ii < 3; ii++) {  // Get average of 200 values and store as average current readings
    aAvg[ii] /= 200;
    gAvg[ii] /= 200;
  }

// Configure the accelerometer for self-test
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG, 0xE0); // Enable self test on all three axes and set accelerometer range to +/- 2 g
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, GYRO_CONFIG,  0xE0); // Enable self test on all three axes and set gyro range to +/- 250 degrees/s
  vTaskDelay(pdMS_TO_TICKS(25));  // Delay a while to let the device stabilize

  for( ii = 0; ii < 200; ii++) {  // get average self-test values of gyro and acclerometer

    I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_XOUT_H, 6, &rawData[0]);  // Read the six raw data registers into data array
    aSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    aSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    aSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;

    I2C_read_bytes(&i2c_instance, MPU9250_SLAVE_ADDRESS, GYRO_XOUT_H, 6, &rawData[0]);  // Read the six raw data registers sequentially into data array
    gSTAvg[0] += (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    gSTAvg[1] += (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    gSTAvg[2] += (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
  }

  for (ii =0; ii < 3; ii++) {  // Get average of 200 values and store as average self-test readings
    aSTAvg[ii] /= 200;
    gSTAvg[ii] /= 200;
  }

  // Configure the gyro and accelerometer for normal operation
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, ACCEL_CONFIG, 0x00);
  I2C_write_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, GYRO_CONFIG,  0x00);
  vTaskDelay(pdMS_TO_TICKS(25));  // Delay a while to let the device stabilize

  // Retrieve accelerometer and gyro factory Self-Test Code from USR_Reg
  selfTest[0] = I2C_read_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, SELF_TEST_X_ACCEL); // X-axis accel self-test results
  selfTest[1] = I2C_read_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, SELF_TEST_Y_ACCEL); // Y-axis accel self-test results
  selfTest[2] = I2C_read_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, SELF_TEST_Z_ACCEL); // Z-axis accel self-test results
  selfTest[3] = I2C_read_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, SELF_TEST_X_GYRO);  // X-axis gyro self-test results
  selfTest[4] = I2C_read_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, SELF_TEST_Y_GYRO);  // Y-axis gyro self-test results
  selfTest[5] = I2C_read_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, SELF_TEST_Z_GYRO);  // Z-axis gyro self-test results

  // Retrieve factory self-test value from self-test code reads
  factoryTrim[0] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[0] - 1.0) )); // FT[Xa] factory trim calculation
  factoryTrim[1] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[1] - 1.0) )); // FT[Ya] factory trim calculation
  factoryTrim[2] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[2] - 1.0) )); // FT[Za] factory trim calculation
  factoryTrim[3] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[3] - 1.0) )); // FT[Xg] factory trim calculation
  factoryTrim[4] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[4] - 1.0) )); // FT[Yg] factory trim calculation
  factoryTrim[5] = (float)(2620/1<<FS)*(pow( 1.01 , ((float)selfTest[5] - 1.0) )); // FT[Zg] factory trim calculation

 // Report results as a ratio of (STR - FT)/FT; the change from Factory Trim of the Self-Test Response
 // To get percent, must multiply by 100
  int i;
  for (i = 0; i < 3; i++) {
    destination[i]   = 100.0*((float)(aSTAvg[i] - aAvg[i]))/factoryTrim[i];   // Report percent differences
    destination[i+3] = 100.0*((float)(gSTAvg[i] - gAvg[i]))/factoryTrim[i+3]; // Report percent differences
  }
}

void MPU9250_print_orientation(void)
{
	if (I2C_read_byte(&i2c_instance, MPU9250_SLAVE_ADDRESS, INT_STATUS) & 0x01)
	{
		readAccelData(accelCount);  // Read the x/y/z adc values
		getAres();

		// Now we'll calculate the accleration value into actual g's
		// This depends on scale being set
		ax = (float)accelCount[0]*aRes; // - accelBias[0];
		ay = (float)accelCount[1]*aRes; // - accelBias[1];
		az = (float)accelCount[2]*aRes; // - accelBias[2];

		readGyroData(gyroCount);  // Read the x/y/z adc values
		getGres();

		// Calculate the gyro value into actual degrees per second
		// This depends on scale being set
		gx = (float)gyroCount[0]*gRes;
		gy = (float)gyroCount[1]*gRes;
		gz = (float)gyroCount[2]*gRes;

		readMagData(magCount);  // Read the x/y/z adc values
		getMres();
		// User environmental x-axis correction in milliGauss, should be
		// automatically calculated
		magbias[0] = +470.;
		// User environmental x-axis correction in milliGauss TODO axis??
		magbias[1] = +120.;
		// User environmental x-axis correction in milliGauss
		magbias[2] = +125.;

		// Calculate the magnetometer values in milliGauss
		// Include factory calibration per data sheet and user environmental
		// corrections
		// Get actual magnetometer value, this depends on scale being set
		mx = (float)magCount[0]*mRes*magCalibration[0] - magbias[0];
		my = (float)magCount[1]*mRes*magCalibration[1] - magbias[1];
		mz = (float)magCount[2]*mRes*magCalibration[2] - magbias[2];

	}

	updateTime();

	  // Sensors x (y)-axis of the accelerometer is aligned with the y (x)-axis of
	  // the magnetometer; the magnetometer z-axis (+ down) is opposite to z-axis
	  // (+ up) of accelerometer and gyro! We have to make some allowance for this
	  // orientationmismatch in feeding the output to the quaternion filter. For the
	  // MPU-9250, we have chosen a magnetic rotation that keeps the sensor forward
	  // along the x-axis just like in the LSM9DS0 sensor. This rotation can be
	  // modified to allow any convenient orientation convention. This is ok by
	  // aircraft orientation standards! Pass gyro rate as rad/s

	MadgwickQuaternionUpdate(ax, ay, az, gx*DEG_TO_RAD, gy*DEG_TO_RAD, gz*DEG_TO_RAD,  my,  mx, mz, deltat);
	//MahonyQuaternionUpdate(ax, ay, az, gx*DEG_TO_RAD, gy*DEG_TO_RAD, gz*DEG_TO_RAD, my, mx, mz, deltat);

	//delt_t = millis() - count;
	//if (delt_t > 500)
	//{
		//count = millis();

		euler_angles[0]   = atan2(2.0f * (*(getQ()+1) * *(getQ()+2) + *getQ() *
			  *(getQ()+3)), *getQ() * *getQ() + *(getQ()+1) * *(getQ()+1)
			  - *(getQ()+2) * *(getQ()+2) - *(getQ()+3) * *(getQ()+3));
		euler_angles[1] = -asin(2.0f * (*(getQ()+1) * *(getQ()+3) - *getQ() *
			  *(getQ()+2)));
		euler_angles[2]  = atan2(2.0f * (*getQ() * *(getQ()+1) + *(getQ()+2) *
			  *(getQ()+3)), *getQ() * *getQ() - *(getQ()+1) * *(getQ()+1)
			  - *(getQ()+2) * *(getQ()+2) + *(getQ()+3) * *(getQ()+3));
		euler_angles[1] *= RAD_TO_DEG;
		euler_angles[0]   *= RAD_TO_DEG;
		// Declination of SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) is
		// 	8° 30' E  ± 0° 21' (or 8.5°) on 2016-07-19
		// - http://www.ngdc.noaa.gov/geomag-web/#declination
		euler_angles[0]   -= 8.5;
		euler_angles[2]  *= RAD_TO_DEG;

		printf("Yaw, Pitch, Roll, rate: %d %d %d %d\n\r", (int)euler_angles[0], (int)euler_angles[1], (int)euler_angles[2], (int)(sumCount/sum));
	//}
}

