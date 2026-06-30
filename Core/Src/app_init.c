#include "app_init.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "i2c.h"
#include "mpu6050.h"
#include "motor.h"
void app_init(void)
{
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

	motor_set_pwm(1000, 1000, 1000, 1000);
	
  uint8_t data = 0x00;  // 0x00 means wake up MPU6050

  HAL_I2C_Mem_Write(&hi2c1,
                    0x68 << 1,
                    0x6B,
                    I2C_MEMADD_SIZE_8BIT,
                    &data,
                    1,
                    100);

  HAL_Delay(100);

  mpu6050_calibrate_gyro();
}

