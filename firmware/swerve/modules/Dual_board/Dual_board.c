#include "Dual_board.h"

Robot_chassis_t can_send_chassis;
Robot_chassis_t can_receive_chassis;

const motor_measure_t *dual_board;

Subscriber_t *can_send_sub;
Publisher_t *can_receive_pub;

void can_send_task (void const * argument)
{
	can_send_sub = SubRegister("chassis_cmd",sizeof(Robot_chassis_t));
	for(;;)
	{
		SubGetMessage(can_send_sub,&can_send_chassis);
		CAN_cmd_dual_board(can_send_chassis.Vx_set,can_send_chassis.Vy_set,can_send_chassis.Wz_set,can_send_chassis.chassis_control);
		osDelay(2);
	}
}

void can_receive_task (void const * argument)
{
	can_receive_pub = PubRegister("chassis_cmd",sizeof(Robot_chassis_t));
	for(;;)
	{
		dual_board = get_dual_board_measure_point();
		can_receive_chassis.Vx_set = dual_board->ecd;
		can_receive_chassis.Vy_set = dual_board->speed_rpm;
		can_receive_chassis.Wz_set = dual_board->given_current;
		can_receive_chassis.chassis_control = dual_board->temperate;//由于时间紧迫，参数名称没有更改，以后有时间再改
		PubPushMessage(can_receive_pub,(void *)&can_receive_chassis);
		osDelay(2);
	}
}