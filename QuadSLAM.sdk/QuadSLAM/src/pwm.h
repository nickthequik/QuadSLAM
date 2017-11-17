
#ifndef SRC_PWM_H_
#define SRC_PWM_H_

#include "gpio.h"

//PWM Detector Masks
#define THROTTLE_MASK    ( 0x0007FF )
#define ROLL_MASK        ( 0x3FF800 )
#define PITCH_MASK       ( 0x0007FF )
#define YAW_MASK         ( 0x3FF800 )
#define ARM_MASK         ( 0x0007FF )
#define FLIGHT_MODE_MASK ( 0x3FF800 )

// PWM Generator Masks
#define MOTOR_1_MASK     ( 0x0007FF )
#define MOTOR_2_MASK     ( 0x3FF800 )
#define MOTOR_3_MASK     ( 0x0007FF )
#define MOTOR_4_MASK     ( 0x3FF800 )
#define PWM_ENABLE_MASK  ( 0x800000 )
#define PWM_RESET_MASK   ( 0x400000 )

// PWM min and max for motors
#define MOTOR_PWM_MAX    ( 2000 )
#define MOTOR_PWM_MIN    ( 1145 )

typedef struct pwm_input {
	uint16_t throttle;
	uint16_t roll;
	uint16_t pitch;
	uint16_t yaw;
	uint16_t arm;
	uint16_t flight_mode;
} PWM_input;

void init_pwm_detector(void);
void get_pwm_input(PWM_input *pwm_input);
void init_pwm_generator(void);
void reset_pwm_generator(void);
void enable_pwm_generator(void);
void disable_pwm_generator(void);
void set_pwm_output(uint16_t pwm[4]);

#endif /* SRC_PWM_H_ */
