#ifndef APP_COMMAND_H
#define APP_COMMAND_H

#include <stdint.h>
#include "flight_types.h"

void app_command_check(int32_t *base_pwm_ptr,
                       uint32_t *last_cmd_tick_ptr,
                       AttitudeData *attitude_ptr);

#endif