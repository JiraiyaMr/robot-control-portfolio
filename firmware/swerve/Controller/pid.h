#ifndef __PID_H_
#define __PID_H_

#include "main.h"
#include "struct_typedef.h"

typedef struct _pid_struct_t
{
  float kp;
  float ki;
  float kd;
  float i_max;
  float out_max;
  
  float ref;      
  float fdb;      
  float err[2];   
 
  float p_out;
  float i_out;
  float d_out;
  float output;
}pid_struct_t;
 
void pid_init(pid_struct_t *pid,
              float kp,
              float ki,
              float kd,
              float i_max,
              float out_max);
 
void gimbal_PID_init(void);
float pid_calc(pid_struct_t *pid, float ref, float fdb);

#endif
