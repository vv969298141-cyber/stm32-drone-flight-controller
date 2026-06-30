#include "mpu6050.h"
#include "i2c.h"
#include "app_uart.h"


int16_t gyro_x_offset = 0;  // Gyro X zero offset
int16_t gyro_y_offset = 0;  // Gyro Y zero offset
int16_t gyro_z_offset = 0;  // Gyro Z zero offset

void mpu6050_calibrate_gyro(void)  // Calibrate gyro zero offset tuo luo yi qu 0
{
  int32_t sum_x = 0;  // Sum of gyro X samples
  int32_t sum_y = 0;  // Sum of gyro Y samples
  int32_t sum_z = 0;  // Sum of gyro Z samples

  uint8_t buf[14];  // Buffer for MPU6050 raw data

  uart1_send_string("Gyro calibration start\r\n");

  for (int i = 0; i < 200; i++)  // Collect 200 samples
  {
    HAL_I2C_Mem_Read(&hi2c1,                // Use I2C1
                     0x68 << 1,            // MPU6050 address
                     0x3B,                 // First data register
                     I2C_MEMADD_SIZE_8BIT, // 8-bit register address
                     buf,                  // Data buffer
                     14,                   // Read 14 bytes
                     100);                 // Timeout in ms

    int16_t gx = (int16_t)(buf[8] << 8 | buf[9]);    // Gyro X raw
    int16_t gy = (int16_t)(buf[10] << 8 | buf[11]);  // Gyro Y raw
    int16_t gz = (int16_t)(buf[12] << 8 | buf[13]);  // Gyro Z raw

    sum_x += gx;  // Add X sample
    sum_y += gy;  // Add Y sample
    sum_z += gz;  // Add Z sample

    HAL_Delay(5); // Wait 5ms between samples
  }

  gyro_x_offset = (int16_t)(sum_x / 200);  // Average X offset
  gyro_y_offset = (int16_t)(sum_y / 200);  // Average Y offset
  gyro_z_offset = (int16_t)(sum_z / 200);  // Average Z offset

  uart1_send_string("Gyro offset: ");
  uart1_send_int(gyro_x_offset);
  uart1_send_string(", ");
  uart1_send_int(gyro_y_offset);
  uart1_send_string(", ");
  uart1_send_int(gyro_z_offset);
  uart1_send_string("\r\n");

  uart1_send_string("Gyro calibration done\r\n");
}


HAL_StatusTypeDef mpu6050_read_raw(MPU6050_RawData *raw)
{
  uint8_t buf[14];
  HAL_StatusTypeDef ret;

  ret = HAL_I2C_Mem_Read(&hi2c1,
                         0x68 << 1,
                         0x3B,
                         I2C_MEMADD_SIZE_8BIT,
                         buf,
                         14,
                         100);

  if (ret != HAL_OK)
  {
    return ret;
  }

  raw->acc_x = (int16_t)(buf[0] << 8 | buf[1]);
  raw->acc_y = (int16_t)(buf[2] << 8 | buf[3]);
  raw->acc_z = (int16_t)(buf[4] << 8 | buf[5]);

  raw->gyro_x = (int16_t)(buf[8] << 8 | buf[9]) - gyro_x_offset;
  raw->gyro_y = (int16_t)(buf[10] << 8 | buf[11]) - gyro_y_offset;
  raw->gyro_z = (int16_t)(buf[12] << 8 | buf[13]) - gyro_z_offset;

  return HAL_OK;
}
