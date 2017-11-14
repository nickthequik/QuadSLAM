
#include "pwm.h"

uint32_t pwm_gen_ctrl_1 = 0x400000; // pwm_generator starts in reset
uint32_t pwm_gen_ctrl_2 = 0;

void init_pwm_detector(void)
{
	XGpio_DiscreteWrite(&xGpio4, GPIO_CHANNEL_2, 0);
}

void get_pwm_input(PWM_input *pwm_input)
{
	uint32_t pwm_read_1, pwm_read_2, pwm_read_3;

	pwm_read_1 = XGpio_DiscreteRead(&xGpio3, GPIO_CHANNEL_1);
	pwm_read_2 = XGpio_DiscreteRead(&xGpio3, GPIO_CHANNEL_2);
	pwm_read_3 = XGpio_DiscreteRead(&xGpio4, GPIO_CHANNEL_1);

	pwm_input->throttle = 2 * (THROTTLE_MASK & pwm_read_1);
	pwm_input->roll = 2 * ((ROLL_MASK & pwm_read_1) >> 10);
	pwm_input->pitch = 2 * (PITCH_MASK & pwm_read_2);
	pwm_input->yaw = 2 * ((YAW_MASK & pwm_read_2) >> 10);
	pwm_input->arm = 2 * (ARM_MASK & pwm_read_3);
	pwm_input->flight_mode = 2 * ((FLIGHT_MODE_MASK & pwm_read_3) >> 10);
}

void init_pwm_generator(void)
{
	pwm_gen_ctrl_1 &= ~PWM_RESET_MASK;
	XGpio_DiscreteWrite(&xGpio5, GPIO_CHANNEL_1, pwm_gen_ctrl_1);
}

void reset_pwm_generator(void)
{
	pwm_gen_ctrl_1 |= PWM_RESET_MASK;
	XGpio_DiscreteWrite(&xGpio5, GPIO_CHANNEL_1, pwm_gen_ctrl_1);
}

void enable_pwm_generator(void)
{
	pwm_gen_ctrl_1 |= PWM_ENABLE_MASK;
	XGpio_DiscreteWrite(&xGpio5, GPIO_CHANNEL_1, pwm_gen_ctrl_1);
}

void disable_pwm_generator(void)
{
	pwm_gen_ctrl_1 &= ~PWM_ENABLE_MASK;
	XGpio_DiscreteWrite(&xGpio5, GPIO_CHANNEL_1, pwm_gen_ctrl_1);
}

void set_pwm_output(uint16_t pwm[4])
{
	pwm_gen_ctrl_1 &= (PWM_ENABLE_MASK | PWM_RESET_MASK);
	pwm_gen_ctrl_2 = 0;

	pwm_gen_ctrl_1 |= (((pwm[1]/2) << 11) & MOTOR_2_MASK) | ((pwm[0]/2) & MOTOR_1_MASK);
	pwm_gen_ctrl_2 |= (((pwm[3]/2) << 11) & MOTOR_4_MASK) | ((pwm[2]/2) & MOTOR_3_MASK);;

	XGpio_DiscreteWrite(&xGpio5, GPIO_CHANNEL_1, pwm_gen_ctrl_1);
	XGpio_DiscreteWrite(&xGpio5, GPIO_CHANNEL_2, pwm_gen_ctrl_2);
}



