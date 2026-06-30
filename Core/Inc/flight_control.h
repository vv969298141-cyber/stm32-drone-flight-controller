#ifndef FLIGHT_CONTROL_H
#define FLIGHT_CONTROL_H

#include <stdint.h>
#include "flight_types.h"

void flight_control_update(FlightState state,
                           AttitudeData *attitude_ptr,
                           float dt,
                           int32_t base_pwm,
                           float *roll_pid_output_ptr,
                           float *pitch_pid_output_ptr,
                           int32_t *m1,
                           int32_t *m2,
                           int32_t *m3,
                           int32_t *m4);

#endif