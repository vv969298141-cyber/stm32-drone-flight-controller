#ifndef MPU6050_H
#define MPU6050_H

#include "stm32f4xx_hal.h"
#include "flight_types.h"

HAL_StatusTypeDef mpu6050_read_raw(MPU6050_RawData *raw);
void mpu6050_calibrate_gyro(void);

#endif