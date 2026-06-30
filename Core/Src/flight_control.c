#include "flight_control.h"
#include "pid.h"
#include "motor.h"


void flight_control_update(FlightState state,
                           AttitudeData *attitude_ptr,
                           float dt,
                           int32_t base_pwm,
                           float *roll_pid_output_ptr,
                           float *pitch_pid_output_ptr,
                           int32_t *m1,
                           int32_t *m2,
                           int32_t *m3,
                           int32_t *m4)
{
    *roll_pid_output_ptr = pid_update(&roll_pid,
                                      0.0f,
                                      attitude_ptr->fused_roll,
                                      dt);        // jisuan roll PID

    *pitch_pid_output_ptr = pid_update(&pitch_pid,
                                       0.0f,
                                       attitude_ptr->fused_pitch,
                                       dt);       // jisuan pitch PID

    if (state == FLIGHT_ARMED)    // jiesuo zhuangtai
    {
        motor_mix(base_pwm,
                  *roll_pid_output_ptr,
                  *pitch_pid_output_ptr,
                  m1,
                  m2,
                  m3,
                  m4);

        motor_set_pwm(*m1, *m2, *m3, *m4);    // shuchu PWM
    }
    else
    {
        motor_lock_output(m1, m2, m3, m4);    // suoding PWM
    }
}	