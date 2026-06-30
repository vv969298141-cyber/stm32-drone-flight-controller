#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>

int32_t limit_int32(int32_t value, int32_t min, int32_t max);

void motor_set_pwm(int32_t m1, int32_t m2, int32_t m3, int32_t m4);

void motor_lock_output(int32_t *m1, int32_t *m2, int32_t *m3, int32_t *m4);

void motor_mix(int32_t base,
               float roll_output,
               float pitch_output,
               int32_t *m1,
               int32_t *m2,
               int32_t *m3,
               int32_t *m4);

#endif