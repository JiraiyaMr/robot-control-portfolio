#ifndef _DUAL_BOARD_H
#define _DUAL_BOARD_H

#include "stm32f4xx.h"                  // Device header
#include "CAN_receive.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "robots.h"
#include "usart.h"
void can_send_task (void const * argument);
void can_receive_task (void const * argument);

#endif

