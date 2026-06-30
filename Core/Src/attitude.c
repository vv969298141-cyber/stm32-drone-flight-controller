#include "math.h"
#include "attitude.h"

uint8_t attitude_is_safe(AttitudeData *attitude_ptr)
{
    if ((attitude_ptr->fused_roll > ANGLE_LIMIT_DEG) ||
        (attitude_ptr->fused_roll < -ANGLE_LIMIT_DEG) ||
        (attitude_ptr->fused_pitch > ANGLE_LIMIT_DEG) ||
        (attitude_ptr->fused_pitch < -ANGLE_LIMIT_DEG))
    {
        return 0;    // jiaodu bu anquan
    }

    return 1;        // jiaodu anquan
}


void attitude_update(MPU6050_RawData *raw, AttitudeData *attitude, float dt)
{
    float acc_x_g;
    float acc_y_g;
    float acc_z_g;
    float gyro_x_dps;
    float gyro_y_dps;

    acc_x_g = (float)raw->acc_x / 16384.0f;
    acc_y_g = (float)raw->acc_y / 16384.0f;
    acc_z_g = (float)raw->acc_z / 16384.0f;

    gyro_x_dps = (float)raw->gyro_x / 131.0f;
    gyro_y_dps = (float)raw->gyro_y / 131.0f;

    attitude->acc_roll = atan2f(acc_y_g, acc_z_g) * 57.29578f;

    attitude->acc_pitch = atan2f(-acc_x_g,
                                 sqrtf(acc_y_g * acc_y_g + acc_z_g * acc_z_g))
                                 * 57.29578f;

    attitude->gyro_roll += gyro_x_dps * dt;
    attitude->gyro_pitch += gyro_y_dps * dt;

    attitude->fused_roll = 0.98f * (attitude->fused_roll + gyro_x_dps * dt)
                         + 0.02f * attitude->acc_roll;

    attitude->fused_pitch = 0.98f * (attitude->fused_pitch + gyro_y_dps * dt)
                          + 0.02f * attitude->acc_pitch;
}