/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Ultimate Liberty license
 * SLA0044, the "License"; You may not use this file except in compliance with
 * the License. You may obtain a copy of the License at:
 *                             www.st.com/SLA0044
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
#include "super_cap.h"
#include "Dual_board.h"
#include "ins_task.h"
#include "helm.h"
#include "shoot.h"
#include "pc_task.h"
#include "usb_task.h"
//#include "helm.h"
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
osThreadId INSTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE BEGIN FunctionPrototypes */
#define CHASSIS_TASK_PRIO						2		// �������ȼ�
#define CHASSIS_STK_SIZE						256		// �����ջ���?
TaskHandle_t Chassis_Task_Handler;						// ������

#define HELM_TASK_PRIO						2		// �������ȼ�
#define HELM_STK_SIZE						256		// �����ջ���?
TaskHandle_t Helm_Task_Handler;						// ������

#define GIMBAL_TASK_PRIO						2		// �������ȼ�
#define GIMBAL_STK_SIZE						512		// �����ջ���?
TaskHandle_t Gimbal_Task_Handler;						// ������

#define ROBOTS_TASK_PRIO						3		// �������ȼ�
#define ROBOTS_STK_SIZE						256		// �����ջ���?
TaskHandle_t Robots_Task_Handler;						// ������

#define INS_TASK_PRIO						2		// �������ȼ�
#define INS_STK_SIZE						256		// �����ջ���?
TaskHandle_t Ins_Task_Handler;						// ������

#define SUPERCAP_TASK_PRIO						2		// �������ȼ�
#define SUPERCAP_STK_SIZE						256		// �����ջ���?
TaskHandle_t SuperCap_Task_Handler;						// ������

#define DUALBOARD_TASK_PRIO						2		// �������ȼ�
#define DUALBOARD_STK_SIZE						256		// �����ջ���?
TaskHandle_t DualBoard_Task_Handler;						// ������

#define SHOOT_TASK_PRIO						2		// �������ȼ�
#define SHOOT_STK_SIZE						512		// �����ջ���?
TaskHandle_t Shoot_Task_Handler;						// ������

#define PC_TASK_PRIO						2		// �������ȼ�
#define PC_STK_SIZE						512		// �����ջ���?
TaskHandle_t Pc_Task_Handler;						// ������

#define USB_TASK_PRIO						2		// �������ȼ�
#define USB_STK_SIZE						512		// �����ջ���?
TaskHandle_t Usb_Task_Handler;						// ������

/* USER CODE END FunctionPrototypes */

void StartINSTask(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* GetTimerTaskMemory prototype (linked to static allocation support) */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/* USER CODE BEGIN GET_TIMER_TASK_MEMORY */
static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
    *ppxTimerTaskStackBuffer = &xTimerStack[0];
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
    /* place for user code */
}
/* USER CODE END GET_TIMER_TASK_MEMORY */

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
  /* definition and creation of INSTask */
  osThreadDef(INSTask, StartINSTask, osPriorityNormal, 0, 1024);
  INSTaskHandle = osThreadCreate(osThread(INSTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
// 	xTaskCreate((TaskFunction_t		)chassis_task,								// ������
//							(const char*		)"chassis_task",				// ��������
//							(uint16_t			)CHASSIS_STK_SIZE,				// �����ջ���?
//							(void*				)NULL,						// ���ݸ��������Ĳ���
//							(UBaseType_t		)CHASSIS_TASK_PRIO,			// �������ȼ�
//							(TaskHandle_t*		)&Chassis_Task_Handler);		// ������
//							
//	xTaskCreate((TaskFunction_t		)helm_task,								// ������
//							(const char*		)"helm_task",				// ��������
//							(uint16_t			)HELM_STK_SIZE,				// �����ջ���?
//							(void*				)NULL,						// ���ݸ��������Ĳ���
//							(UBaseType_t		)HELM_TASK_PRIO,			// �������ȼ�
//							(TaskHandle_t*		)&Helm_Task_Handler);		// ������
							
//	xTaskCreate((TaskFunction_t		)gimbal_task,								// ������
//							(const char*		)"gimbal_task",				// ��������
//							(uint16_t			)GIMBAL_STK_SIZE,				// �����ջ���?
//							(void*				)NULL,						// ���ݸ��������Ĳ���
//							(UBaseType_t		)GIMBAL_TASK_PRIO,			// �������ȼ�
//							(TaskHandle_t*		)&Gimbal_Task_Handler);		// ������
//							
//	xTaskCreate((TaskFunction_t		)shoot_task,								// ������
//							(const char*		)"shoot_task",				// ��������
//							(uint16_t			)SHOOT_STK_SIZE,				// �����ջ���?
//							(void*				)NULL,						// ���ݸ��������Ĳ���
//							(UBaseType_t		)SHOOT_TASK_PRIO,			// �������ȼ�
//							(TaskHandle_t*		)&Shoot_Task_Handler);		// ������
//							
//	xTaskCreate((TaskFunction_t		)can_send_task,								// ������
//							(const char*		)"can_send_task",				// ��������
//							(uint16_t			)DUALBOARD_STK_SIZE,				// �����ջ���?
//							(void*				)NULL,						// ���ݸ��������Ĳ���
//							(UBaseType_t		)DUALBOARD_TASK_PRIO,			// �������ȼ�
//							(TaskHandle_t*		)&DualBoard_Task_Handler);		// ������
//							
//	xTaskCreate((TaskFunction_t		)robot_task,								// ������
//							(const char*		)"robot_task",				// ��������
//							(uint16_t			)ROBOTS_STK_SIZE,				// �����ջ���?
//							(void*				)NULL,						// ���ݸ��������Ĳ���
//							(UBaseType_t		)ROBOTS_TASK_PRIO,			// �������ȼ�
//							(TaskHandle_t*		)&Robots_Task_Handler);		// ������
//	
//	xTaskCreate((TaskFunction_t		)StartINSTask,								// ������
//							(const char*		)"ins_task",				// ��������
//							(uint16_t			)INS_STK_SIZE,				// �����ջ���?
//							(void*				)NULL,						// ���ݸ��������Ĳ���
//							(UBaseType_t		)INS_TASK_PRIO,			// �������ȼ�
//							(TaskHandle_t*		)&Ins_Task_Handler);		// ������
							
	xTaskCreate((TaskFunction_t		)pc_task,								
							(const char*		)"pc_task",				
							(uint16_t			)PC_STK_SIZE,				
							(void*				)NULL,						
							(UBaseType_t		)PC_TASK_PRIO,			
							(TaskHandle_t*		)&Pc_Task_Handler);		
							
	xTaskCreate((TaskFunction_t		)usb_task,								
							(const char*		)"usb_task",				
							(uint16_t			)USB_STK_SIZE,				
							(void*				)NULL,						
							(UBaseType_t		)USB_TASK_PRIO,			
							(TaskHandle_t*		)&Usb_Task_Handler);		
//							
//	xTaskCreate((TaskFunction_t		)supercap_task,							
//							(const char*		)"supercap_task",				
//							(uint16_t			)SUPERCAP_STK_SIZE,				
//							(void*				)NULL,						
//							(UBaseType_t		)SUPERCAP_TASK_PRIO,			
//							(TaskHandle_t*		)&SuperCap_Task_Handler);		

  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartINSTask */
/**
 * @brief Function implementing the INSTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartINSTask */
void StartINSTask(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartINSTask */
    INS_Init();
    /* Infinite loop */
    for (;;)
    {
        INS_Task();
        osDelay(1);
    }
  /* USER CODE END StartINSTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
