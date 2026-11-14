#ifndef _ROBOTS_H
#define _ROBOTS_H
#include "struct_typedef.h"
#include "stm32f4xx.h"                  // Device header
#include "remote_control.h"
//#include "tools.h"
#include "pub_sub.h"
#include <stdio.h>
#include "gimbal.h"
//#include "ins.h"
#include "math.h"

//static PubSub chassis_cmd;

#define IS_TWO_BOARD 1    //定义是否为双板控制， 0为单板，1为双板
#define MASTER_OR_SLAVER 0    //定义是上C板还是下C板，0为下C板，1为上C板

typedef enum
{
	ctrl_err          =0,
	RC_MECH_control   =1,
	RC_GYRO_control   =2,
	RC_SPIN_control   =3,
	key_control       =4,
	
}Robot_control_mode;

typedef enum
{
	Shoot_Stop        =0,
	Continuous_Shoot  =1,
	Single_Shoot      =2,
	
}Robot_shoot_mode;

typedef enum
{
	OPEN              =1,
	CLOSE             =2,
	
}Robot_mag_mode;

//底盘结构体
typedef struct
{
	int16_t Vx_set;
  int16_t Vy_set;
  int16_t Wz_set;
	Robot_control_mode chassis_control;
}Robot_chassis_t;

//云台结构体
typedef struct
{
	int16_t yaw_motor_mid;
	double yaw_target_angle;
	double yaw_now_angle;
	double pitch_target_angle;
	double pitch_now_angle;
	Robot_control_mode gimbal_control;
}Robot_gimbal_t;

//发射机构结构体
typedef struct
{
	Robot_mag_mode mag_mode;
	Robot_shoot_mode Shoot_mode;

	int16_t trigger_speed;
	int16_t Shoot_speed;
}Robot_shoot_t;

//机器人状态结构体
typedef struct
{
	const RC_t *Robot_RC;
	const KEY_Mouse_t *Robot_KEY;
	Robot_control_mode Robot_Control_mode;
}Robot_Status;

void robot_task(void const * argument);

void ROBOT_CTRL (Robot_Status *RS_t);

void Control_Mode_Handler(Robot_Status *RS_t);

void Chaais_Message_Handler(Robot_Status *RS_t,Robot_chassis_t *chassis_t);

void Gimbal_Message_Handler(Robot_Status *RS_t,Robot_gimbal_t *gimbal_t);

void Robot_Init(Robot_Status *RS_t);

void Magazine (Robot_shoot_t *shoot_cmd_t);
/* --------------------------功能层------------------------- */
double offsetAngle (void);

int16_t RC_to_Chassis (int16_t RC_data,int16_t XianFu);

double Pitch_Angle (double angle);

double Yaw_Angle (double angle);

#endif
