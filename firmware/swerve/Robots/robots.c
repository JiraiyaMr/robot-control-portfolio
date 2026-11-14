#include "robots.h"

#define pi 3.1425926

Robot_Status Robot_Status_t;
Robot_chassis_t chassis_cmd_t;
Robot_gimbal_t gimbal_cmd_t;
Robot_shoot_t shoot_cmd_t;

gimbal_feedback gimbai_fb_from_gimbal;
//IMU_feedback ins_fb_form_ins;

//底盘信息发布和订阅的结构体
Publisher_t *chassis_cmd_pub;   
Subscriber_t *chassis_feed_sub; 

//云台信息发布和订阅的结构体
Publisher_t *gimbal_cmd_pub;   
Subscriber_t *gimbal_feed_sub; 

//发射信息发布和订阅的结构体
Publisher_t *shoot_cmd_pub;   
Subscriber_t *shoot_feed_sub; 

//陀螺仪信息订阅的结构体
Subscriber_t *ins_feed_sub;

/* --------------------------任务层------------------------- */
void robot_task(void const * argument)
{
	Robot_Init(&Robot_Status_t);
	gimbal_cmd_t.yaw_motor_mid = 674;
	for(;;)
	{
		
		SubGetMessage(gimbal_feed_sub,&gimbai_fb_from_gimbal);
//		SubGetMessage(ins_feed_sub,&ins_fb_form_ins);
		ROBOT_CTRL(&Robot_Status_t);
		PubPushMessage(chassis_cmd_pub,(void *)&chassis_cmd_t);
		PubPushMessage(gimbal_cmd_pub,(void *)&gimbal_cmd_t);
		osDelay(2);
	}
}

/* --------------------------控制层------------------------- */
//机器人命令初始化
void Robot_Init(Robot_Status *RS_t)
{
//	#if  IS_TWO_BOARD
//	    #ifdef  MASTER_OR_SLAVER
							chassis_cmd_pub = PubRegister("chassis_cmd",sizeof(Robot_chassis_t));
							gimbal_cmd_pub = PubRegister("gimbal_cmd",sizeof(Robot_gimbal_t));
	            gimbal_feed_sub = SubRegister("gimbal_feedback",sizeof(gimbal_feedback));
//	            ins_feed_sub = SubRegister("ins_feedback",sizeof(gimbal_feedback));
//	    #endif
//						
//	#endif
	
}
void ROBOT_CTRL (Robot_Status *RS_t)
{
	RS_t->Robot_RC = get_remote_control_point();
	Control_Mode_Handler(&Robot_Status_t);
	Chaais_Message_Handler(&Robot_Status_t,&chassis_cmd_t);
	Gimbal_Message_Handler(&Robot_Status_t,&gimbal_cmd_t);
}

/* --------------------------功能层------------------------- */
//遥控器信息处理，得到控制模式
void Control_Mode_Handler(Robot_Status *RS_t)
{
	//
	switch(RS_t->Robot_RC->S1)
	{
		case 1:
		{
			RS_t->Robot_Control_mode = RC_MECH_control;
		  break;
		}
	  case 2:
		{
			RS_t->Robot_Control_mode = RC_GYRO_control;
		  break;
		}
		case 3:
		{
			RS_t->Robot_Control_mode = RC_SPIN_control;
		  break;
		}
	}
}

//遥控器信息处理，得到底盘运动信息
void Chaais_Message_Handler(Robot_Status *RS_t,Robot_chassis_t *chassis_t)
{
	if(RS_t->Robot_Control_mode == RC_MECH_control)
	{
		chassis_t->chassis_control = RC_MECH_control;
		chassis_t->Vx_set = RC_to_Chassis(RS_t->Robot_RC->RC_CH1,20);
		chassis_t->Vy_set = RC_to_Chassis(RS_t->Robot_RC->RC_CH0,20);
		chassis_t->Wz_set = RC_to_Chassis(RS_t->Robot_RC->RC_CH2,8);
	}
	else if(RS_t->Robot_Control_mode == RC_GYRO_control)
	{
		chassis_t->chassis_control = RC_GYRO_control;
		chassis_t->Vx_set = RC_to_Chassis(RS_t->Robot_RC->RC_CH1,20);
		chassis_t->Vy_set = RC_to_Chassis(RS_t->Robot_RC->RC_CH0,20);
		chassis_t->Wz_set = offsetAngle()*40;//将底盘与云台的绝对角度差转换为底盘的z轴速度，具体系数有待确定
	}
	else if(RS_t->Robot_Control_mode == RC_SPIN_control)
	{
		chassis_t->chassis_control = RC_SPIN_control;
		chassis_t->Vx_set = RS_t->Robot_RC->RC_CH1 * cos(offsetAngle()) - RS_t->Robot_RC->RC_CH0 * sin(offsetAngle());
		chassis_t->Vy_set = RS_t->Robot_RC->RC_CH1 * sin(offsetAngle()) + RS_t->Robot_RC->RC_CH0 * cos(offsetAngle());
		chassis_t->Wz_set = 10;
	}
}

//遥控器信息处理，得到云台控制信息
void Gimbal_Message_Handler(Robot_Status *RS_t,Robot_gimbal_t *gimbal_t)
{
	if(RS_t->Robot_Control_mode == RC_MECH_control)
	{
		gimbal_t->gimbal_control = RC_MECH_control;
	}
	else if(RS_t->Robot_Control_mode == RC_GYRO_control)
	{
		gimbal_t->gimbal_control = RC_GYRO_control;
		gimbal_t->pitch_target_angle += 0.0001f*RS_t->Robot_RC->RC_CH2;
		gimbal_t->pitch_target_angle = Pitch_Angle(gimbal_t->pitch_target_angle);
		gimbal_t->yaw_target_angle += 0.0004f*RS_t->Robot_RC->RC_CH3;
		gimbal_t->yaw_target_angle = Yaw_Angle(gimbal_t->yaw_target_angle);
//		gimbal_t->pitch_now_angle = ins_fb_form_ins.Pitch;
//		gimbal_t->yaw_now_angle = ins_fb_form_ins.YawTotalAngle;
	}
	else if(RS_t->Robot_Control_mode == RC_SPIN_control)
	{
		gimbal_t->gimbal_control = RC_SPIN_control;
		gimbal_t->pitch_target_angle += 0.0001f*RS_t->Robot_RC->RC_CH2;
		gimbal_t->pitch_target_angle = Pitch_Angle(gimbal_t->pitch_target_angle);
		gimbal_t->yaw_target_angle += 0.0004f*RS_t->Robot_RC->RC_CH3;
		gimbal_t->yaw_target_angle = Yaw_Angle(gimbal_t->yaw_target_angle);
//		gimbal_t->pitch_now_angle = ins_fb_form_ins.Pitch;
//		gimbal_t->yaw_now_angle = ins_fb_form_ins.YawTotalAngle;
	}
}

//键盘鼠标控制时的信息处理
void Key_Message_Handler (Robot_Status *RS_t,Robot_chassis_t *chassis_t)
{
	if(RS_t->Robot_KEY->key[Key_B])
	{
		Magazine(&shoot_cmd_t);//关闭弹仓
	}
//	if(
	chassis_t->Vx_set = (RS_t->Robot_KEY->key[Key_W] - RS_t->Robot_KEY->key[Key_S])*30;
	chassis_t->Vy_set = (RS_t->Robot_KEY->key[Key_D] - RS_t->Robot_KEY->key[Key_A])*30;
	chassis_t->Wz_set = offsetAngle()*400;
}

//开启弹仓或关闭弹仓
void Magazine (Robot_shoot_t *shoot_cmd_t)
{
	if (shoot_cmd_t->mag_mode == CLOSE)
	    shoot_cmd_t->mag_mode = OPEN;
	else
		  shoot_cmd_t->mag_mode = CLOSE;
}

//将遥控器的数据转化为底盘速度数据
int16_t RC_to_Chassis (int16_t RC_data,int16_t XianFu)
{
	return XianFu*RC_data/660;
}

//计算底盘与云台的角度差（无需改动）
double offsetAngle (void)
{
	double Angle;
	int16_t diff;
	diff = gimbal_cmd_t.yaw_motor_mid - gimbai_fb_from_gimbal.yaw_angle;
	if(diff > 4096)
		diff = -(8192-diff);
	else if(diff < -4096)
		diff = 8192+diff;
	Angle = diff*pi/8192;
	return Angle;
}

////计算云台电机的目标值（过零点）
//uint16_t Handle_Angle8191_PID_Over_Zero(int16_t tar, uint16_t cur)
//{
//	if(tar - cur > 4096)   
//	{
//		return (cur+8191);        
//	}
//	else if(tar - cur < -4096)
//	{
//		return (cur-8191);
//	}
//	else
//	{
//		return cur;
//	}
//}

//pitch轴角度处理
double Pitch_Angle (double angle)
{
	if(angle>360.0f)
		{
			return angle-360;
		}
	else if (angle<-360.0f)
		return angle+360;
	else 
		return angle;
}

//yaw轴角度处理
double Yaw_Angle (double angle)
{
	if(angle>45.0f)
		{
			return 45;
		}
	else if (angle<-45.0f)
		return -45;
	else 
		return angle;
}
