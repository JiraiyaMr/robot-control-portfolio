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

//void can_comm ()
//{
//	
//}

////can发送
//void CAN_cmd_send(uint32_t StdId, int16_t mocalun1, int16_t mocalun2, int16_t shoot, int16_t rev)
//{
//	  CAN_TxHeaderTypeDef *can_tx_messag;
//    uint32_t send_mail_box;
//	  uint8_t  gimbal_fric_can_send_data[8];
//    can_tx_messag->StdId = StdId;
//    can_tx_messag->IDE = CAN_ID_STD;
//    can_tx_messag->RTR = CAN_RTR_DATA;
//    can_tx_messag->DLC = 0x08;
////    gimbal_fric_can_send_data[0] = (mocalun1 >> 8);
////    gimbal_fric_can_send_data[1] = mocalun1;
////    gimbal_fric_can_send_data[2] = (mocalun2 >> 8);
////    gimbal_fric_can_send_data[3] = mocalun2;
////    gimbal_fric_can_send_data[4] = (shoot >> 8);
////    gimbal_fric_can_send_data[5] = shoot;
////    gimbal_fric_can_send_data[6] = (rev >> 8);
////    gimbal_fric_can_send_data[7] = rev;
//    HAL_CAN_AddTxMessage(&hcan1, &can_tx_messag, gimbal_fric_can_send_data, &send_mail_box);
//}
