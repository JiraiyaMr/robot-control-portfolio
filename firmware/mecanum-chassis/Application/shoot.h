#ifndef SHOOT_H
#define SHOOT_H

#include "struct_typedef.h"
#include "remote_control.h"
#include "CAN_receive.h"
#include "pid.h"
#include "FreeRTOS.h"
#include "task.h"
#include "user_lib.h"
#include "main.h"
#include "bsp_fric.h"

//射击发射开关通道数据
#define SHOOT_RC_MODE_CHANNEL       1
//云台模式使用的开关通道
#define SHOOT_CONTROL_TIME          2

//电机码盘值最大以及中值
#define HALF_ECD_RANGE  4096
#define ECD_RANGE       8190

//电机rmp 变化成 旋转速度的比例
#define MOTOR_RPM_TO_SPEED          0.00290888208665721596153948461415f
#define MOTOR_ECD_TO_ANGLE          0.000021305288720633905968306772076277f
#define FULL_COUNT                  18

//拨弹速度
#define TRIGGER_SPEED               4.9f
#define CONTINUE_TRIGGER_SPEED      14.0f
#define READY_TRIGGER_SPEED         5.0f

//摩擦轮速度
#define FRIC1_SPEED                 3.0f
#define FRIC2_SPEED                 3.0f //补偿机械误差


//拨弹轮电机PID
#define TRIGGER_ANGLE_PID_KP        400.0f  
#define TRIGGER_ANGLE_PID_KI        0.1f
#define TRIGGER_ANGLE_PID_KD        2.0f

//卡单时间 以及反转时间
#define BLOCK_TRIGGER_SPEED         1.0f
#define BLOCK_TIME                  1000
#define REVERSE_TIME                500
#define REVERSE_SPEED_LIMIT         13.0f

#define TRIGGER_BULLET_PID_MAX_OUT  10000.0f
#define TRIGGER_BULLET_PID_MAX_IOUT 9000.0f

#define TRIGGER_READY_PID_MAX_OUT   10000.0f
#define TRIGGER_READY_PID_MAX_IOUT  7000.0f
#define TRIGGER_READY_PID_MAX_ERROR  10000.0f
#define TRIGGER_READY_PID_DEAD  0.0f

//射击完成后 子弹弹出去后，判断时间，以防误触发
#define SHOOT_DONE_KEY_OFF_TIME     15
//鼠标长按判断
#define PRESS_LONG_TIME             400
//遥控器射击开关打下档一段时间后 连续发射子弹 用于清单
#define RC_S_LONG_TIME              2000
//摩擦轮高速 加速 时间
#define UP_ADD_TIME                 80

//电机反馈码盘值范围
#define HALF_ECD_RANGE              4096


//电机rmp 变化成 旋转速度的比例
#define MOTOR_RPM_TO_SPEED          0.00290888208665721596153948461415f
#define MOTOR_ECD_TO_ANGLE          0.000021305288720633905968306772076277f
#define FULL_COUNT                  18

//微动开关IO
#define BUTTEN_TRIG_PIN HAL_GPIO_ReadPin(BUTTON_TRIG_GPIO_Port, BUTTON_TRIG_Pin)

//射击摩擦轮激光打开 关闭
#define SHOOT_ON_KEYBOARD           KEY_PRESSED_OFFSET_Q
#define SHOOT_OFF_KEYBOARD          KEY_PRESSED_OFFSET_E

#define SWITCH_TRIGGER_ON           0
#define SWITCH_TRIGGER_OFF          1

#define SHOOT_FRIC_PWM_ADD_VALUE    0.2f
//单个底盘电机最大速度
#define MAX_WHEEL_SPEED    0.5f       //4.0

//云台3508最大can发送电流值
#define MAX_MOTOR_CAN_CURRENT 16000.0f

//云台3505电机速度环PID
#define M3505_gimbal_MOTOR_SPEED_PID_KP 3000.0f
#define M3505_gimbal_MOTOR_SPEED_PID_KI 1.0f
#define M3505_gimbal_MOTOR_SPEED_PID_KD 2.0f
#define M3505_gimbal_MOTOR_SPEED_PID_MAX_OUT MAX_MOTOR_CAN_CURRENT
#define M3505_gimbal_MOTOR_SPEED_PID_MAX_IOUT 4000.0f
#define M3505_gimbal_MOTOR_SPEED_PID_MAX_ERROR 1000.0f//lwx：不知道参数 后续要调参
#define M3505_gimbal_MOTOR_SPEED_PID_DEAD 5.0f

//m3508转化成底盘速度(m/s)的比例，
#define M3508_MOTOR_RPM_TO_VECTOR 0.000414446438251351f 
#define CHASSIS_MOTOR_RPM_TO_VECTOR_SEN M3508_MOTOR_RPM_TO_VECTOR
#define MAX_3508_WHEEL_SPEED 1.0f

//底盘任务控制频率，尚未使用这个宏
#define CHASSIS_CONTROL_FREQUENCE 500.0f

#define PI_TEN                      0.314f
#define ANGLE_TARGET                0.15f


typedef enum
{
    SHOOT_STOP = 0,
    SHOOT_READY_FRIC,
    SHOOT_READY_BULLET,
    SHOOT_READY,
    SHOOT_BULLET,
    SHOOT_CONTINUE_BULLET,
    SHOOT_DONE,
} shoot_mode_e;//射击状态机

//两个3508摩擦轮数据结构体
typedef struct
{
  const motor_measure_t *fric_3508_motor_measure;
  fp32 accel;
  fp32 speed;                // 反馈速度     单位为m/s
  fp32 speed_set;            // 设定速度
  int16_t give_current;      //发送的pid计算得到的电流
} fric_3508_motor_t;  

typedef struct
{
  shoot_mode_e shoot_mode;
  shoot_mode_e last_shoot_mode;
  const RC_t *shoot_rc;
  const motor_measure_t *shoot_motor_measure;
  
  fric_3508_motor_t  fric_3508_motor_chassis[2]; //两个3508摩擦轮数据 
  pid_type_def fric_3508_motor_speed_pid[2];       //gimbal_motor speed PID.云台电机速度pid
  
  ramp_function_source_t fric1_ramp;
  uint16_t fric_can1;
  ramp_function_source_t fric2_ramp;
  uint16_t fric_can2;
  
  pid_type_def trigger_speed_pid;
  pid_type_def trigger_angle_pid;
  pid_type_def trigger_motor_pid;
  fp32 trigger_speed_set;//拨弹轮速度
  fp32 speed;
  fp32 speed_set;
  fp32 angle;
  fp32 angle_set;
  fp32 angle_last;
  int16_t given_current;
  fp32 last_last_ecd;
  fp32 ecd_count;
  fp32 total_angle;
  
  bool_t press_l;
  bool_t press_r;
  bool_t last_press_l;
  bool_t last_press_r;
  uint16_t press_l_time;
  uint16_t press_r_time;
  uint16_t rc_s_time;

  bool_t move_flag;
  
  pid_type_def fric_speed_pid[2];//摩擦轮pid
  
  uint16_t block_time;
  uint16_t reverse_time;
  
  bool_t key;
  uint8_t key_time;
  
  bool_t shoot_done;
  
  
  
  
}shoot_control_t;


fp32 angle_zero_solve (fp32 tar,fp32 cur);

void shoot_ctrl(void);
void Shoot_Init(shoot_control_t *shoot);
void shoot_task(void const * argument);
void get_shoot_data();
void set_shoot_pid_parameter(shoot_control_t *shoot);
void trigger_angle_control();
void count_shoot_output(shoot_control_t *shoot);
void shoot_set_mode(void);
void trigger_motor_turn_back(void);
void Fric_3508_control_loop(shoot_control_t *Fric_3508_control);
void Fric_feedback_update(shoot_control_t *feedback_update);
void Fric_PID_Init(void);
#endif
