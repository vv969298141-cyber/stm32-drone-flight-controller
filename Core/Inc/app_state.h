#ifndef APP_STATE_H
#define APP_STATE_H

#include <stdint.h>
#include "flight_types.h"

extern uint32_t last_cmd_tick;

extern int32_t base_pwm;

extern int32_t motor1;
extern int32_t motor2;
extern int32_t motor3;
extern int32_t motor4;

extern float roll_pid_output;
extern float pitch_pid_output;

extern FlightState flight_state;

extern AttitudeData attitude;
extern MPU6050_RawData raw;

#endif