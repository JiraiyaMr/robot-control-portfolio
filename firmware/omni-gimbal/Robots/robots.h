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
#include "ins_task.h"

//static PubSub chassis_cmd;

#define IS_TWO_BOARD 1    //定义是否为双板控制， 0为单板，1为双板
#define MASTER_OR_SLAVER 0    //定义是上C板还是下C板，0为下C板，1为上C板

#define FRIC_SPEED1 3.05f   //键鼠控制下的摩擦轮速度设定
#define FRIC_SPEED2 3.05f
#define CONTINUOUS_TRIGGER_SPEED -3.5f
#define FAST_CONTINUOUS_TRIGGER_SPEED -3.0f

//遥控控制时的枚举
typedef enum
{
	ctrl_err          =0,
	RC_MECH_control   =1,
	RC_GYRO_control   =2,
	RC_SPIN_control   =3,
	key_control       =4,
	
}Robot_control_mode;

//键盘控制时的枚举
typedef enum
{
	key_err           =0,
  Key_GYRO_control  =1,
	Key_SPIN_control  =2,
	Key_AIM_control   =3,
}Key_control_mode;

//键盘控制时的底盘枚举
typedef enum
{
	Chassis_err           =0,
  Chassis_GYRO_control  =1,
	Chassis_SPIN_control  =2,
	Chassis_AIM_control   =3,
}Key_chassis_mode;

//键盘控制时的云台枚举
typedef enum
{
	Gimbal_err           =0,
  Gimbal_GYRO_control  =1,
	Gimbal_SPIN_control  =2,
	Gimbal_AIM_control   =3,
}Key_Gimbal_mode;

typedef enum
{
	Shoot_Stop             =4,
	Single_Shoot           =1,
	Double_Shoot           =2,
	Continuous_Shoot       =3,
	Ready_Shoot            =5,
	Fast_Continuous_Shoot  =6,

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
    double Wz_set;
	Robot_control_mode chassis_control;
	Key_chassis_mode key_chassis;
}Robot_chassis_t;

//云台结构体
typedef struct
{
	int16_t yaw_motor_mid;
	int16_t pitch_motor_mid;
	
	int flag;
	
	double yaw_target_angle;
	double last_yaw_target_angle;
	double total_yaw_target_angle;
	double later_yaw_target_angle;
	
	int16_t ControlRoundCount;
	
	double yaw_now_angle;
	double pitch_target_angle;
	double pitch_now_angle;
	Robot_control_mode gimbal_control;
	
	int16_t yaw_speed;
	
	Key_Gimbal_mode key_gimbal;
}Robot_gimbal_t;

//发射机构结构体
typedef struct
{
	Robot_mag_mode mag_mode;
	Robot_shoot_mode Shoot_mode;
	Robot_shoot_mode Last_Shoot_mode;

	fp32 trigger_speed;
	fp32 Shoot_speed;
	
	fp32 speed;
  fp32 speed_set;
  fp32 angle;
  fp32 angle_set;
	
	fp32 fric1_speed;
	fp32 fric2_speed;
	int flag;
	int Ready_Flag;
	int fric_flag;
}Robot_shoot_t;


//机器人状态结构体
typedef struct
{
	const RC_t *Robot_RC;
	const KEY_Mouse_t *Robot_KEY;
	
	Robot_control_mode Robot_Control_mode;
    Robot_control_mode Last_Control_mode;
	
	Key_control_mode Key_control_mode;
	Key_control_mode Last_Key_control_mode;
}Robot_Status;

void robot_task(void const * argument);

void ROBOT_CTRL (Robot_Status *RS_t);

void Control_Mode_Handler(Robot_Status *RS_t);

void Key_Mode_Handler(Robot_Status *RS_t);

void Key_Chssis_Message_Handler (Robot_Status *RS_t,Robot_chassis_t *chassis_t);
void Key_Gimbal_Message_Handler (Robot_Status *RS_t,Robot_gimbal_t *gimbal_t);

void Chaais_Message_Handler(Robot_Status *RS_t,Robot_chassis_t *chassis_t);

void Gimbal_Message_Handler(Robot_Status *RS_t,Robot_gimbal_t *gimbal_t);

void Key_Shoot_Message_Handler (Robot_Status *RS_t);

void Robot_Init(Robot_Status *RS_t);

void Magazine (Robot_shoot_t *shoot_cmd_t);

void Cuttinggun(Robot_shoot_t *shoot_cmd_t);

void Shoot(Robot_shoot_t *shoot_cmd_t);
/* --------------------------功能层------------------------- */
double offsetAngle (void);

int16_t RC_to_Chassis (int16_t RC_data,int16_t XianFu);

double Pitch_Angle (double angle);

double Yaw_Angle (double angle);

#endif
