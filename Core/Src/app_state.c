#include "app_state.h"

uint32_t last_cmd_tick = 0;
int32_t base_pwm = 1000;

int32_t motor1 = 1000;
int32_t motor2 = 1000;
int32_t motor3 = 1000;
int32_t motor4 = 1000;

float roll_pid_output = 0.0f;
float pitch_pid_output = 0.0f;

FlightState flight_state = FLIGHT_LOCKED;    // dangqian feikong zhuangtai

AttitudeData attitude = {0};
MPU6050_RawData raw;            // baocun MPU6050 yuan shi shuju
