#include "pid.h"
 
pid_struct_t gimbal_yaw_speed_pid;
pid_struct_t gimbal_yaw_angle_pid;
 
#define LimitMax(input, max)   \
    {                          \
        if (input > max)       \
        {                      \
            input = max;       \
        }                      \
        else if (input < -max) \
        {                      \
            input = -max;      \
        }                      \
    }
 
 
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
/**
  * @brief          pid计算
  * @param[out]     pid: PID结构数据指针
  * @param[in]      ref: 反馈数据
  * @param[in]      set: 设定值
  * @retval         pid输出
  */
fp32 pid_shoot_calc(pid_type_def *pid, fp32 ref, fp32 set)
{
    if (pid == NULL)
    {
        return 0.0f;
    }

    pid->error[2] = pid->error[1];
    pid->error[1] = pid->error[0];
    pid->set = set;
    pid->feedback = ref;
    pid->error[0] = set - ref;
    if (pid->mode == PID_POSITION)//位置式PID
    {
        pid->Pout = pid->Kp * pid->error[0];
        pid->Iout += pid->Ki * pid->error[0];
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - pid->error[1]);
        pid->Dout = pid->Kd * pid->Dbuf[0];
        LimitMax(pid->Iout, pid->max_iout);
        pid->out = pid->Pout + pid->Iout + pid->Dout;//总输出
        LimitMax(pid->out, pid->max_out);
    }
    else if (pid->mode == PID_DELTA)//增量式PID
    {
        pid->Pout = pid->Kp * (pid->error[0] - pid->error[1]);
        pid->Iout = pid->Ki * pid->error[0];
        pid->Dbuf[2] = pid->Dbuf[1];
        pid->Dbuf[1] = pid->Dbuf[0];
        pid->Dbuf[0] = (pid->error[0] - 2.0f * pid->error[1] + pid->error[2]);
        pid->Dout = pid->Kd * pid->Dbuf[0];
        pid->out += pid->Pout + pid->Iout + pid->Dout;//总输出
        LimitMax(pid->out, pid->max_out);
    }
    return pid->out;
}

/**
  * @brief          pid struct data init
  * @param[out]     pid: PID结构数据指针
  * @param[in]      mode: PID_POSITION:普通PID
  *                 PID_DELTA: 差分PID
  * @param[in]      PID: 0: kp, 1: ki, 2:kd
  * @param[in]      max_out: pid最大输出
  * @param[in]      max_iout: pid最大积分输出
  * @retval         none
  */
void pid_shoot_init(pid_type_def *pid, uint8_t mode, const fp32 PID[3], fp32 max_out, fp32 max_iout, fp32 max_err, fp32 dead_err)
{
    if (pid == NULL || PID == NULL)
    {
        return;
    }
    pid->mode = mode;
    pid->Kp = PID[0];
    pid->Ki = PID[1];
    pid->Kd = PID[2];
    pid->max_out = max_out;
    pid->max_iout = max_iout;
    pid->Dbuf[0] = pid->Dbuf[1] = pid->Dbuf[2] = 0.0f;
    pid->error[0] = pid->error[1] = pid->error[2] = pid->Pout = pid->Iout = pid->Dout = pid->out = 0.0f;
		//lwx;新加err限幅 和死区限制
		pid->err_max = max_err;
		pid->err_dead = dead_err;
}

/**
  * @brief          forwardfeed_pid_init
  * @param[out]     pid: PID结构数据指针
  * @param[in]      mode: 前馈式PID
  * @param[in]      PID: 0: kp, 1: ki, 2:kd
  * @param[in]      max_out: pid最大输出
  * @param[in]      max_iout: pid最大积分输出
  * @retval         none
  */
void forwardfeed_pid_init(forwardfeed_pid_struct_t *pid,
              float kp,
              float ki,
              float kd,
							float ff_k_delta,
							float ff_k_delta_two,
              float i_max,
              float out_max)
{
  pid->kp      = kp;
  pid->ki      = ki;
  pid->kd      = kd;
  pid->i_max   = i_max;
  pid->out_max = out_max;
	pid->ff_k_delta = ff_k_delta;
	pid->ff_k_delta_two = ff_k_delta_two;
	pid->ff_k_delta_two = ff_k_delta_two;
}

/**
  * @brief          前馈式pid计算
  * @param[out]     pid: PID结构数据指针
  * @param[in]      ref: 反馈数据
  * @param[in]      set: 设定值
  * @param[in]      fdb：
  * @retval         pid输出
  */
fp32 forwardfeed_pid_calc(forwardfeed_pid_struct_t *pid, fp32 ref, fp32 fdb ,fp32 tar)
{
  pid->ref = ref;
  pid->fdb = fdb;
  pid->err[1] = pid->err[0];
  pid->err[0] = pid->ref - pid->fdb;
	
	pid->target_change[1] = pid->target_change[0];
	pid->target_change[0] = pid->target[0]-pid->target[1];
	
	pid->target[1] = pid->target[0];
	pid->target[0] = tar;
	
	pid->foeward_out = pid->ff_k_delta*(pid->target[0] - pid->target[1]) + 
										 pid->ff_k_delta_two*(pid->target_change[0] - pid->target_change[1]);
  
  pid->p_out  = pid->kp * pid->err[0];
  pid->i_out += pid->ki * pid->err[0];
  pid->d_out  = pid->kd * (pid->err[0] - pid->err[1]);
	
	if(pid->i_out>pid->i_max)
		pid->i_out=pid->i_max;
	else if(pid->i_out<-pid->i_max)
        pid->i_out=-pid->i_max;		
  
  pid->output = pid->p_out + pid->i_out + pid->d_out + pid->foeward_out;
	if(pid->output>pid->out_max)
		pid->output=pid->out_max;
	else if(pid->output<-pid->out_max)
        pid->output=-pid->out_max;	
 
  return pid->output;
}