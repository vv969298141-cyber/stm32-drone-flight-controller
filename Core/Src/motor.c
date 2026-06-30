#include "motor.h"
#include "tim.h"


int32_t limit_int32(int32_t value, int32_t min, int32_t max)
{
  if (value < min)  // xiao yu zui xiao zhi, fan hui min
  {
    return min;
  }

  if (value > max)  // da yu zui da zhi, fan hui max
  {
    return max;
  }

  return value;  // zai fan wei nei, yuan yang fan hui
}


void motor_set_pwm(int32_t m1, int32_t m2, int32_t m3, int32_t m4)
{
  m1 = limit_int32(m1, 1000, 2000);  // xian zhi M1 zai 1000 dao 2000
  m2 = limit_int32(m2, 1000, 2000);  // xian zhi M2 zai 1000 dao 2000
  m3 = limit_int32(m3, 1000, 2000);  // xian zhi M3 zai 1000 dao 2000
  m4 = limit_int32(m4, 1000, 2000);  // xian zhi M4 zai 1000 dao 2000

  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1, m1);  // she zhi TIM4 CH1 PWM
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2, m2);  // she zhi TIM4 CH2 PWM
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, m3);  // she zhi TIM4 CH3 PWM
  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, m4);  // she zhi TIM4 CH4 PWM
}

void motor_lock_output(int32_t *m1, int32_t *m2, int32_t *m3, int32_t *m4)
{
    *m1 = 1000;    // M1 suoding dao zuixiao PWM
    *m2 = 1000;    // M2 suoding dao zuixiao PWM
    *m3 = 1000;    // M3 suoding dao zuixiao PWM
    *m4 = 1000;    // M4 suoding dao zuixiao PWM

    motor_set_pwm(1000, 1000, 1000, 1000);    // shiji PWM shuchu ye suoding
}

void motor_mix(int32_t base,
               float roll_output,
               float pitch_output,
               int32_t *m1,
               int32_t *m2,
               int32_t *m3,
               int32_t *m4)
{
    *m1 = base + (int32_t)pitch_output + (int32_t)roll_output;
    *m2 = base + (int32_t)pitch_output - (int32_t)roll_output;
    *m3 = base - (int32_t)pitch_output - (int32_t)roll_output;
    *m4 = base - (int32_t)pitch_output + (int32_t)roll_output;
}