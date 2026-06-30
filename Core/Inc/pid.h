#ifndef PID_H
#define PID_H

#include "flight_types.h"

extern PIDController roll_pid;
extern PIDController pitch_pid;

float pid_update(PIDController *pid, float target, float measured, float dt);

#endif