#include "pid.h"

PIDController roll_pid = {1.0f, 0.0f, 0.05f, 0.0f, 0.0f, 0.0f};
PIDController pitch_pid = {1.0f, 0.0f, 0.05f, 0.0f, 0.0f, 0.0f};

float pid_update(PIDController *pid, float target, float measured, float dt)
{
  float error = target - measured;  // Calculate current error

  pid->integral += error * dt;  // Accumulate error over time

  float derivative = 0.0f;  // Error change rate

  if (dt > 0.0f)  // Avoid division by zero
  {
    derivative = (error - pid->last_error) / dt;  // Calculate derivative term
  }

  pid->output = pid->kp * error
              + pid->ki * pid->integral
              + pid->kd * derivative;  // PID output

  pid->last_error = error;  // Save current error for next loop

  return pid->output;  // Return PID result
}