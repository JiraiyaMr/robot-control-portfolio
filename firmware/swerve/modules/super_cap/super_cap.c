#include "super_cap.h"

Super_cap_t Super_Cap;

void supercap_task (void const * argument)
{
	SP_init(&Super_Cap);
	for(;;)
	{
		
		CAN_cmd_supercap(6000);
		osDelay(100);
	}
}

void SP_init (Super_cap_t *Super_Cap)
{
	Super_Cap->Super_cap_can = get_super_cup_measure_point();
}
