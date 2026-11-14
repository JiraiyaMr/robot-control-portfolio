/**
  **************wust*********************************************************
  * @file       shoot.c/h
  * @brief      shoot任务
  * @author     lhx
  @verbatim
  ==============================================================================
  1.Continuous_shoot mode is conducted by the single_pid "pid_shoot_calc(&shoot_
  control.trigger_motor_pid, shoot_control.speed, shoot_control.speed_set); " 
  its pid parameters are set by "TRIGGER_MOTOR_PID_KP..." in shoot.h
  
  2.Single_shoot mode is conducted by the cascade_pid,different from the contin-
  uous mode ,its pid output function is "count_shoot_output(&shoot_control);",
  its pid parameters are set by the function "set_shoot_pid_parameter(&shoot_con
  trol);" in shoot.c
  
	ps:
  ==============================================================================
  @endverbatim
  ******************************************************************************
  */

/**
 * @brief 头文件声明处
 * @param
 */
#include "shoot.h"

#define pi 3.1415926535
#define shoot_fric1_on(pwm) fric1_on((pwm)) //摩擦轮1pwm宏定义
#define shoot_fric2_on(pwm) fric2_on((pwm)) //摩擦轮2pwm宏定义
#define shoot_fric_off()    fric_off()      //关闭两个摩擦轮

#define shoot_laser_on()    laser_on()      //激光开启宏定义
#define shoot_laser_off()   laser_off()     //激光关闭宏定义

//微动开关IO
#define BUTTEN_TRIG_PIN HAL_GPIO_ReadPin(BUTTON_TRIG_GPIO_Port, BUTTON_TRIG_Pin)
shoot_control_t shoot_control;
 
fp32 fric1_speedset;
fp32 fric2_speedset;

Robot_shoot_t shoot_cmd;

Subscriber_t *shoot_cmd_from_robot; 
    
fp32 shoot_watch_1= 0.0f;
fp32 shoot_watch_2= 0.0f;
fp32 shoot_watch_3= 0.0f;
fp32 shoot_watch_4= 0.0f;
 
fp32 receive1=0.0f;
fp32 receive2=0.0f;
const static fp32 motor_speed_pid[3] = {M3505_gimbal_MOTOR_SPEED_PID_KP, M3505_gimbal_MOTOR_SPEED_PID_KI, M3505_gimbal_MOTOR_SPEED_PID_KD};


/**********************************************************************************************************
*函 数 名: Shoot_Init()
*功能说明: 初始化摩擦轮电机，拨盘电机pid参数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void Shoot_Init(shoot_control_t *init)
{
  
 
	uint8_t i;
	shoot_cmd.fric_flag = 0;
	init->fric_3508_motor_chassis[0].fric_3508_motor_measure = get_fric_1_measure_point();
	pid_shoot_init(&init->fric_3508_motor_speed_pid[0], PID_POSITION, motor_speed_pid, M3505_gimbal_MOTOR_SPEED_PID_MAX_OUT, M3505_gimbal_MOTOR_SPEED_PID_MAX_IOUT, M3505_gimbal_MOTOR_SPEED_PID_MAX_ERROR, M3505_gimbal_MOTOR_SPEED_PID_DEAD);

	init->fric_3508_motor_chassis[1].fric_3508_motor_measure = get_fric_2_measure_point();
	pid_shoot_init(&init->fric_3508_motor_speed_pid[1], PID_POSITION, motor_speed_pid, M3505_gimbal_MOTOR_SPEED_PID_MAX_OUT, M3505_gimbal_MOTOR_SPEED_PID_MAX_IOUT, M3505_gimbal_MOTOR_SPEED_PID_MAX_ERROR, M3505_gimbal_MOTOR_SPEED_PID_DEAD);

	Fric_feedback_update(init);//更新摩擦轮信息
	static const fp32 Trigger_motor_pid[3] = {TRIGGER_MOTOR_PID_KP,TRIGGER_MOTOR_PID_KI, TRIGGER_MOTOR_PID_KD}; //p有问题
	shoot_control.shoot_mode = SHOOT_READY;//状态机为 准备射击
	shoot_control.shoot_rc=get_remote_control_point();//获取遥控信息
 
	//  shoot_control.pc_vel = get_pc_cmd_vel_pointer();//获取小电脑信息
	shoot_control.shoot_motor_measure=get_trigger_motor_measure_point();//获取电机信息
	pid_shoot_init(&shoot_control.trigger_motor_pid,PID_POSITION, Trigger_motor_pid, TRIGGER_READY_PID_MAX_OUT, TRIGGER_READY_PID_MAX_IOUT, TRIGGER_READY_PID_MAX_ERROR, TRIGGER_READY_PID_DEAD);//初始化拨弹电机pid
	shoot_control.fric_can1 = FRIC_OFF;//摩擦轮关闭
	shoot_control.fric_can2 = FRIC_OFF;
	shoot_control.ecd_count = 0;
	shoot_control.angle = shoot_control.shoot_motor_measure->ecd * MOTOR_ECD_TO_ANGLE;
	shoot_control.given_current = 0;
	shoot_control.move_flag = 0;
//	shoot_control.angle_set = shoot_control.angle;

	shoot_control.speed = 0.0f;
	shoot_control.speed_set = 0.0f;
	shoot_control.trigger_speed_set=0;//拨盘电机转速
	shoot_control.key_time=0;
	set_shoot_pid_parameter(&shoot_control);

	shoot_cmd.Shoot_mode = Shoot_Stop;
	shoot_cmd_from_robot = SubRegister("shoot_cmd",sizeof(Robot_shoot_t));
}

/**********************************************************************************************************
*函 数 名:shoot_task(void const * argument)
*功能说明: 射击任务
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void shoot_task(void const * argument)
{
  Shoot_Init(&shoot_control);//初始化发射机构（包括摩擦轮和拨弹电机）
  shoot_control.angle_set=-31.5F;
  while(1)
  {
	get_shoot_data();
//	printf("%f,%f\n", shoot_control.angle_set, shoot_control.total_angle);
    SubGetMessage(shoot_cmd_from_robot,&shoot_cmd);
    Fric_feedback_update(&shoot_control);//摩擦轮电机信息更新
    shoot_ctrl();
    osDelay(1);
  }
}
/**********************************************************************************************************
*函 数 名:Shoot_Ctrl()
*功能说明: 射击控制
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void shoot_ctrl(void)
{
	/* key_mouse control*/


   if(shoot_cmd.Shoot_mode == Ready_Shoot)
		{
		 
        fric1_speedset=shoot_cmd.fric1_speed;
		fric2_speedset=shoot_cmd.fric2_speed;
        CAN_cmd_fric(shoot_control.fric_can1,shoot_control.fric_can2,0,0);      //3508摩擦轮can发送	
        shoot_control.total_angle=0;
        shoot_control.ecd_count=0;
        shoot_control.angle_set=0;
		shoot_cmd.angle_set=0;
        get_shoot_data();
		}
   if (shoot_cmd.Shoot_mode == Continuous_Shoot)//连发
    {
        //设置拨弹轮的拨动速度,并开启堵转反转处理
        shoot_control.speed_set = shoot_cmd.trigger_speed; 
		shoot_control.trigger_speed_set=shoot_control.speed_set;
        trigger_motor_turn_back();
        shoot_control.shoot_motor_measure = get_trigger_motor_measure_point();
        shoot_control.speed = shoot_control.shoot_motor_measure->speed_rpm * MOTOR_RPM_TO_SPEED;//计算输出轴速度
        pid_shoot_calc(&shoot_control.trigger_motor_pid, shoot_control.speed, shoot_control.speed_set);     //计算拨弹轮电机PID
        shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_pid.out);   
        fric1_speedset=shoot_cmd.fric1_speed;
		fric2_speedset=shoot_cmd.fric2_speed;			
        CAN_cmd_fric(shoot_control.fric_can1,shoot_control.fric_can2,shoot_control.given_current,0);      //3508摩擦轮can发送	
        
    }
   if (shoot_cmd.Shoot_mode == Fast_Continuous_Shoot)//fast连发
    {
        //设置拨弹轮的拨动速度,并开启堵转反转处理
        shoot_control.speed_set = shoot_cmd.trigger_speed; 
	    shoot_control.trigger_speed_set=shoot_control.speed_set;
        trigger_motor_turn_back();
        shoot_control.shoot_motor_measure = get_trigger_motor_measure_point();
        shoot_control.speed = shoot_control.shoot_motor_measure->speed_rpm * MOTOR_RPM_TO_SPEED;//计算输出轴速度
        pid_shoot_calc(&shoot_control.trigger_motor_pid, shoot_control.speed, shoot_control.speed_set);     //计算拨弹轮电机PID
        shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_pid.out);   
        fric1_speedset=shoot_cmd.fric1_speed;
	    fric2_speedset=shoot_cmd.fric2_speed;			
        CAN_cmd_fric(shoot_control.fric_can1,shoot_control.fric_can2,shoot_control.given_current,0);      //can发送	
        
    }
    if(shoot_cmd.Shoot_mode == Single_Shoot)//遥控器单发  鼠标左键按下
    {
       get_shoot_data();

       uint16_t i,flag=0;
       shoot_control.shoot_motor_measure = get_trigger_motor_measure_point();
       if(shoot_cmd.flag == 0)//遥控器控制
       count_shoot_output(&shoot_control);
       else		 //键鼠控制
       count_keyshoot_output(&shoot_control);
	   shoot_control.trigger_speed_set=shoot_control.speed_set;
       trigger_motor_turn_back();
					
       shoot_control.given_current = (int16_t)(shoot_control.trigger_speed_pid.out); // cascade pid output
      
       fric1_speedset=shoot_cmd.fric1_speed;
	   fric2_speedset=shoot_cmd.fric2_speed;
       CAN_cmd_fric(shoot_control.fric_can1,shoot_control.fric_can2,shoot_control.given_current,0);   
//				shoot_cmd.angle_set=0;
    
    }
    if(shoot_cmd.Shoot_mode == Shoot_Stop)
    {
        fric1_speedset=0;
	    fric2_speedset=0;
		Fric_3508_control_loop(&shoot_control);
        shoot_control.fric_can1=shoot_control.fric_3508_motor_chassis[0].give_current;
	    shoot_control.fric_can2=shoot_control.fric_3508_motor_chassis[1].give_current;		
	    CAN_cmd_fric(shoot_control.fric_can1,shoot_control.fric_can2,0,0);    
    }
    
	 Fric_3508_control_loop(&shoot_control);
     pid_shoot_calc(&shoot_control.trigger_motor_pid, shoot_control.speed, shoot_control.speed_set);     //计算拨弹轮电机PID
     shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_pid.out);      
     shoot_control.fric_can1=shoot_control.fric_3508_motor_chassis[0].give_current;
	 shoot_control.fric_can2=shoot_control.fric_3508_motor_chassis[1].give_current;	
  
	 shoot_watch_1=(fp32 )shoot_control.fric_can1;
	 shoot_watch_2= (fp32)shoot_control.fric_can2;
	 shoot_watch_3= shoot_control.fric_3508_motor_chassis[0].speed;
	 shoot_watch_4= shoot_control.fric_3508_motor_chassis[1].speed;
//      CAN_cmd_fric(shoot_control.fric_can1,shoot_control.fric_can2,shoot_control.given_current,0);      //3508摩擦轮can发送	
//      CAN_cmd_chassis(0,0,shoot_control.given_current,0); 

	
//	/* remote control*/
//	
//	
//  shoot_set_mode();//射击状态机设置
////  trigger_angle_control();
//  
//   if(shoot_control.shoot_mode == SHOOT_READY)
//		{
//		    shoot_control.speed_set = 0.0f;  
//        CAN_cmd_chassis(0,0,0,0);
//			  fric1_speedset=FRIC1_SPEED;
//        fric2_speedset=FRIC2_SPEED;
//        CAN_cmd_fric(shoot_control.fric_can1,shoot_control.fric_can2,0,0);      //3508摩擦轮can发送	
//        shoot_control.total_angle=0;
//      shoot_control.ecd_count=0;
//      shoot_control.angle_set=0;
////      get_shoot_data();
//		}
//   if (shoot_control.shoot_mode == SHOOT_CONTINUE_BULLET)//连发
//    {
//        //设置拨弹轮的拨动速度,并开启堵转反转处理
//        shoot_control.speed_set = -CONTINUE_TRIGGER_SPEED; 
//			  shoot_control.trigger_speed_set=shoot_control.speed_set;
//        trigger_motor_turn_back();
//        shoot_control.shoot_motor_measure = get_trigger_motor_measure_point();
//        shoot_control.speed = shoot_control.shoot_motor_measure->speed_rpm * MOTOR_RPM_TO_SPEED;//计算输出轴速度
//        pid_shoot_calc(&shoot_control.trigger_motor_pid, shoot_control.speed, shoot_control.speed_set);     //计算拨弹轮电机PID
//        shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_pid.out);   
//        fric1_speedset=FRIC1_SPEED;
//			  fric2_speedset=FRIC2_SPEED;			
//        CAN_cmd_fric(shoot_control.fric_can1,shoot_control.fric_can2,shoot_control.given_current,0);      //3508摩擦轮can发送	
//    }
//    if(shoot_control.shoot_mode == SHOOT_BULLET)//单发
//    {
//       get_shoot_data();

//     uint16_t i,flag=0;
//      

//        shoot_control.shoot_motor_measure = get_trigger_motor_measure_point();

//     
//     count_shoot_output(&shoot_control);    
//    
//        

//			  shoot_control.trigger_speed_set=shoot_control.speed_set;
//        trigger_motor_turn_back();
////        shoot_control.shoot_motor_measure = get_trigger_motor_measure_point();
////        shoot_control.speed = shoot_control.shoot_motor_measure->speed_rpm * MOTOR_RPM_TO_SPEED;//计算输出轴速度
////        pid_calc(&shoot_control.trigger_motor_pid, shoot_control.speed, shoot_control.speed_set);     //计算拨弹轮电机PID
//        
////      if((shoot_control.total_angle>shoot_control.angle_set))
////      {
////        shoot_control.given_current=0;
////      }
////      else
//          shoot_control.given_current = (int16_t)(shoot_control.trigger_speed_pid.out); 
//        fric1_speedset=FRIC1_SPEED;
//        fric2_speedset=FRIC2_SPEED;
//        CAN_cmd_fric(shoot_control.fric_can1,shoot_control.fric_can2,shoot_control.given_current,0);   

//    
//    }
//    else if(shoot_control.shoot_mode == SHOOT_DONE)
//    {
//			fric1_speedset=0;
//      fric2_speedset=0;
//        CAN_cmd_fric(shoot_control.fric_can1,shoot_control.fric_can2,0,0);
//    }
//    
//	    ramp_calc(&shoot_control.fric1_ramp, SHOOT_FRIC_PWM_ADD_VALUE);
//      ramp_calc(&shoot_control.fric2_ramp, SHOOT_FRIC_PWM_ADD_VALUE);
//		
////		  shoot_control.fric_3508_motor_chassis[0].speed_set =-shoot_control.fric1_ramp.out;
////		  shoot_control.fric_3508_motor_chassis[1].speed_set =shoot_control.fric2_ramp.out;
//		  Fric_3508_control_loop(&shoot_control);
//      pid_shoot_calc(&shoot_control.trigger_motor_pid, shoot_control.speed, shoot_control.speed_set);     //计算拨弹轮电机PID
//      shoot_control.given_current = (int16_t)(shoot_control.trigger_motor_pid.out);      
//      shoot_control.fric_can1=shoot_control.fric_3508_motor_chassis[0].give_current;
//	    shoot_control.fric_can2=shoot_control.fric_3508_motor_chassis[1].give_current;	
//  
//		  shoot_watch_1=(fp32 )shoot_control.fric_can1;
//		  shoot_watch_2= (fp32)shoot_control.fric_can2;
//		  shoot_watch_3= shoot_control.fric_3508_motor_chassis[0].speed;
//		  shoot_watch_4= shoot_control.fric_3508_motor_chassis[1].speed;
////      CAN_cmd_fric(shoot_control.fric_can1,shoot_control.fric_can2,shoot_control.given_current,0);      //3508摩擦轮can发送	
////      CAN_cmd_chassis(0,0,shoot_control.given_current,0); 

}

fp32 angle_zero_solve (fp32 tar,fp32 cur)
{
  if((tar-cur>PI)&&(tar-cur<2*PI) )
    cur+=2*PI;
  else if ((tar-cur<-PI)&&(tar-cur>-2*PI))
    cur-=2*PI;
  return cur;
}

/**********************************************************************************************************
*函 数 名: Fric_feedback_update(shoot_control_t *feedback_update)
*功能说明:底盘测量数据更新，包括电机速度，欧拉角度，机器人速度
*形    参: fric_feedback_update:"shoot_control_t"变量指针.
*返 回 值: 无
**********************************************************************************************************/
void Fric_feedback_update(shoot_control_t *feedback_update)
{
		uint8_t i = 0;
    if (feedback_update == NULL)
    {
        return;
    }
	
    for (i = 0; i < 2; i++) //开启3508摩擦轮的控制
    {
        //update motor speed, accel is differential of speed PID
        //更新电机速度  (得到的.speed为m/s)，    加速度是速度的PID微分
       //feedback_update->fric_3508_motor_chassis[i].fric_3508_motor_measure = get_chassis_motor_measure_point(i);
       feedback_update->fric_3508_motor_chassis[i].speed = CHASSIS_MOTOR_RPM_TO_VECTOR_SEN * feedback_update->fric_3508_motor_chassis[i].fric_3508_motor_measure->speed_rpm;
       feedback_update->fric_3508_motor_chassis[i].accel = feedback_update->fric_3508_motor_speed_pid[i].Dbuf[0] * CHASSIS_CONTROL_FREQUENCE;
    }
	
}
/**********************************************************************************************************
*函 数 名: Fric_3508_control_loop(shoot_control_t *Fric_3508_control)
*功能说明: 控制循环，根据控制设定值，计算电机电流值，进行控制
*形    参: chassis_move_control_loop:"Fric_3508_control"变量指针.
*返 回 值: 无
**********************************************************************************************************/
void Fric_3508_control_loop(shoot_control_t *Fric_3508_control)
{
    fp32 max_vector = 0.0f, vector_rate = 0.0f;
    fp32 temp = 0.0f;
    fp32 wheel_3508_speed[2] = {2.0f, -2.0f};
    uint8_t i = 0;
	
    //calculate the max speed in four wheels, limit the max speed
    //计算轮子控制最大速度，并限制其最大速度             将脉轮运动分解得到的各个速度赋给speed_set
    for (i = 0; i < 2; i++)
    {
        //gimbal_3508_control->gimbal_3508_motor_chassis[i].speed_set =wheel_3508_speed[i];
        temp = fabs(Fric_3508_control->fric_3508_motor_chassis[i].speed_set);
        if (max_vector < temp)
        {
            max_vector = temp;
        }
    }

    if (max_vector > 0.5)
    {
        vector_rate = 0.5 / max_vector;
        for (i = 0; i < 2; i++)
        {
            Fric_3508_control->fric_3508_motor_chassis[i].speed_set *= vector_rate;
        }
    }
    
    pid_shoot_calc(&Fric_3508_control->fric_3508_motor_speed_pid[0], Fric_3508_control->fric_3508_motor_chassis[0].speed, fric1_speedset);
    pid_shoot_calc(&Fric_3508_control->fric_3508_motor_speed_pid[1], Fric_3508_control->fric_3508_motor_chassis[1].speed, -fric2_speedset);
		
		receive1=Fric_3508_control->fric_3508_motor_chassis[0].speed;
		receive2=Fric_3508_control->fric_3508_motor_chassis[1].speed;

    //功率控制
   // chassis_power_control(gimbal_3508_control);

    //赋值电流值
    for (i = 0; i < 2; i++)
    {
        Fric_3508_control->fric_3508_motor_chassis[i].give_current = (int16_t)(Fric_3508_control->fric_3508_motor_speed_pid[i].out);
    }
}


/**********************************************************************************************************
*函 数 名: get_shoot_data()
*功能说明: 获取射击数据
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void get_shoot_data()
{
  
  shoot_control.shoot_motor_measure = get_trigger_motor_measure_point();


  shoot_control.speed = shoot_control.shoot_motor_measure->speed_rpm * MOTOR_RPM_TO_SPEED;//计算输出轴速度
  shoot_control.last_last_ecd=shoot_control.shoot_motor_measure->last_ecd;

 //电机圈数重置， 因为输出轴旋转一圈， 电机轴旋转 36圈，将电机轴数据处理成输出轴数据，用于控制输出轴角度
//  if((shoot_control.shoot_motor_measure->ecd<100)&&( shoot_control.shoot_motor_measure->last_ecd>8100))&&(shoot_control.shoot_motor_measure->ecd>300)&&( shoot_control.shoot_motor_measure->last_ecd<7900)
//    shoot_control.ecd_count++;&&(shoot_control.shoot_motor_measure->ecd<300)&&( shoot_control.shoot_motor_measure->last_ecd>7900)
    if ((shoot_control.shoot_motor_measure->ecd - shoot_control.last_last_ecd > 4096))
    {
        shoot_control.ecd_count--;
    }
  
   else if(shoot_control.shoot_motor_measure->ecd - shoot_control.last_last_ecd < -4096)
    {
        shoot_control.ecd_count++;
    }
  
    
    shoot_control.total_angle = (float)shoot_control.ecd_count * 360.0f + (float)shoot_control.shoot_motor_measure->ecd / 8192.0f * 360.0f;

//    
//    shoot_control.total_angle = (shoot_control.ecd_count * ECD_RANGE + shoot_control.shoot_motor_measure->ecd) * MOTOR_ECD_TO_ANGLE*57.3f;//计算输出轴角度
//    shoot_control.key = BUTTEN_TRIG_PIN; //微动开关
//

}

/**********************************************************************************************************
*函 数 名: set_shoot_pid_parameter(shoot_control_t *shoot)
*功能说明: 设置射击pid系数
*形    参: shoot
*返 回 值: 无
**********************************************************************************************************/
void set_shoot_pid_parameter(shoot_control_t *shoot)
{
  static const fp32 Fric_speed_pid[3]={1.5,0.1,0};
  static const fp32 Trigger_angle_pid[3]={250,0.1,0.65*8192/2/PI};
  static const fp32 Trigger_speed_pid[3]={200,0,3};
//   static const fp32 Trigger_angle_pid[3]={15,0.05,0.3};
//  static const fp32 Trigger_speed_pid[3]={15,0,0.3};
  
  int i;
  pid_shoot_init(&shoot->trigger_angle_pid,PID_POSITION,Trigger_angle_pid,10000,5000,10000,0);
  pid_shoot_init(&shoot->trigger_speed_pid,PID_POSITION,Trigger_speed_pid,6000,200,10000,0);
}

/**********************************************************************************************************
*函 数 名: shoot_set_mode(void)
*功能说明: 射击状态机设置，遥控器上拨一次开启，再上拨关闭，下拨1次发射1颗，一直处在下，则持续发射，用于3min准备时间清理子弹
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void shoot_set_mode(void)
{
  
    static int16_t last_l,last_r;
    static int16_t last_s,last_ch;

  
    if(shoot_control.shoot_rc->RC_CH4==660)
    {
      if(last_ch==0)
      {
        shoot_control.angle_set-=31.5f;
        shoot_control.shoot_mode=SHOOT_BULLET;
        last_ch=-660; 
      }
    }
    if(shoot_control.shoot_rc->RC_CH4==0)
    {
      shoot_control.shoot_mode=SHOOT_READY;
      last_ch=0;
    }
      
      switch(shoot_control.shoot_rc->S2)
     {
//       case 1: 
//       {
//         if(last_s==3)
//         {  
//          shoot_control.angle_set+=25.0f;

//         shoot_control.shoot_mode = SHOOT_BULLET;
//         last_s=0;
//         }
//       } break;
       case 2: shoot_control.shoot_mode = SHOOT_CONTINUE_BULLET; break;
      // case 3: shoot_control.shoot_mode = SHOOT_READY;last_s=3;break;
     }
    switch(shoot_control.shoot_rc->S2)//拨到最上边关闭电机
     {
       case 1: shoot_control.shoot_mode = SHOOT_DONE; break;
     }
     
}
/**********************************************************************************************************
*函 数 名:  trigger_motor_turn_back(void)
*功能说明: 堵转倒转处理
*形    参: 
*返 回 值: 无
**********************************************************************************************************/
void trigger_motor_turn_back(void)
{
    if( shoot_control.block_time < BLOCK_TIME)
    {
        shoot_control.speed_set = shoot_control.trigger_speed_set;
    }
    else
    {
        shoot_control.speed_set = -shoot_control.trigger_speed_set;
    }

    if(fabs(shoot_control.speed) < BLOCK_TRIGGER_SPEED && shoot_control.block_time < BLOCK_TIME)
    {
        shoot_control.block_time++;
        shoot_control.reverse_time = 0;
    }
    else if (shoot_control.block_time == BLOCK_TIME && shoot_control.reverse_time < REVERSE_TIME)
    {
        shoot_control.reverse_time++;
    }
    else
    {
        shoot_control.block_time = 0;
    }
}
/**********************************************************************************************************
*函 数 名:  trigger_angle_control()
*功能说明: 拨弹轮角度控制
*形    参: 
*返 回 值: 无
**********************************************************************************************************/
void trigger_angle_control()
{
  fp32 delta_angle;

}
/**********************************************************************************************************
*函 数 名:  count_output(shoot_control_t *shoot)
*功能说明: 输出pid结果
*形    参: shoot
*返 回 值: 无
**********************************************************************************************************/
void count_shoot_output(shoot_control_t *shoot)
{
 
  shoot->trigger_speed_set=pid_shoot_calc(&shoot->trigger_angle_pid,shoot->total_angle,shoot->angle_set);
  shoot->given_current=pid_shoot_calc(&shoot->trigger_speed_pid,shoot->shoot_motor_measure->speed_rpm,shoot->trigger_speed_set);

}

/**********************************************************************************************************
*函 数 名:  count__keyshoot_output(shoot_control_t *shoot)
*功能说明: 输出 key_mouse control pid结果
*形    参: shoot
*返 回 值: 无
**********************************************************************************************************/
void count_keyshoot_output(shoot_control_t *shoot)
{
  shoot->angle_set=shoot_cmd.angle_set;
  shoot->trigger_speed_set=pid_shoot_calc(&shoot->trigger_angle_pid,shoot->total_angle,shoot->angle_set);
  shoot->given_current=pid_shoot_calc(&shoot->trigger_speed_pid,shoot->shoot_motor_measure->speed_rpm,shoot->trigger_speed_set);
  

}
/**********************************************************************************************************
*函 数 名: Reset_infomaiton (shoot_control_t *shoot)
*功能说明: 重置信息
*形    参: shoot
*返 回 值: 无
**********************************************************************************************************/
void Reset_infomaiton (shoot_control_t *shoot)
{
	shoot->angle_set=0;
  shoot->speed_set=0;
  shoot->trigger_speed_set=0;
}