#include "app_uart.h"
#include "usart.h"

void uart1_send_string(char *str) //chuankou
{
  while (*str != '\0')
  {
    uint8_t data = (uint8_t)(*str);
    HAL_UART_Transmit(&huart1, &data, 1, HAL_MAX_DELAY);
    str++;
  }
}

void uart1_send_int(int32_t num)  //chankou
{
  char buf[16];  // ?????????,int32_t?????

  int i = 0;  // ??????

  if (num == 0)  // ?????0,??????'0'
  {
    uart1_send_string("0");
    return;
  }

  if (num < 0)  // ?????,?????
  {
    uart1_send_string("-");
    num = -num;
  }

  while (num > 0)  // ??????????????
  {
    buf[i++] = (num % 10) + '0';  // ?????,?????
    num /= 10;  // ????
  }

  while (i > 0)  // ????,??????
  {
    i--;
    uint8_t data = (uint8_t)buf[i];
    HAL_UART_Transmit(&huart1, &data, 1, HAL_MAX_DELAY);
  }
}

void uart1_send_fixed_2(int32_t value100)  // xiao shu ding dian da yin
{
	
  if (value100 < 0)  // Handle negative value
  {
    uart1_send_string("-");
    value100 = -value100;
  }

  int32_t integer_part = value100 / 100;  // Integer part
  int32_t decimal_part = value100 % 100;  // Decimal part

  uart1_send_int(integer_part);  // Send integer part
  uart1_send_string(".");        // Send decimal point

  if (decimal_part < 10)  // Add leading zero for values like 0.05
  {
    uart1_send_string("0");
  }

  uart1_send_int(decimal_part);  // Send decimal part
}