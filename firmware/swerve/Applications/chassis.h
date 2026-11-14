#ifndef _CHASSIS_H
#define _CHASSIS_H
#include "struct_typedef.h"
#include "stm32f4xx.h"                  // Device header
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "robots.h"
#include "CAN_receive.h"
#include "pid.h"
#include "pub_sub.h"

#define CHAS_LF 0
#define CHAS_RF 1
#define CHAS_LB 2
#define CHAS_RB 3

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
  int move_direction;
} chassis_motor_t;

typedef enum
{
  PID_MODE_Err = 0,
  RC_MECH_PID = 1,     
  RC_GYRO_PID = 2,
  RC_SPIN_PID = 3,	
  PID_MODE_PID_CNT ,

}PID_Mode_t;

typedef enum
{
  PID_Err = 0,
  RC_MECH = 1,     
  RC_GYRO = 2,
  RC_SPIN = 3,	
  PID_MODE_CNT ,

}PID_TYPE_t;

typedef enum
{
  Err = 0,
  YES = 1,
  NO = 2,

}IS_CHASSIS_SPIN;

typedef struct
{
  pid_struct_t chassis_speed_pid[4];
	
  PID_TYPE_t chassis_pid_mode[4];
  fp32 spin_inception_information[3];
//  const fp32 *chassis_INS_angle;
	
 
  PID_Mode_t pid_mode;
  int16_t Vx_set;
  int16_t Vy_set;
  int16_t Wz_set;
	Robot_control_mode chassis;
	
  chassis_motor_t chassis_move_t[4];
	
	int move_direction;
	
} chassis_move_t;

void chassis_Init(void);

void chassis_task (void const * argument);

void RC_Ctrl (void);
void HELM_Ctrl (void);
void KEY_Ctrl (void);
//void chassis_task(void *arg);
void helm_task(void *arg);
void chassis_Ctrl(void);
void Helm_Ctrl(void);

void receive_chassis_cmd (const void* data);

//double msp(double x, double in_min, double in_max, double out_min, double out_max);

void get_remote_data(chassis_move_t *chassis,int32_t MAX_SPEED,int32_t MAX_WZ_SPEED);
void get_chassis_data (chassis_move_t *chassis);
void get_chassis_data2 (chassis_move_t *chassis);
void set_helm_pid_parameter (chassis_move_t *chassis);
void set_chassis_pid_parameter (chassis_move_t *chassis);
void resolving_speed(chassis_move_t *chassis);
void resolving_angle(chassis_move_t *chassis);
void count_output(chassis_move_t *chassis);
void count_output2(chassis_move_t *chassis);

int32_t  Pch_To_Speed(int32_t Channel,int32_t Speed_Max);
void clear_pidout (chassis_move_t *chassis);

void Initialize_spin_after_init (chassis_move_t *chassis);
void count_spin_output(chassis_move_t *chassis);
void Reset_infomaiton (chassis_move_t *chassis);


#endif
