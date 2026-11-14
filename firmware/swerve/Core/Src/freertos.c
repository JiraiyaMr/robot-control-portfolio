/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "chassis.h"
#include "robots.h"
#include "gimbal.h"
//#include "ins.h"
#include "super_cap.h"
#include "Dual_board.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
#define CHASSIS_TASK_PRIO						2		// 任务优先级
#define CHASSIS_STK_SIZE						256		// 任务堆栈大小
TaskHandle_t Chassis_Task_Handler;						// 任务句柄

#define HELM_TASK_PRIO						2		// 任务优先级
#define HELM_STK_SIZE						256		// 任务堆栈大小
TaskHandle_t Helm_Task_Handler;						// 任务句柄

#define GIMBAL_TASK_PRIO						2		// 任务优先级
#define GIMBAL_STK_SIZE						256		// 任务堆栈大小
TaskHandle_t Gimbal_Task_Handler;						// 任务句柄

#define ROBOTS_TASK_PRIO						2		// 任务优先级
#define ROBOTS_STK_SIZE						256		// 任务堆栈大小
TaskHandle_t Robots_Task_Handler;						// 任务句柄

#define INS_TASK_PRIO						2		// 任务优先级
#define INS_STK_SIZE						256		// 任务堆栈大小
TaskHandle_t Ins_Task_Handler;						// 任务句柄

#define SUPERCAP_TASK_PRIO						2		// 任务优先级
#define SUPERCAP_STK_SIZE						256		// 任务堆栈大小
TaskHandle_t SuperCap_Task_Handler;						// 任务句柄

#define DUALBOARD_TASK_PRIO						3		// 任务优先级
#define DUALBOARD_STK_SIZE						256		// 任务堆栈大小
TaskHandle_t DualBoard_Task_Handler;						// 任务句柄
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
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
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
	xTaskCreate((TaskFunction_t		)chassis_task,								// 任务函数
							(const char*		)"chassis_task",				// 任务名称
							(uint16_t			)CHASSIS_STK_SIZE,				// 任务堆栈大小
							(void*				)NULL,						// 传递给任务函数的参数
							(UBaseType_t		)CHASSIS_TASK_PRIO,			// 任务优先级
							(TaskHandle_t*		)&Chassis_Task_Handler);		// 任务句柄
//							
	xTaskCreate((TaskFunction_t		)helm_task,								// 任务函数
							(const char*		)"helm_task",				// 任务名称
							(uint16_t			)HELM_STK_SIZE,				// 任务堆栈大小
							(void*				)NULL,						// 传递给任务函数的参数
							(UBaseType_t		)HELM_TASK_PRIO,			// 任务优先级
							(TaskHandle_t*		)&Helm_Task_Handler);		// 任务句柄
							
	xTaskCreate((TaskFunction_t		)can_receive_task,								// 任务函数
							(const char*		)"can_receive_task",				// 任务名称
							(uint16_t			)DUALBOARD_STK_SIZE,				// 任务堆栈大小
							(void*				)NULL,						// 传递给任务函数的参数
							(UBaseType_t		)DUALBOARD_TASK_PRIO,			// 任务优先级
							(TaskHandle_t*		)&DualBoard_Task_Handler);		// 任务句柄
							
//	xTaskCreate((TaskFunction_t		)gimbal_task,								// 任务函数
//							(const char*		)"gimbal_task",				// 任务名称
//							(uint16_t			)GIMBAL_STK_SIZE,				// 任务堆栈大小
//							(void*				)NULL,						// 传递给任务函数的参数
//							(UBaseType_t		)GIMBAL_TASK_PRIO,			// 任务优先级
//							(TaskHandle_t*		)&Gimbal_Task_Handler);		// 任务句柄
							
//	xTaskCreate((TaskFunction_t		)robot_task,								// 任务函数
//							(const char*		)"robot_task",				// 任务名称
//							(uint16_t			)ROBOTS_STK_SIZE,				// 任务堆栈大小
//							(void*				)NULL,						// 传递给任务函数的参数
//							(UBaseType_t		)ROBOTS_TASK_PRIO,			// 任务优先级
//							(TaskHandle_t*		)&Robots_Task_Handler);		// 任务句柄
	
//	xTaskCreate((TaskFunction_t		)ins_task,								// 任务函数
//							(const char*		)"ins_task",				// 任务名称
//							(uint16_t			)INS_STK_SIZE,				// 任务堆栈大小
//							(void*				)NULL,						// 传递给任务函数的参数
//							(UBaseType_t		)INS_TASK_PRIO,			// 任务优先级
//							(TaskHandle_t*		)&Ins_Task_Handler);		// 任务句柄
							
//	xTaskCreate((TaskFunction_t		)supercap_task,								// 任务函数
//							(const char*		)"supercap_task",				// 任务名称
//							(uint16_t			)SUPERCAP_STK_SIZE,				// 任务堆栈大小
//							(void*				)NULL,						// 传递给任务函数的参数
//							(UBaseType_t		)SUPERCAP_TASK_PRIO,			// 任务优先级
//							(TaskHandle_t*		)&SuperCap_Task_Handler);		// 任务句柄
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
