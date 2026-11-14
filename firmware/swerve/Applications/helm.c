#include "helm.h"

#define pi 3.1415926535

int helm_before_ctrl_mode = ctrl_err;

Helm_move_t Helm_t;

Subscriber_t *helm_cmd_from_robots; 

Robot_chassis_t helm;

void helm_task (void const * argument)
{
	helm_Init();
	for(;;)
	{
//		chassis_Ctrl();
		SubGetMessage(helm_cmd_from_robots,&helm);
		HELM_Ctrl();
		osDelay(2);
	}
}

//底盘信息初始化
void helm_Init(void)
{
	int i;
	for(i=0;i<4;i++)
	{
		Helm_t.helm_move_t[i].angle = 0;
	}
	helm_cmd_from_robots = SubRegister("chassis_cmd",sizeof(Robot_chassis_t));
}

void HELM_Ctrl (void)
{
//	set_helm_pid_parameter(&chassis_t);
	switch(helm.chassis_control)
	{
		case(RC_MECH_control):
			if(helm_before_ctrl_mode!= helm.chassis_control)
			{
				helm_before_ctrl_mode = helm.chassis_control;
				Helm_t.pid_mode = RC_MECH_PID;
				Helm_t.helm_pid_mode[HELM_LF] = RC_MECH;
				Helm_t.helm_pid_mode[HELM_RF] = RC_MECH;
				Helm_t.helm_pid_mode[HELM_LB] = RC_MECH;
				Helm_t.helm_pid_mode[HELM_RB] = RC_MECH;
				set_helm_pid_parameter(&Helm_t);
//				Reset_infomaiton(&Helm_t);
			}
			set_helm_pid_parameter(&Helm_t);
			get_data_from_robots();
			get_helm_data(&Helm_t);
//			get_remote_data(&Helm_t,15000,5000);
//			resolving_speed(&chassis_t);
			resolving_angle(&Helm_t);
			Angle_adjust(&Helm_t);
			count_helm_output(&Helm_t);
//			clear_pidout(&chassis_t);
			break;
		case(RC_SPIN_control):
			if(helm_before_ctrl_mode!= helm.chassis_control)
			{
				helm_before_ctrl_mode = helm.chassis_control;
				Helm_t.pid_mode = RC_SPIN_PID;
				Helm_t.helm_pid_mode[HELM_LF] = RC_SPIN;
				Helm_t.helm_pid_mode[HELM_RF] = RC_SPIN;
				Helm_t.helm_pid_mode[HELM_LB] = RC_SPIN;
				Helm_t.helm_pid_mode[HELM_RB] = RC_SPIN;
				set_helm_pid_parameter(&Helm_t);
//				Reset_infomaiton(&Helm_t);
			}
			get_helm_data(&Helm_t);
			get_data_from_robots();
//			Initialize_spin_after_init(&Helm_t);
			resolving_angle(&Helm_t);
			Angle_adjust(&Helm_t);
			count_helm_output(&Helm_t);
			break;
		case(RC_GYRO_control):
			if(helm_before_ctrl_mode!= helm.chassis_control)
			{
				helm_before_ctrl_mode = helm.chassis_control;
				Helm_t.pid_mode = RC_SPIN_PID;
				Helm_t.helm_pid_mode[HELM_LF] = RC_GYRO;
				Helm_t.helm_pid_mode[HELM_RF] = RC_GYRO;
				Helm_t.helm_pid_mode[HELM_LB] = RC_GYRO;
				Helm_t.helm_pid_mode[HELM_RB] = RC_GYRO;
				set_helm_pid_parameter(&Helm_t);
//				Reset_infomaiton(&Helm_t);
			}
			get_helm_data(&Helm_t);
			get_data_from_robots();
//			Initialize_spin_after_init(&Helm_t);
			resolving_angle(&Helm_t);
			Angle_adjust(&Helm_t);
			count_helm_output(&Helm_t);
			break;
	}
}

void get_data_from_robots (void)
{
	Helm_t.Vx_set = helm.Vx_set;
	Helm_t.Vy_set = helm.Vy_set;
	Helm_t.Wz_set = helm.Wz_set;
}

void get_helm_data (Helm_move_t *helm)
{
	helm->helm_move_t[0].motor_data = get_helm_LF_measure_point();
	helm->helm_move_t[1].motor_data = get_helm_RF_measure_point();
	helm->helm_move_t[2].motor_data = get_helm_LB_measure_point();
	helm->helm_move_t[3].motor_data = get_helm_RB_measure_point();
	
	helm->helm_move_t[0].angle = helm->helm_move_t[0].motor_data->ecd;
	helm->helm_move_t[0].speed = helm->helm_move_t[0].motor_data->speed_rpm;
	helm->helm_move_t[1].angle = helm->helm_move_t[1].motor_data->ecd;
	helm->helm_move_t[1].speed = helm->helm_move_t[1].motor_data->speed_rpm;
	helm->helm_move_t[2].angle = helm->helm_move_t[2].motor_data->ecd;
	helm->helm_move_t[2].speed = helm->helm_move_t[2].motor_data->speed_rpm;
	helm->helm_move_t[3].angle = helm->helm_move_t[3].motor_data->ecd;
	helm->helm_move_t[3].speed = helm->helm_move_t[3].motor_data->speed_rpm;

}

void Angle_adjust(Helm_move_t *helm)
{
	helm->helm_move_t[0].angle = Handle_Angle8191_PID_Over_Zero (helm->helm_move_t[0].angle_set,helm->helm_move_t[0].angle);
	helm->helm_move_t[1].angle = Handle_Angle8191_PID_Over_Zero (helm->helm_move_t[1].angle_set,helm->helm_move_t[1].angle);
	helm->helm_move_t[2].angle = Handle_Angle8191_PID_Over_Zero (helm->helm_move_t[2].angle_set,helm->helm_move_t[2].angle);
	helm->helm_move_t[3].angle = Handle_Angle8191_PID_Over_Zero (helm->helm_move_t[3].angle_set,helm->helm_move_t[3].angle);
}

int16_t Handle_Angle8191_PID_Over_Zero(int16_t tar, uint16_t cur)
{
	if(tar - cur > 4096)   
	{
		return (cur+8191);        
	}
	else if(tar - cur < -4096)
	{
		return (cur-8191);
	}
	else
	{
		return cur;
	}
}

void resolving_angle(Helm_move_t *helm)
{
	float theta = atan(1.0/1.0);
	int32_t vx;
	int32_t vy;
	int32_t wz;
	
	int i;
	
	vx = helm->Vx_set;
	vy = helm->Vy_set;
	wz = helm->Wz_set;
	
	helm->helm_move_t[1].angle_set = atan2((vy-wz*sin(theta)),
											  (vx-wz*cos(theta)));
	helm->helm_move_t[0].angle_set = atan2((vy-wz*sin(theta)),
											  (vx+wz*cos(theta)));
	helm->helm_move_t[3].angle_set = atan2((vy+wz*sin(theta)),
											  (vx+wz*cos(theta)));
	helm->helm_move_t[2].angle_set = atan2((vy+wz*sin(theta)),
											  (vx-wz*cos(theta)));
	
	if(helm->Vx_set == 0 && helm->Vy_set == 0 && helm->Wz_set == 0)
	{
		helm->helm_move_t[1].angle_set = pi/4;
		helm->helm_move_t[0].angle_set = -pi/4;
		helm->helm_move_t[3].angle_set = pi/4;
		helm->helm_move_t[2].angle_set = -pi/4;
	}
	
	for(i=0;i<4;i++)
	{
		if(helm->helm_move_t[i].angle_set>pi/2)
		{
			helm->helm_move_t[i].angle_set-=pi;
		}
		else if(helm->helm_move_t[i].angle_set<-pi/2)
		{
			helm->helm_move_t[i].angle_set+=pi;
		}
	}
	
	helm->helm_move_t[0].angle_set = helm_angle_solving(helm->helm_move_t[0].angle_set,531);
	helm->helm_move_t[1].angle_set = helm_angle_solving(helm->helm_move_t[1].angle_set,6391);
	helm->helm_move_t[2].angle_set = helm_angle_solving(helm->helm_move_t[2].angle_set,8351);
	helm->helm_move_t[3].angle_set = helm_angle_solving(helm->helm_move_t[3].angle_set,6737);
}

double helm_angle_solving(double x, double middle)
{
	double code;
	double target;
	code = x/pi*4096;
	target = code+middle;
	if(target>8191)
		target = target-8191;
	else if (target<0)
		target = target+8191;
	return target;
}

void set_helm_pid_parameter (Helm_move_t *helm)
{
	int i;
//	switch(chassis->pid_mode)
//	
//		case(RC_MECH_PID):
				for(i=0;i<4;i++)
				{
					pid_init(&helm->helm_angle_pid[i], 2, 0, 0.5, 800, 5000);
					pid_init(&helm->helm_speed_pid[i], 70, 0, 0, 800, 15000);
				}
////		case(RC_GYRO_PID):
////				for(i=0;i<4;i++)
////				{
////					pid_init(&chassis->helm_angle_pid[i], 1.5, 0.11, 0, 1000, 16000);
////					pid_init(&chassis->helm_speed_pid[i], 400, 0, 0, 0, 20000);
////				}
//		case(RC_SPIN_PID):
//				for(i=0;i<4;i++)
//				{
//					pid_init(&chassis->helm_angle_pid[i], 40, 0, 0, 0, 15000);
//					pid_init(&chassis->helm_speed_pid[i], 10, 0, 0, 800, 12000);
//				}
//	}
}

void count_helm_output(Helm_move_t *helm)
{
	int i;
	
	for(i=0;i<4;i++)
	{
		helm->helm_move_t[i].speed_set = pid_calc(&helm->helm_angle_pid[i],helm->helm_move_t[i].angle_set,helm->helm_move_t[i].angle);
		helm->helm_move_t[i].give_current = pid_calc(&helm->helm_speed_pid[i],helm->helm_move_t[i].speed_set,helm->helm_move_t[i].speed);
	}
	CAN_cmd_helm(helm->helm_move_t[0].give_current,helm->helm_move_t[1].give_current,
				       helm->helm_move_t[2].give_current,helm->helm_move_t[3].give_current);

}

//void clear_pidout (helm_move_t *helm)
//{
//	int i;
//	for(i=0;i<4;i++)
//	{
//		chassis->helm_angle_pid[i].d_out = 0;
//		chassis->helm_angle_pid[i].i_out = 0;
//		chassis->helm_angle_pid[i].p_out = 0;
//		chassis->helm_angle_pid[i].output = 0;
//		chassis->helm_angle_pid[i].err[0] =0;
//		chassis->helm_angle_pid[i].err[1] =0;
//		chassis->helm_angle_pid[i].ref = 0;
//		chassis->helm_speed_pid[i].d_out = 0;
//		chassis->helm_speed_pid[i].i_out = 0;
//		chassis->helm_speed_pid[i].p_out = 0;
//		chassis->helm_speed_pid[i].output = 0;
//		chassis->helm_speed_pid[i].err[0] =0;
//		chassis->helm_speed_pid[i].err[1] =0;
//		chassis->helm_speed_pid[i].ref = 0;
//		chassis->chassis_speed_pid[i].d_out = 0;
//		chassis->chassis_speed_pid[i].i_out = 0;
//		chassis->chassis_speed_pid[i].p_out = 0;
//		chassis->chassis_speed_pid[i].output = 0;
//		chassis->chassis_speed_pid[i].err[0] =0;
//		chassis->chassis_speed_pid[i].err[1] =0;
//		chassis->chassis_speed_pid[i].ref = 0;
//	}
//}

//*************************************小陀螺***********************************//
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

//void count_spin_output(chassis_move_t *chassis)
//{
//	int i;
//	switch(chassis->Is_chassis_spin)
//	{
//		case(YES):
//		{
//			for(i=0;i<4;i++)
//			{
//				chassis->chassis_move_t[i].give_current = pid_calc(&chassis->chassis_speed_pid[i],chassis->chassis_move_t[i].speed_set,chassis->chassis_move_t->speed);
//			}
//			CAN_cmd_chassis(chassis->chassis_move_t[0].give_current,chassis->chassis_move_t[1].give_current,
//						   chassis->chassis_move_t[2].give_current,chassis->chassis_move_t[3].give_current);
//			break;
//		}
//		case(NO):
//		{
//			for(i=0;i<4;i++)
//			{
//				chassis->helm_move_t[i].speed_set = pid_calc(&chassis->helm_angle_pid[i],chassis->helm_move_t[i].angle_set,chassis->helm_move_t[i].angle);
//				chassis->helm_move_t[i].give_current = pid_calc(&chassis->helm_speed_pid[i],chassis->helm_move_t[i].speed_set,chassis->helm_move_t[i].speed);
//			}
//			CAN_cmd_helm(chassis->helm_move_t[0].give_current,chassis->helm_move_t[1].give_current,
//						 chassis->helm_move_t[2].give_current,chassis->helm_move_t[3].give_current);
//			for(i=0;i<4;i++)
//			{
//				chassis->chassis_move_t[i].give_current = pid_calc(&chassis->chassis_speed_pid[i],chassis->chassis_move_t[i].speed_set,chassis->chassis_move_t->speed);
//			}
//			CAN_cmd_chassis(chassis->chassis_move_t[0].give_current,chassis->chassis_move_t[1].give_current,
//						   chassis->chassis_move_t[2].give_current,chassis->chassis_move_t[3].give_current);
//			break;
//		}
//	}
//}

//void Output_optimization (chassis_move_t *chassis_motor_t)
//{
//	
//}

//void Reset_infomaiton (chassis_move_t *chassis)
//{
//	chassis->Vx_set = 0;
//	chassis->Vy_set =0;
//	chassis->Wz_set = 0;
//}
