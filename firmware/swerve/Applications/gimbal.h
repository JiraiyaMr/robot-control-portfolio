#ifndef __GIMBAL_H_
#define __GIMBAL_H_

#include "main.h"
#include "struct_typedef.h"
#include "remote_control.h"
#include "CAN_receive.h"
#include "pid.h"
#include "robots.h"
#include "pub_sub.h"
#include "FreeRTOS.h"
#include "task.h"
//#include "helm.h"
//#include "ins_task.h"
//#include "system_universal_definition.h"


#define MOTOR_DISTANCE_TO_CENTER 0.2f
#define CHASSIS_WZ_SET_SCALE 0.1f

typedef struct
{
  const motor_measure_t *motor_data;
  fp32 angle;
  fp32 angle_set;
  fp32 speed;
  fp32 speed_set;
  int16_t give_current;
} gimbal_motor_t;

typedef enum
{
  gim_Err =0,
  gim_YES =1,
  gim_NO =2,

}IS_GIMBAL_SPIN;

typedef struct
{
//	int16_t Gimbal_INS_MID;
////	int16_t chassis_INS_Target;
  double yaw_target_angle;
	uint16_t yaw_now_angle;
	double pitch_target_angle;
	uint16_t pitch_now_angle;
	
	double yaw_target;
	
  pid_struct_t gimbal_yaw_angle_pid;
	pid_struct_t gimbal_yaw_speed_pid;
	pid_struct_t gimbal_pitch_angle_pid;
	pid_struct_t gimbal_pitch_speed_pid;
//  PID_TYPE_t gimbal_yaw_type;
//  PID_TYPE_t gimbal_pitch_type;
////  fp32 spin_inception_information[3];

//  Ctrl_Mode_t ctrl_mode;
//  PID_Mode_t pid_mode;
	
  gimbal_motor_t gim_yaw_move_t;
	gimbal_motor_t gim_pitch_move_t;

  IS_GIMBAL_SPIN Is_gimbal_spin;

} gimbal_move_t;

typedef struct
{
	fp32 yaw_angle;
	fp32 pitch_angle;
}gimbal_feedback;

void gimbal_task(void *arg);

void Gimbal_Ctrl(void);
void Gimbal_RC_Ctrl (void);

void Gimbal_Init(void);

void gim_remote_data(gimbal_move_t *gimbal);

double gimbal_angle_solving(double x, double middle);

double angle_solve(double angle);

void count_gim_output(gimbal_move_t *gimbal);

void set_gimbal_pid_parameter (gimbal_move_t *gimbal);

void get_gimbal_motor_data (gimbal_move_t *gimbal);

#endif
