#ifndef _SUPER_CAP_H
#define _SUPER_CAP_H

#include "stm32f4xx.h"                  // Device header
#include "CAN_receive.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct
{
	const super_cap_t *Super_cap_can;
}Super_cap_t;

void supercap_task (void const * argument);

void SP_init (Super_cap_t *Super_Cap);

#endif

