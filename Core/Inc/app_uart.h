#ifndef APP_UART_H
#define APP_UART_H

#include <stdint.h>

void uart1_send_string(char *str);        // fa song zi fu chuan
void uart1_send_int(int32_t num);         // fa song zheng shu
void uart1_send_fixed_2(int32_t value100); // fa song fang da 100 bei de xiao shu

#endif