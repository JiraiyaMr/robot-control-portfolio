#include "gimbal.h"
#include "math.h"
#include "remote_control.h"
#include "pid.h"
#define pi 3.1415926535

gimbal_move_t gimbal_t;
gimbal_feedback gimbal_fb;
Robot_gimbal_t gimbal;

Publisher_t *gimbal_feedback_to_robots;
Subscriber_t *gimbal_cmd_from_robots;
int gim_before_ctrl_mode = ctrl_err;
//int gim_before_pid_mode = PID_MODE_Err;
static double target_angle =0;

void gimbal_task(void *arg)
{
	Gimbal_Init();
	for(;;)
	{
		SubGetMessage(gimbal_cmd_from_robots,&gimbal);
		Gimbal_RC_Ctrl();
		PubPushMessage(gimbal_feedback_to_robots,(void *)&gimbal_fb);
		osDelay(2);
	}
}

//void Gimbal_Ctrl(void)
//{
////	switch(chassis_t.ctrl_mode)
////	{
////		case(RC_CTRL_MODE):RC_Ctrl();break;
////		case(KEY_CTRL_MODE):KEY_Ctrl();break;
////	}
//	gimbal_t.Is_gimbal_spin = gim_NO;
//	Gimbal_RC_Ctrl();
//}

//云台初始化
void Gimbal_Init(void)
{
	gimbal_feedback_to_robots = PubRegister("gimbal_feedback",sizeof(gimbal_feedback));
	gimbal_cmd_from_robots = SubRegister("gimbal_cmd",sizeof(Robot_gimbal_t));
}

void Gimbal_RC_Ctrl (void)
{
//	gimbal_t.gimbal_RC = get_remote_control_point();
//	gimbal_t.gimbal_INS = get_gimbal_ins_measure_point();
	switch(gimbal.gimbal_control)
	{
		case(RC_SPIN_control):                //小陀螺模式
		{
			if(gim_before_ctrl_mode!= gimbal.gimbal_control)
			{
				gim_before_ctrl_mode =  gimbal.gimbal_control;
//				gimbal_t.pid_mode = RC_GYRO_PID;
//			  set_gimbal_pid_parameter(&gimbal_t);
//				gimbal_t.Gimbal_INS_MID = gimbal_t.gimbal_INS->yaw/100;
//				gimbal_t.yaw_target_angle = 0;
//				Reset_infomaiton(&chassis_t);
			}
//			gimbal_t.gimbal_INS = get_gimbal_ins_measure_point();
			  get_gimbal_motor_data(&gimbal_t);
			  gimbal_fb.yaw_angle = gimbal_t.gim_yaw_move_t.angle;
			  gimbal_fb.pitch_angle = gimbal_t.gim_pitch_move_t.angle;
			  gimbal_t.yaw_now_angle = gimbal_t.gim_yaw_move_t.angle;
			  gimbal_t.yaw_target_angle = gimbal_t.gim_yaw_move_t.angle + (gimbal.yaw_target_angle - gimbal.yaw_now_angle)*8192/2/pi;
			  gimbal_t.yaw_now_angle = Handle_Angle8191_PID_Over_Zero(gimbal_t.yaw_target_angle,gimbal_t.yaw_now_angle);
			  count_gim_output(&gimbal_t);
//		  clear_pidout(&chassis_t);
			break;
		}
	  case(RC_GYRO_control):               //陀螺仪模式
		{
			if(gim_before_ctrl_mode!= gimbal.gimbal_control)
			{
				gim_before_ctrl_mode =  gimbal.gimbal_control;
//				gimbal_t.pid_mode = RC_MECH_PID;
//				set_gimbal_pid_parameter(&gimbal_t);
//				gimbal_t.Gimbal_INS_MID = gimbal_t.gimbal_INS->Yaw;
//				Reset_infomaiton(&chassis_t);
			}
			get_gimbal_motor_data(&gimbal_t);
			gimbal_fb.yaw_angle = gimbal_t.gim_yaw_move_t.angle;
			gimbal_fb.pitch_angle = gimbal_t.gim_pitch_move_t.angle;
      gimbal_t.yaw_now_angle = gimbal_t.gim_yaw_move_t.angle;
			gimbal_t.yaw_target_angle = gimbal_t.gim_yaw_move_t.angle + (gimbal.yaw_target_angle - gimbal.yaw_now_angle)*8192/2/pi;
			gimbal_t.yaw_now_angle = Handle_Angle8191_PID_Over_Zero(gimbal_t.yaw_target_angle,gimbal_t.yaw_now_angle);
			count_gim_output(&gimbal_t);
//		  clear_pidout(&chassis_t);
			break;
		}
	}
}

//得到云台电机数据
void get_gimbal_motor_data (gimbal_move_t *gimbal)
{
		gimbal->gim_yaw_move_t.motor_data = get_yaw_gimbal_motor_measure_point();
	  gimbal->gim_yaw_move_t.angle = gimbal->gim_yaw_move_t.motor_data->ecd;
	  gimbal->gim_yaw_move_t.speed = gimbal->gim_yaw_move_t.motor_data->speed_rpm;
	  
	  gimbal->gim_pitch_move_t.motor_data = get_pitch_gimbal_motor_measure_point();
	  gimbal->gim_pitch_move_t.angle = gimbal->gim_pitch_move_t.motor_data->ecd;
	  gimbal->gim_pitch_move_t.speed = gimbal->gim_pitch_move_t.motor_data->speed_rpm;
}

double msp(double x, double in_min, double in_max, double out_min, double out_max)
{
	return (x-in_min)*(out_max-out_min)/(in_max-in_min)+out_min;
}

//double angle_solve(double angle)
//{
//	if(angle>=360)
//	{
//		angle-=360;
//	}
//	else if (angle<=-360)
//		angle +=360;
//	return angle;
//}

//double gimbal_angle_solving(double x, double middle)
//{
//	double code;
//	double target;
//	code = x/pi*4096;
//	target = code+middle;
//	if(target>8191)
//		target = target-8191;
//	else if (target<0)
//		target = target+8191;
//	return target;
//}

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

///**
//  * @brief          
//  * @param[in]      none
//  */
//int32_t  Pch_To_Speed(int32_t Channel,int32_t Speed_Max)
//{
//	int32_t Speed=0;
//	Speed=( Channel+660) * (Speed_Max +Speed_Max) / (660 +660) -Speed_Max;
//	return Speed;
//}

////设置云台电机PID
//void set_gimbal_pid_parameter (gimbal_move_t *gimbal)
//{
//	switch(gimbal->pid_mode)
//	{
//		case(RC_MECH_PID):
//					pid_init(&gimbal->gimbal_yaw_angle_pid, 3, 0, 0, 0, 400);
//					pid_init(&gimbal->gimbal_yaw_speed_pid, 18, 0, 0, 800, 12000);
//		      pid_init(&gimbal->gimbal_pitch_angle_pid, 2, 0, 0, 0, 400);
//					pid_init(&gimbal->gimbal_pitch_speed_pid, 10, 0, 0, 800, 12000);
//					break;
//		case(RC_SPIN_PID):
//					pid_init(&gimbal->gimbal_yaw_angle_pid, 3, 0, 0, 0, 400);
//					pid_init(&gimbal->gimbal_yaw_speed_pid, 18, 0, 0, 800, 12000);
//		      pid_init(&gimbal->gimbal_pitch_angle_pid, 2, 0, 0, 0, 400);
//					pid_init(&gimbal->gimbal_pitch_speed_pid, 10, 0, 0, 800, 12000);
//		      break;
//		case(RC_GYRO_PID):
//				  pid_init(&gimbal->gimbal_yaw_angle_pid, 3, 0, 0, 0, 400);
//					pid_init(&gimbal->gimbal_yaw_speed_pid, 18, 0, 0, 800, 12000);
//		      pid_init(&gimbal->gimbal_pitch_angle_pid, 2, 0, 0, 0, 400);
//					pid_init(&gimbal->gimbal_pitch_speed_pid, 10, 0, 0, 800, 12000);
//					break;
//	}
//}

//计算云台电机串级PID输出
void count_gim_output(gimbal_move_t *gimbal)
{
	gimbal->gim_yaw_move_t.speed_set = pid_calc(&gimbal->gimbal_yaw_angle_pid,gimbal->yaw_target_angle,gimbal->yaw_now_angle);
	gimbal->gim_yaw_move_t.give_current = pid_calc(&gimbal->gimbal_yaw_speed_pid,gimbal->gim_yaw_move_t.speed_set,gimbal->gim_yaw_move_t.speed);
	
	gimbal->gim_pitch_move_t.speed_set = pid_calc(&gimbal->gimbal_pitch_speed_pid,gimbal->pitch_target_angle,gimbal->gim_pitch_move_t.angle);
	gimbal->gim_pitch_move_t.give_current = pid_calc(&gimbal->gimbal_pitch_speed_pid,gimbal->gim_pitch_move_t.speed_set,gimbal->gim_pitch_move_t.speed);
	
	CAN_cmd_gimbal(gimbal->gim_yaw_move_t.give_current,gimbal->gim_pitch_move_t.give_current,0,0);
}

//void clear_pidout (chassis_move_t *chassis)
//{
//	int i;
//	for(i=0;i<4;i++)
//	{
//		chassis->chassis_speed_pid[i].d_out = 0;
//		chassis->chassis_speed_pid[i].i_out = 0;
//		chassis->chassis_speed_pid[i].p_out = 0;
//		chassis->chassis_speed_pid[i].output = 0;
//		chassis->chassis_speed_pid[i].err[0] =0;
//		chassis->chassis_speed_pid[i].err[1] =0;
//		chassis->chassis_speed_pid[i].ref = 0;
//	}
//}

////*************************************小陀螺***********************************//
//void Get_inception_infomation(chassis_move_t *chassis)
//{
//	
//}

//void Initialize_spin_after_init (chassis_move_t *chassis)
//{
//	chassis->Vx_set = 0;
//	chassis->Vy_set = 0;
//	chassis->Wz_set = 5000;
//}

//void Output_optimization (chassis_move_t *chassis_motor_t)
//{
//	
//}

//void Reset_infomaiton (gimbal_motor_t *gimbal)
//{
//	
//}
