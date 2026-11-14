#ifndef _HELM_H
#define _HELM_H

#include "struct_typedef.h"
#include "pub_sub.h"
#include "robots.h"
#include "math.h"

#define HELM_LF 0
#define HELM_RF 1
#define HELM_LB 2
#define HELM_RB 3

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
  pid_struct_t helm_angle_pid[4];
  pid_struct_t helm_speed_pid[4];
  PID_TYPE_t helm_pid_mode[4];
  fp32 spin_inception_information[3];
//  const fp32 *chassis_INS_angle;
  PID_Mode_t pid_mode;
  int16_t Vx_set;
  int16_t Vy_set;
  int16_t Wz_set;
	
  chassis_motor_t helm_move_t[4];
  
  int move_direction;
  
  IS_CHASSIS_SPIN Is_chassis_spin;

} Helm_move_t;

void helm_Init(void);

void HELM_Ctrl (void);

void get_data_from_robots (void);

void get_helm_data (Helm_move_t *helm);
double helm_angle_solving(double x, double middle);
int16_t Handle_Angle8191_PID_Over_Zero(int16_t tar, uint16_t cur);
void Angle_adjust(Helm_move_t *helm);
void resolving_angle(Helm_move_t *helm);
void set_helm_pid_parameter (Helm_move_t *helm);
void count_helm_output(Helm_move_t *helm);

#endif
