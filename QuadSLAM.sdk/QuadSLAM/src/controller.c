/*
 * controller.c
 *
 *  Created on: Nov 14, 2017
 *      Author: Ryan
 */

#include "controller.h"
#include "util.h"
#include "pwm.h"

#define THROTTLE_PWM_MIN 1000
#define THROTTLE_PWM_MAX 2000
#define THROTTLE_MAX_OUTPUT 1800

#define YAW_KP 0
#define YAW_KI 0
#define YAW_KD 0
#define YAW_RANGE 20.0
#define YAW_PWM_MID 1450
#define YAW_PWM_MIN 900
#define YAW_PWM_MAX 2000
#define YAW_PWM_RANGE 1100

#define PITCH_KP 23
#define PITCH_KI 0
#define PITCH_KD 0
#define PITCH_RANGE 20.0
#define PITCH_PWM_MID 1450
#define PITCH_PWM_MIN 900
#define PITCH_PWM_MAX 2000
#define PITCH_PWM_RANGE 1100

#define ROLL_KP 5
#define ROLL_KI 0
#define ROLL_KD 0
#define ROLL_RANGE 20.0 //10 degrees both ways
#define ROLL_PWM_MID 1450
#define ROLL_PWM_MIN 900
#define ROLL_PWM_MAX 2000
#define ROLL_PWM_RANGE 1100

PID yaw, pitch, roll;
extern float euler_angles[3];

void controller_init()
{
	yaw.axes = YAW;
	yaw.Kp = YAW_KP;
	yaw.Ki = YAW_KI;
	yaw.Kd = YAW_KD;
	yaw.error_sum = 0;
	yaw.previous_error = 0;
	yaw.i_limit = 10;

	pitch.axes = PITCH;
	pitch.Kp = PITCH_KP;
	pitch.Ki = PITCH_KI;
	pitch.Kd = PITCH_KD;
	pitch.error_sum = 0;
	pitch.previous_error = 0;
	pitch.i_limit = 10;

	roll.axes = ROLL;
	roll.Kp = ROLL_KP;
	roll.Ki = ROLL_KI;
	roll.Kd = ROLL_KD;
	roll.error_sum = 0;
	roll.previous_error = 0;
	roll.i_limit = 10;
}


void controller_update(PWM_input* pwm_input)
{
	uint16_t pwm_out[4];

	//TODO: Handle arming and other flight control states e.g. lost connection, autonomous, manual mode

	//TODO: Handle yaw . Right now the yaw will attempt to stay 0, however we will want the quad to rotate. This value will involve our image processing pipeline
	//maps input pwm roll pitch and yaw to their respective angles
	float yaw_sp = (pwm_input->yaw - YAW_PWM_MID) * YAW_RANGE / YAW_PWM_RANGE;
	float pitch_sp = (pwm_input->pitch - PITCH_PWM_MID) * PITCH_RANGE / PITCH_PWM_RANGE;
	float roll_sp = (pwm_input->roll - ROLL_PWM_MID) * ROLL_RANGE / ROLL_PWM_RANGE;
	float throttle;
	int i;

	//TODO:handle case where connection is lost with the transmitter
	if(pwm_input->throttle < THROTTLE_PWM_MIN)
		throttle = 5;

	throttle = MOTOR_PWM_MIN + (MOTOR_PWM_MAX - MOTOR_PWM_MIN) * (pwm_input->throttle - THROTTLE_PWM_MIN) / (THROTTLE_PWM_MAX - THROTTLE_PWM_MIN);
	if(throttle > THROTTLE_MAX_OUTPUT)
		throttle = THROTTLE_MAX_OUTPUT;

	pid_update(&yaw, yaw_sp);
	pid_update(&pitch, pitch_sp);
	pid_update(&roll, roll_sp);

	//TODO: Do this summation properly. Signals may be out of order and have wrong sign.
	pwm_out[0] = throttle + roll.output - yaw.output;
	pwm_out[1] = throttle + pitch.output + yaw.output;
	pwm_out[2] = throttle + roll.output - yaw.output;
	pwm_out[3] = throttle + pitch.output + yaw.output;


	//constain each of the pwm outputs to the motors.
	for(i = 0; i < 4; i++)
	{
		if(pwm_out[i] > MOTOR_PWM_MAX)
			pwm_out[i] = MOTOR_PWM_MAX;
		if(pwm_out[i] < MOTOR_PWM_MIN)
			pwm_out[i] = MOTOR_PWM_MIN;
	}

	set_pwm_output(pwm_out);
}

void pid_update(PID* pid, float sp)
{
	float pv = euler_angles[pid->axes];
	float error = pv - sp;
	float output;

	XTime current_time;
	XTime_GetTime(&current_time);

	//TODO: handle first loop iteration case where last_time will be zero but current_time will be large, resulting in integral explosion
	float dt = (current_time - pid->last_time) / 325000000.0f;
	pid->error_sum += error*dt;

	//should be used to bound integral term.
	if(pid->error_sum > pid->i_limit)
		pid->error_sum = pid->i_limit;
	if(pid->error_sum < -pid->i_limit)
		pid->error_sum = -pid->i_limit;


	output = pid->Kp * error + pid->Ki * pid->error_sum + pid->Kd * (error - pid->previous_error) / dt;

	//TODO:Potentially bound output ?

	pid->output = (int32_t)output;

	pid->previous_error = error;
	pid->last_time = current_time;
}
