#ifndef VOFA_H
#define VOFA_H

#include "PID.h"

typedef enum
{
	mode		= 0,
	Kp   		= 1,
	Ki   		= 2,
	Kd   		= 3,
	max_iout 	= 4,
	max_out 	= 5,
} PID_Element_Enum;

float Get_Data(void);
void UART1_Init(void);
void USART1_Update(void);
	
#endif
