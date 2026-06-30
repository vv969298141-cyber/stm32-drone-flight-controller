#ifndef ATTITUDE_H
#define ATTITUDE_H

#include <stdint.h>
#include "flight_types.h"

uint8_t attitude_is_safe(AttitudeData *attitude_ptr);
void attitude_update(MPU6050_RawData *raw, AttitudeData *attitude, float dt);

#endif