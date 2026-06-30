#include "app_command.h"
#include <stdio.h>
#include "usart.h"
#include "app_uart.h"
#include "app_state.h"
#include "attitude.h"
#include "motor.h"
#include "cmsis_os.h"
#include "uart_queue.h"

void app_command_check(int32_t *base_pwm_ptr, uint32_t *last_cmd_tick_ptr,AttitudeData *attitude_ptr)
{
    uint8_t cmd;    // baocun jieshou dao de mingling

    if (HAL_UART_Receive(&huart1, &cmd, 1, 0) == HAL_OK)    // fei zuse jieshou 1 ge zifu
    {
			 *last_cmd_tick_ptr = HAL_GetTick(); 
			if (cmd == 'x')    // emergency stop
{
    flight_state = FLIGHT_LOCKED;
    *base_pwm_ptr = 1000;
    motor_lock_output(&motor1, &motor2, &motor3, &motor4);
    uart_queue_send_string("CMD=EMERGENCY_STOP\r\n");
}
        else if (cmd == 'a')    // shuru a jiesuo
        {
            if (flight_state == FLIGHT_ERROR)    // cuowu zhuangtai bu neng jiesuo
    {
        uart_queue_send_string("CMD=ARM_DENIED_ERROR\r\n");
    }
		else if (*base_pwm_ptr != 1000)
{
    uart_queue_send_string("CMD=ARM_DENIED_BASE\r\n");
}
    else if (attitude_is_safe(attitude_ptr) == 0)
    {
        uart_queue_send_string("CMD=ARM_DENIED_ANGLE\r\n");    // jiaodu guoda, bu jiesuo
    }
    else
    {
        flight_state = FLIGHT_ARMED;
        uart_queue_send_string("CMD=ARM\r\n");
    }
}
        else if (cmd == 'd')    // 5shuru d suoding
        {
            flight_state = FLIGHT_LOCKED;
            uart_queue_send_string("CMD=DISARM\r\n");
        }
        else if (cmd == 'r')    // shuru r cong ERROR huifu dao LOCKED
        {
            flight_state = FLIGHT_LOCKED;
            uart_queue_send_string("CMD=RESET_TO_LOCKED\r\n");
        }
        else if (cmd == 'u')    // shuru u zengjia jichu youmen
        {
            *base_pwm_ptr = limit_int32(*base_pwm_ptr + 10, 1000, 1100);

            char cmd_msg[64];    // baocun mingling fan hui wen ben

snprintf(cmd_msg,
         sizeof(cmd_msg),
         "CMD=BASE_UP BASE=%ld\r\n",
         (long)(*base_pwm_ptr));

uart_queue_send_string(cmd_msg);
        }
        else if (cmd == 'j')    // shuru j jianshao jichu youmen
        {
            *base_pwm_ptr = limit_int32(*base_pwm_ptr - 10, 1000, 1100);

            char cmd_msg[64];    // baocun mingling fan hui wen ben

snprintf(cmd_msg,
         sizeof(cmd_msg),
         "CMD=BASE_DOWN BASE=%ld\r\n",
         (long)(*base_pwm_ptr));

uart_queue_send_string(cmd_msg);
        }
    }
}