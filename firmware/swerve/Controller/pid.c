#include "pid.h"
 
pid_struct_t gimbal_yaw_speed_pid;
pid_struct_t gimbal_yaw_angle_pid;
 
void pid_init(pid_struct_t *pid,
              float kp,
              float ki,
              float kd,
              float i_max,
              float out_max)
{
  pid->kp      = kp;
  pid->ki      = ki;
  pid->kd      = kd;
  pid->i_max   = i_max;
  pid->out_max = out_max;
}
 
 
float pid_calc(pid_struct_t *pid, float ref, float fdb)
{
  pid->ref = ref;
  pid->fdb = fdb;
  pid->err[1] = pid->err[0];
  pid->err[0] = pid->ref - pid->fdb;
  
  pid->p_out  = pid->kp * pid->err[0];
  pid->i_out += pid->ki * pid->err[0];
  pid->d_out  = pid->kd * (pid->err[0] - pid->err[1]);
//	if(pid->i_out>pid->i_max)
//		pid->i_out=pid->i_max;
//	else if(pid->i_out<-pid->i_max)
//        pid->i_out=-pid->i_max;		
  
  pid->output = pid->p_out + pid->i_out + pid->d_out;
	if(pid->output>pid->out_max)
		pid->output=pid->out_max;
	else if(pid->output<-pid->out_max)
        pid->output=-pid->out_max;	
 
  return pid->output;
}
 
//void gimbal_PID_init()
//{
//	pid_init(&gimbal_yaw_speed_pid, 1.5, 0.11, 0, 1000, 16000);//P=30,I=0,D=0
//	pid_init(&gimbal_yaw_angle_pid, 400, 0, 0, 0, 320);//P=400,I=0,D=0
//}
