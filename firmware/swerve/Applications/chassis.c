/**
  **************wust*********************************************************
  * @file       chassis.c/h
  * @brief      底盘任务
  * @author     wwy
  @verbatim
  ==============================================================================
  1.
  2.
	ps:
  ==============================================================================
  @endverbatim
  ******************************************************************************
  */

/**
 * @brief 头文件声明处
 * @param
 */
#include "chassis.h"
#include "math.h"
#include "remote_control.h"

#define pi 3.1415926535

chassis_move_t chassis_t;
Robot_chassis_t chassis;

Subscriber_t *chassis_cmd_from_robots; 

int r;

int before_ctrl_mode = ctrl_err;
//int before_pid_mode = PID_MODE_Err;

/* --------------------------任务层------------------------- */
void chassis_task (void const * argument)
{
	chassis_Init();
	for(;;)
	{
//		chassis_Ctrl();
		SubGetMessage(chassis_cmd_from_robots,&chassis);
		RC_Ctrl();
		osDelay(2);
	}
}
/* --------------------------控制层------------------------- */
void RC_Ctrl (void)
{
//	chassis_t.chassis_RC = get_remote_control_point();
	switch(chassis.chassis_control)
	{
		case(RC_MECH_control):
			if(before_ctrl_mode!= chassis.chassis_control)
			{
				before_ctrl_mode = chassis.chassis_control;
				chassis_t.pid_mode = RC_MECH_PID;
				chassis_t.chassis_pid_mode[CHAS_LF] = RC_MECH;
				chassis_t.chassis_pid_mode[CHAS_RF] = RC_MECH;
				chassis_t.chassis_pid_mode[CHAS_LB] = RC_MECH;
				chassis_t.chassis_pid_mode[CHAS_RB] = RC_MECH;
//				set_helm_pid_parameter(&chassis_t);
				set_chassis_pid_parameter(&chassis_t);
//				Reset_infomaiton(&chassis_t);
			}
			get_chassis_data(&chassis_t);
			resolving_speed(&chassis_t);
			count_output(&chassis_t);
//			clear_pidout(&chassis_t);
			break;
		case(RC_SPIN_control):
			if(before_ctrl_mode!= chassis.chassis_control)
			{
				before_ctrl_mode = chassis.chassis_control;
				chassis_t.pid_mode = RC_MECH_PID;
				chassis_t.chassis_pid_mode[CHAS_LF] = RC_MECH;
				chassis_t.chassis_pid_mode[CHAS_RF] = RC_MECH;
				chassis_t.chassis_pid_mode[CHAS_LB] = RC_MECH;
				chassis_t.chassis_pid_mode[CHAS_RB] = RC_MECH;
//				set_helm_pid_parameter(&chassis_t);
				set_chassis_pid_parameter(&chassis_t);
//				Reset_infomaiton(&chassis_t);
			}
			get_chassis_data(&chassis_t);
			resolving_speed(&chassis_t);
			count_output(&chassis_t);
//			clear_pidout(&chassis_t);
			break;
		case(RC_GYRO_control):
			if(before_ctrl_mode!= chassis.chassis_control)
			{
				before_ctrl_mode = chassis.chassis_control;
				chassis_t.pid_mode = RC_MECH_PID;
				chassis_t.chassis_pid_mode[CHAS_LF] = RC_MECH;
				chassis_t.chassis_pid_mode[CHAS_RF] = RC_MECH;
				chassis_t.chassis_pid_mode[CHAS_LB] = RC_MECH;
				chassis_t.chassis_pid_mode[CHAS_RB] = RC_MECH;
//				set_helm_pid_parameter(&chassis_t);
				set_chassis_pid_parameter(&chassis_t);
//				Reset_infomaiton(&chassis_t);
			}
			get_chassis_data(&chassis_t);
			resolving_speed(&chassis_t);
			count_output(&chassis_t);
//			clear_pidout(&chassis_t);
			break;
	}
}

void KEY_Ctrl (void)
{
	
}

/* --------------------------功能层------------------------- */
//底盘信息初始化
void chassis_Init(void)
{
	int i;
	for(i=0;i<4;i++)
	{
		chassis_t.chassis_move_t[i].speed = 0;
	}
	chassis_cmd_from_robots = SubRegister("chassis_cmd",sizeof(Robot_chassis_t));
}

//获取底盘电机信息
void get_chassis_data(chassis_move_t *chassis)
{
	chassis->chassis_move_t[CHAS_LF].motor_data = get_CHAS_LF_motor_measure_point();
	chassis->chassis_move_t[CHAS_RF].motor_data = get_CHAS_RF_motor_measure_point();
	chassis->chassis_move_t[CHAS_LB].motor_data = get_CHAS_LB_motor_measure_point();
	chassis->chassis_move_t[CHAS_RB].motor_data = get_CHAS_RB_motor_measure_point();
	
	chassis->chassis_move_t[CHAS_LF].speed = chassis->chassis_move_t[CHAS_LF].motor_data->speed_rpm;
	chassis->chassis_move_t[CHAS_RF].speed = chassis->chassis_move_t[CHAS_RF].motor_data->speed_rpm;
	chassis->chassis_move_t[CHAS_LB].speed = chassis->chassis_move_t[CHAS_LB].motor_data->speed_rpm;
	chassis->chassis_move_t[CHAS_RB].speed = chassis->chassis_move_t[CHAS_RB].motor_data->speed_rpm;
}

//解算底盘各电机的速度
void resolving_speed(chassis_move_t *chassis_t)
{
	float theta = atan(1.0/1.0);
	int16_t vx;
	int16_t vy;
	int16_t wz;
	
	int i;
	fp32 helm[4];
	
	vx = chassis.Vx_set*100;
	vy = chassis.Vy_set*100;
	wz = chassis.Wz_set*100;
	
	helm[0] = atan2((vy-wz*sin(theta)),
									(vx-wz*cos(theta)));
	helm[1] = atan2((vy-wz*sin(theta)),
											  (vx+wz*cos(theta)));
	helm[2] = atan2((vy+wz*sin(theta)),
											  (vx+wz*cos(theta)));
	helm[3] = atan2((vy+wz*sin(theta)),
											  (vx-wz*cos(theta)));
	
	for(i=0;i<4;i++)
	{
		if(helm[i]>pi/2)
		{
		    chassis_t->chassis_move_t[i].move_direction = -1;
		}
		else if(helm[i]<-pi/2)
		{
		    chassis_t->chassis_move_t[i].move_direction = -1;
		}
		else
			chassis_t->chassis_move_t[i].move_direction = 1;
	}
	
  chassis_t->chassis_move_t[0].speed_set = sqrt((vy-wz*cos(theta))*(vy-wz*cos(theta))+(vx-wz*sin(theta))*(vx-wz*sin(theta)))*chassis_t->chassis_move_t[0].move_direction;
	chassis_t->chassis_move_t[1].speed_set = sqrt((vy-wz*cos(theta))*(vy-wz*cos(theta))+(vx+wz*sin(theta))*(vx+wz*sin(theta)))*chassis_t->chassis_move_t[1].move_direction;
	chassis_t->chassis_move_t[2].speed_set = sqrt((vy+wz*cos(theta))*(vy+wz*cos(theta))+(vx+wz*sin(theta))*(vx+wz*sin(theta)))*chassis_t->chassis_move_t[2].move_direction;
	chassis_t->chassis_move_t[3].speed_set = sqrt((vy+wz*cos(theta))*(vy+wz*cos(theta))+(vx-wz*sin(theta))*(vx-wz*sin(theta)))*chassis_t->chassis_move_t[3].move_direction;
	
//	chassis_t->chassis_move_t[0].speed_set = vx + vy +  MOTOR_DISTANCE_TO_CENTER * wz;
//	chassis_t->chassis_move_t[1].speed_set = -vx + vy +  MOTOR_DISTANCE_TO_CENTER * wz;
//	chassis_t->chassis_move_t[2].speed_set = -vx - vy + MOTOR_DISTANCE_TO_CENTER * wz;
//	chassis_t->chassis_move_t[3].speed_set = vx - vy +  MOTOR_DISTANCE_TO_CENTER * wz;
}

//逆运动解算底盘速度


//设置底盘电机PID参数
void set_chassis_pid_parameter (chassis_move_t *chassis)
{
	int i;
	switch(chassis->pid_mode)
	{
		case(RC_MECH_PID):
				for(i=0;i<4;i++)
				{
					pid_init(&chassis->chassis_speed_pid[i], 1.5, 0.01, 0, 3000, 20000);
				}
		case(RC_GYRO_PID):
				for(i=0;i<4;i++)
				{
					pid_init(&chassis->chassis_speed_pid[i], 1.5, 0.01, 0, 0, 2000);
				}
		case(RC_SPIN_PID):
				for(i=0;i<4;i++)
				{
					pid_init(&chassis->chassis_speed_pid[i], 1.5, 0.01, 0, 3000, 20000);
				}
	}
}

//计算电机的输入电流
void count_output(chassis_move_t *chassis)
{
	chassis->chassis_move_t[CHAS_LF].give_current = pid_calc(&chassis->chassis_speed_pid[CHAS_LF],
	                                                          chassis->chassis_move_t[CHAS_LF].speed_set,
	                                                          chassis->chassis_move_t[CHAS_LF].speed);
	chassis->chassis_move_t[CHAS_RF].give_current = pid_calc(&chassis->chassis_speed_pid[CHAS_RF],
	                                                          chassis->chassis_move_t[CHAS_RF].speed_set,
	                                                          chassis->chassis_move_t[CHAS_RF].speed);
	chassis->chassis_move_t[CHAS_LB].give_current = pid_calc(&chassis->chassis_speed_pid[CHAS_LB],
	                                                          chassis->chassis_move_t[CHAS_LB].speed_set,
                                                            chassis->chassis_move_t[CHAS_LB].speed);
	chassis->chassis_move_t[CHAS_RB].give_current = pid_calc(&chassis->chassis_speed_pid[CHAS_RB],
	                                                          chassis->chassis_move_t[CHAS_RB].speed_set,
	                                                          chassis->chassis_move_t[CHAS_RB].speed);
	
	CAN_cmd_chassis(chassis->chassis_move_t[CHAS_LF].give_current,chassis->chassis_move_t[CHAS_RF].give_current,
	                chassis->chassis_move_t[CHAS_LB].give_current,chassis->chassis_move_t[CHAS_RB].give_current);

}
