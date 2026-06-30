/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "flight_types.h"
#include <stdio.h>
#include "app_uart.h"
#include "mpu6050.h"
#include "attitude.h"
#include "motor.h"
#include "flight_control.h"
#include "app_state.h"
#include "app_command.h"
#include "uart_queue.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define UART_QUEUE_TEXT_SIZE 192
#define UART_QUEUE_LENGTH    8

typedef struct
{
  char text[UART_QUEUE_TEXT_SIZE];    // chuan kou yao fa song de yi tiao wen ben
} UartQueueMessage;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

uint32_t control_task_count = 0;
uint32_t uart_task_count = 0;
uint32_t status_task_count = 0;
															
osMessageQueueId_t uart_queueHandle;    // chuan kou xiao xi dui lie ju bing

const osMessageQueueAttr_t uart_queue_attributes = {
  .name = "uart_queue"                  // dui lie ming zi, zhu yao yong yu tiao shi
};

osThreadId_t UartPrintTaskHandle;       // chuan kou da yin ren wu ju bing

const osThreadAttr_t UartPrintTask_attributes = {
  .name = "UartPrintTask",              // ren wu ming zi
  .stack_size = 512 * 4,                // ren wu zhan da xiao
  .priority = (osPriority_t) osPriorityBelowNormal,  // ren wu you xian ji
};


/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ControlTask */
osThreadId_t ControlTaskHandle;
const osThreadAttr_t ControlTask_attributes = {
  .name = "ControlTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for UartTask */
osThreadId_t UartTaskHandle;
const osThreadAttr_t UartTask_attributes = {
  .name = "UartTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for StatusTask */
osThreadId_t StatusTaskHandle;
const osThreadAttr_t StatusTask_attributes = {
  .name = "StatusTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void StartUartPrintTask(void *argument);        // chuan kou da yin ren wu han shu

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartControlTask(void *argument);
void StartUartTask(void *argument);
void StartStatusTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
//	uart_mutexHandle = osMutexNew(&uart_mutex_attributes);    // chuang jian chuan kou hu chi suo
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	uart_queueHandle = osMessageQueueNew(UART_QUEUE_LENGTH,
                                     sizeof(UartQueueMessage),
                                     &uart_queue_attributes);    // chuang jian chuan kou xiao xi dui lie
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of ControlTask */
  ControlTaskHandle = osThreadNew(StartControlTask, NULL, &ControlTask_attributes);

  /* creation of UartTask */
  UartTaskHandle = osThreadNew(StartUartTask, NULL, &UartTask_attributes);

  /* creation of StatusTask */
  StatusTaskHandle = osThreadNew(StartStatusTask, NULL, &StatusTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
	UartPrintTaskHandle = osThreadNew(StartUartPrintTask,
                                  NULL,
                                  &UartPrintTask_attributes);    // chuang jian chuan kou da yin ren wu																	 
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartControlTask */
/**
* @brief Function implementing the ControlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartControlTask */
void StartControlTask(void *argument) // kong zhi suan fa
{
  /* USER CODE BEGIN StartControlTask */
  /* Infinite loop */
	HAL_StatusTypeDef ret;    // baocun MPU6050 duqu jieguo
  float dt = 0.01f;         // 10ms = 0.01s
  for(;;)
  {
		control_task_count++;   // ControlTask yunxing cishu +1

    ret = mpu6050_read_raw(&raw);    // duqu MPU6050 yuan shi shu ju

    if (ret == HAL_OK)               // duqu chenggong
    {
      attitude_update(&raw, &attitude, dt);    // gengxin zitai jiao

      if (flight_state == FLIGHT_ARMED)        // zhiyou jiesuo hou cai zuo baohu
      {
        if (HAL_GetTick() - last_cmd_tick >= 5000)
        {
          flight_state = FLIGHT_LOCKED;        // 5s mei mingling, zidong suoding
          motor_lock_output(&motor1, &motor2, &motor3, &motor4);

          uart_queue_send_string("CMD_TIMEOUT_LOCKED\r\n");

        }

        if (attitude_is_safe(&attitude) == 0)
        {
          flight_state = FLIGHT_ERROR;         // jiaodu guo da, jinru ERROR
          motor_lock_output(&motor1, &motor2, &motor3, &motor4);

          uart_queue_send_string("ERROR=ANGLE_LIMIT\r\n");

        }
      }

      flight_control_update(flight_state,
                            &attitude,
                            dt,
                            base_pwm,
                            &roll_pid_output,
                            &pitch_pid_output,
                            &motor1,
                            &motor2,
                            &motor3,
                            &motor4);
    }
    else
    {
      if (flight_state != FLIGHT_ERROR)
      {

        uart_queue_send_string("ERROR=MPU_READ\r\n");

      }

      flight_state = FLIGHT_ERROR;             // MPU6050 duqu shibai
      motor_lock_output(&motor1, &motor2, &motor3, &motor4);
    }

    osDelay(10);    // 10ms yunxing yi ci kongzhi renwu
  }
  /* USER CODE END StartControlTask */
}

/* USER CODE BEGIN Header_StartUartTask */
/**
* @brief Function implementing the UartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUartTask */
void StartUartTask(void *argument)      // chuan kou
{
  /* USER CODE BEGIN StartUartTask */
  /* Infinite loop */
  for(;;)
  {
		uart_task_count++;

		app_command_check(&base_pwm, &last_cmd_tick, &attitude);

    osDelay(1);
  }
  /* USER CODE END StartUartTask */
}

/* USER CODE BEGIN Header_StartStatusTask */
/**
* @brief Function implementing the StatusTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartStatusTask */
void StartStatusTask(void *argument)
{
  /* USER CODE BEGIN StartStatusTask */
  /* Infinite loop */
	uint32_t led_interval = 500;    // LED shan shuo zhou qi
	uint32_t status_print_tick = HAL_GetTick();   // da yin zhou qi shi jian
  uint32_t led_tick = HAL_GetTick();            // LED zhou qi shi jian
  uint32_t status_print_interval = 100;          // 100ms da yin yi ci      
  for(;;)
  {
  uint32_t now_ms = HAL_GetTick();

    status_task_count++;    // StatusTask yunxing cishu +1

    if (now_ms - status_print_tick >= status_print_interval)
    {
      status_print_tick = now_ms;

      char status_msg[UART_QUEUE_TEXT_SIZE];     // bao cun yi zheng hang zhuang tai wen ben
const char *state_text = "UNKNOWN";        // zhuang tai wen ben mo ren zhi

if (flight_state == FLIGHT_LOCKED)
{
  state_text = "LOCKED";                   // suoding zhuang tai
}
else if (flight_state == FLIGHT_ARMED)
{
  state_text = "ARMED";                    // jiesuo zhuang tai
}
else if (flight_state == FLIGHT_ERROR)
{
  state_text = "ERROR";                    // cuowu zhuang tai
}

snprintf(status_msg,
         sizeof(status_msg),
         "STATE=%s, ROLL_X100=%ld, PITCH_X100=%ld, ROLL_PID=%ld, PITCH_PID=%ld, M1=%ld, M2=%ld, M3=%ld, M4=%ld\r\n",
         state_text,
         (long)(attitude.fused_roll * 100.0f),
         (long)(attitude.fused_pitch * 100.0f),
         (long)roll_pid_output,
         (long)pitch_pid_output,
         (long)motor1,
         (long)motor2,
         (long)motor3,
         (long)motor4);                    // ba duo ge shu ju zu he cheng yi hang wen ben

uart_queue_send_string(status_msg);        // ba wen ben fang ru chuan kou dui lie
    }

    if (flight_state == FLIGHT_ARMED)
    {
      led_interval = 100;   // jiesuo kuai shan
    }
    else if (flight_state == FLIGHT_ERROR)
    {
      led_interval = 50;    // cuowu geng kuai shan
    }
    else
    {
      led_interval = 500;   // suoding man shan
    }

    if (now_ms - led_tick >= led_interval)
    {
      led_tick = now_ms;
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);    // fan zhuan LED
    }

    osDelay(1);   // rang chu liqi you kong qi
	}
}
  /* USER CODE END StartStatusTask */


/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void uart_queue_send_string(const char *str)
{
  UartQueueMessage msg;    // chuang jian yi ge dui lie xiao xi
  uint32_t i = 0;          // zi fu xia biao

  if (str == NULL)         // ru guo chuan ru kong zhi zhen, zhi jie fan hui
  {
    return;
  }

  while ((str[i] != '\0') && (i < (UART_QUEUE_TEXT_SIZE - 1U)))
  {
    msg.text[i] = str[i];  // yi ge zi fu yi ge zi fu kao bei
    i++;
  }

  msg.text[i] = '\0';      // shou dong bu shang zi fu chuan jie shu fu

  if (uart_queueHandle != NULL)    // dui lie yi jing chuang jian cai fa song
  {
    (void)osMessageQueuePut(uart_queueHandle, &msg, 0U, 0U);    // ba xiao xi fang ru dui lie
  }
}

void StartUartPrintTask(void *argument)
{
  UartQueueMessage msg;    // yong lai jie shou dui lie li de xiao xi

  for(;;)
  {
    if (uart_queueHandle == NULL)    // dui lie mei chuang jian cheng gong
    {
      osDelay(100);                  // deng yi xia zai jian cha
    }
    else if (osMessageQueueGet(uart_queueHandle, &msg, NULL, osWaitForever) == osOK)
    {
      uart1_send_string(msg.text);   // cong dui lie qu dao xiao xi hou, tong yi fa song
    }
  }
}

/* USER CODE END Application */

