#ifndef __PID_H_
#define __PID_H_

#include "main.h"
#include "struct_typedef.h"

enum PID_MODE
{
    PID_POSITION = 0,
    PID_DELTA
};


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

typedef struct
{
	float kp;
  float ki;
  float kd;
  float i_max;
  float out_max;
	
	float ff_k_delta;
	float ff_k_delta_two;
	
	float target[2];
	float target_change[2];
  
  float ref;      
  float fdb;      
  float err[2];   
 
  float p_out;
  float i_out;
  float d_out;
	float foeward_out;
  float output;
}forwardfeed_pid_struct_t;
 
typedef struct
{
    uint8_t mode;
    //PID 三参数
    fp32 Kp;
    fp32 Ki;
    fp32 Kd;

    fp32 max_out;  //最大输出
    fp32 max_iout; //最大积分输出

    fp32 set;		//设置值
    fp32 feedback;		//反馈
    fp32 err;

    fp32 out;
    fp32 Pout;
    fp32 Iout;
    fp32 Dout;
    fp32 Dbuf[3];  //微分项 0最新 1上一次 2上上次
    fp32 error[3]; //误差项 0最新 1上一次 2上上次
    
	  fp32 err_max;//lwx；新增pid err限幅
	  fp32 err_dead;//lwx；新增pid err死区限制
		
} pid_type_def;

void pid_init(pid_struct_t *pid,
              float kp,
              float ki,
              float kd,
              float i_max,
              float out_max);
 
void gimbal_PID_init(void);
							
							
void forwardfeed_pid_init(forwardfeed_pid_struct_t *pid,
              float kp,
              float ki,
              float kd,
							float ff_k_delta,
							float ff_k_delta_two,
              float i_max,
              float out_max);
							
fp32 forwardfeed_pid_calc(forwardfeed_pid_struct_t *pid, fp32 ref, fp32 fdb ,fp32 tar);
							
fp32 pid_shoot_calc(pid_type_def *pid, fp32 ref, fp32 set);					
float pid_calc(pid_struct_t *pid, float ref, float fdb);
void pid_shoot_init(pid_type_def *pid, uint8_t mode, const fp32 PID[3], fp32 max_out, fp32 max_iout, fp32 max_err, fp32 dead_err);
#endif
