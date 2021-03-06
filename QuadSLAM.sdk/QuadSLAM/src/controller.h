/*
 * controller.h
 *
 *  Created on: Nov 14, 2017
 *      Author: Ryan
 */

#ifndef SRC_CONTROLLER_H_
#define SRC_CONTROLLER_H_

#include "xtime_l.h"
#include "pwm.h"

#define QUADCOPTER_ARMED 1800 //when greater than 1900, is armed

enum axes {
  YAW = 0,
  PITCH,
  ROLL,
};

typedef struct {
	uint8_t axes;

	float Kp;
	float Ki;
	float Kd;

	float previous_error;
	float error_sum;
	XTime last_time;

	float i_limit;

	int first_loop;

	int output;
} PID;

void controller_init();

void pid_update(PID* pid, float sp);
void controller_update(PWM_input* pwm_input);

#endif /* SRC_CONTROLLER_H_ */
