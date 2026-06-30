#ifndef FLIGHT_TYPES_H
#define FLIGHT_TYPES_H

#include <stdint.h>

#define ANGLE_LIMIT_DEG 45.0f

typedef enum
{
    FLIGHT_LOCKED = 0,    // suoding zhuangtai
    FLIGHT_ARMED  = 1,    // jiesuo zhuangtai
    FLIGHT_ERROR  = 2     // cuowu zhuangtai
} FlightState;

typedef struct
{
  float kp;
  float ki;
  float kd;

  float integral;
  float last_error;

  float output;
} PIDController;

typedef struct
{
  float acc_roll;     // jia su du ji suan de roll jiao
  float acc_pitch;    // jia su du ji suan de pitch jiao

  float gyro_roll;    // tuo luo yi ji fen de roll jiao
  float gyro_pitch;   // tuo luo yi ji fen de pitch jiao

  float fused_roll;   // hu bu lv bo hou de roll jiao
  float fused_pitch;  // hu bu lv bo hou de pitch jiao
} AttitudeData;

typedef struct
{
  int16_t acc_x;   // jia su du X yuan shi shu ju
  int16_t acc_y;   // jia su du Y yuan shi shu ju
  int16_t acc_z;   // jia su du Z yuan shi shu ju

  int16_t gyro_x;  // tuo luo yi X qu ling pian hou shu ju
  int16_t gyro_y;  // tuo luo yi Y qu ling pian hou shu ju
  int16_t gyro_z;  // tuo luo yi Z qu ling pian hou shu ju
} MPU6050_RawData;

#endif
