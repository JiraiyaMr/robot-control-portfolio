#include "robots.h"

#define pi 3.1425926
int Q=1,last_Q=0;

int bv;

Robot_Status Robot_Status_t;
Robot_chassis_t chassis_cmd_t;
Robot_gimbal_t gimbal_cmd_t;
Robot_shoot_t shoot_cmd_t;

gimbal_feedback gimbai_fb_from_gimbal;
IMU_feedback ins_fb_form_ins;

//底盘信息发布和订阅的结构体
Publisher_t *chassis_cmd_pub;   
Subscriber_t *chassis_feed_sub; 

//云台信息发布和订阅的结构体
Publisher_t *gimbal_cmd_pub;   
Subscriber_t *gimbal_feed_sub; 

//发射信息发布和订阅的结构体
Publisher_t *shoot_cmd_pub;   
Subscriber_t *shoot_feed_sub; 

//陀螺仪信息订阅的结构体
Subscriber_t *ins_feed_sub;

/* --------------------------任务层------------------------- */
void robot_task(void const * argument)
{
	Robot_Init(&Robot_Status_t);
	gimbal_cmd_t.yaw_motor_mid = 8170;
  gimbal_cmd_t.pitch_motor_mid = 2443;
	for(;;)
	{
		
		SubGetMessage(gimbal_feed_sub,&gimbai_fb_from_gimbal);
		SubGetMessage(ins_feed_sub,&ins_fb_form_ins);
		ROBOT_CTRL(&Robot_Status_t);
		PubPushMessage(chassis_cmd_pub,(void *)&chassis_cmd_t);
		PubPushMessage(gimbal_cmd_pub,(void *)&gimbal_cmd_t);
		PubPushMessage(shoot_cmd_pub,(void *)&shoot_cmd_t);
		osDelay(2);
	}
}

/* --------------------------控制层------------------------- */
//机器人命令初始化
void Robot_Init(Robot_Status *RS_t)
{
//	#if  IS_TWO_BOARD
//	    #ifdef  MASTER_OR_SLAVER
				chassis_cmd_pub = PubRegister("chassis_cmd",sizeof(Robot_chassis_t));
				gimbal_cmd_pub = PubRegister("gimbal_cmd",sizeof(Robot_gimbal_t));
	            gimbal_feed_sub = SubRegister("gimbal_feedback",sizeof(gimbal_feedback));
	            shoot_cmd_pub = PubRegister("shoot_cmd",sizeof(Robot_shoot_t));
             	ins_feed_sub = SubRegister("ins_feedback",sizeof(IMU_feedback));
//	    #endif
//						
//	#endif
	RS_t->Last_Control_mode = ctrl_err;
	RS_t->Key_control_mode = Key_GYRO_control;
}
void ROBOT_CTRL (Robot_Status *RS_t)
{
	RS_t->Robot_RC = get_remote_control_point();
	RS_t->Robot_KEY = get_key_control_point();
//	Control_Mode_Handler(&Robot_Status_t);
//	Chaais_Message_Handler(&Robot_Status_t,&chassis_cmd_t);
//	Gimbal_Message_Handler(&Robot_Status_t,&gimbal_cmd_t);
	Key_Mode_Handler(&Robot_Status_t);
	Key_Chssis_Message_Handler(&Robot_Status_t,&chassis_cmd_t);
	Key_Gimbal_Message_Handler(&Robot_Status_t,&gimbal_cmd_t);
	Key_Shoot_Message_Handler(&Robot_Status_t);
}

/* --------------------------功能层------------------------- */
//遥控器信息处理，得到控制模式
void Control_Mode_Handler(Robot_Status *RS_t)
{
	//
	switch(RS_t->Robot_RC->S1)
	{
		case 1:
		{
			RS_t->Robot_Control_mode = RC_MECH_control;
		  break;
		}
	  case 2:
		{
			RS_t->Robot_Control_mode = RC_GYRO_control;
		  break;
		}
		case 3:
		{
			RS_t->Robot_Control_mode = RC_SPIN_control;
		  break;
		}
	}
}

//键盘信息处理，得到控制模式
void Key_Mode_Handler(Robot_Status *RS_t)
{
	if(RS_t->Robot_KEY->state_change[Key_SHIFT])
	{
		if(RS_t->Key_control_mode == Key_GYRO_control)
		{
		   RS_t->Key_control_mode = Key_SPIN_control;
		   RS_t->Last_Key_control_mode = Key_SPIN_control;
		}
		else
		{
			RS_t->Key_control_mode = Key_GYRO_control;
			RS_t->Last_Key_control_mode = Key_GYRO_control;
		}
	}
	if(RS_t->Robot_KEY->press_r)
	{
		RS_t->Key_control_mode = Key_AIM_control;//自瞄
	}
	if(RS_t->Key_control_mode == Key_AIM_control)
	{
		RS_t->Key_control_mode = RS_t->Last_Key_control_mode;
	}
}
	

//遥控器信息处理，得到底盘运动信息
void Chaais_Message_Handler(Robot_Status *RS_t,Robot_chassis_t *chassis_t)
{
	//机械模式调速度
	if(RS_t->Robot_Control_mode == RC_MECH_control)
	{
		chassis_t->chassis_control = RC_MECH_control;
		chassis_t->Vx_set = RC_to_Chassis(RS_t->Robot_RC->RC_CH1,20);
		chassis_t->Vy_set = RC_to_Chassis(RS_t->Robot_RC->RC_CH0,20);
		chassis_t->Wz_set = RC_to_Chassis(RS_t->Robot_RC->RC_CH2,40);//到时候调大一点
	}
	//陀螺仪模式
	else if(RS_t->Robot_Control_mode == RC_GYRO_control)
	{
		chassis_t->chassis_control = RC_GYRO_control;
		chassis_t->Vx_set = RC_to_Chassis(RS_t->Robot_RC->RC_CH1,20);
		chassis_t->Vy_set = RC_to_Chassis(RS_t->Robot_RC->RC_CH0,20);
		chassis_t->Wz_set = -offsetAngle()*200;//将底盘与云台的绝对角度差转换为底盘的z轴速度，具体系数有待确定
	}
	//小陀螺模式
	else if(RS_t->Robot_Control_mode == RC_SPIN_control)
	{
		chassis_t->chassis_control = RC_SPIN_control;
		if(RS_t->Robot_RC->RC_CH1 == 0 && RS_t->Robot_RC->RC_CH0 == 0)
		{
			chassis_t->Wz_set = 100;//到时候调大一点
//			chassis_t->Vx_set = RS_t->Robot_RC->RC_CH1 * cos(offsetAngle()) - RS_t->Robot_RC->RC_CH0 * sin(offsetAngle());
//			chassis_t->Vy_set = RS_t->Robot_RC->RC_CH1 * sin(offsetAngle()) + RS_t->Robot_RC->RC_CH0 * cos(offsetAngle());//x,y轴速度分解，速度映射小一点
		}
		else if (RS_t->Robot_RC->RC_CH1 != 0 || RS_t->Robot_RC->RC_CH0 != 0)
		{
			chassis_t->Wz_set = 30;
			chassis_t->Vx_set = (RS_t->Robot_RC->RC_CH1 * cos(offsetAngle()) - RS_t->Robot_RC->RC_CH0 * sin(offsetAngle()))*0.1;
			chassis_t->Vy_set = (RS_t->Robot_RC->RC_CH1 * sin(offsetAngle()) + RS_t->Robot_RC->RC_CH0 * cos(offsetAngle()))*0.1;
		}
	}
}

//遥控器信息处理，得到云台控制信息
void Gimbal_Message_Handler(Robot_Status *RS_t,Robot_gimbal_t *gimbal_t)
{
	switch(RS_t->Robot_Control_mode)
	{
		case RC_MECH_control:
		{
			if(RS_t->Last_Control_mode != RC_MECH_control)
			{
				//Nothing
			}
			gimbal_t->gimbal_control = RC_MECH_control;
			gimbal_t->yaw_speed = offsetAngle()*40;
			RS_t->Last_Control_mode = RC_MECH_control;
			break;
		}
		case RC_GYRO_control:
		{
			if(RS_t->Last_Control_mode != RC_GYRO_control)
			{
				gimbal_t->later_yaw_target_angle = ins_fb_form_ins.YawTotalAngle;
			}
			gimbal_t->gimbal_control = RC_GYRO_control;
			gimbal_t->pitch_target_angle += 0.0003f*RS_t->Robot_RC->RC_CH3;
			gimbal_t->pitch_target_angle = Pitch_Angle(gimbal_t->pitch_target_angle);
			if(RS_t->Robot_RC->RC_CH2>0)
			{
				gimbal_t->later_yaw_target_angle += 0.2;
			}
			else if (RS_t->Robot_RC->RC_CH2<0)
			{
				gimbal_t->later_yaw_target_angle -= 0.2;
			}
			gimbal_t->later_yaw_target_angle = Yaw_Angle(gimbal_t->later_yaw_target_angle);
			
			gimbal_t->pitch_now_angle = ins_fb_form_ins.Pitch;
			gimbal_t->yaw_now_angle = ins_fb_form_ins.YawTotalAngle;
			RS_t->Last_Control_mode = RC_GYRO_control;
			break;
		}
		case RC_SPIN_control:
		{
			if(RS_t->Last_Control_mode != RC_SPIN_control)
			{
				gimbal_t->later_yaw_target_angle = ins_fb_form_ins.YawTotalAngle;
			}
			gimbal_t->gimbal_control = RC_SPIN_control;
			gimbal_t->pitch_target_angle += 0.0003f*RS_t->Robot_RC->RC_CH3;
			gimbal_t->pitch_target_angle = Pitch_Angle(gimbal_t->pitch_target_angle);
			if(RS_t->Robot_RC->RC_CH2>0)
			{
				gimbal_t->later_yaw_target_angle += 0.2;
			}
			else if (RS_t->Robot_RC->RC_CH2<0)
			{
				gimbal_t->later_yaw_target_angle -= 0.2;
			}
			gimbal_t->later_yaw_target_angle = Yaw_Angle(gimbal_t->later_yaw_target_angle);
			
			gimbal_t->pitch_now_angle = ins_fb_form_ins.Pitch;
			gimbal_t->yaw_now_angle = ins_fb_form_ins.YawTotalAngle;
			RS_t->Last_Control_mode = RC_SPIN_control;
			break;
		}
	}
}

//键盘鼠标控制时的信息处理
//Q改变射击状态，SHIFT小陀螺(按下开启，再按下关闭)，鼠标左键射击，右键自瞄，C超级电容，R弹仓开启和关闭，H飞坡模式

void Key_Chssis_Message_Handler (Robot_Status *RS_t,Robot_chassis_t *chassis_t)
{
	int16_t VX,VY;
	switch(RS_t->Key_control_mode)
	{
	  case Key_GYRO_control:
		{
			chassis_t->chassis_control = RC_GYRO_control;
//			if(RS_t->Robot_KEY->Key[Key_W] == push)W
//			{
//				a++;
//				chassis_t->Vx_set = 10;
//			}
//			else
//				chassis_t->Vx_set = 0;
			  chassis_t->Vx_set = (RS_t->Robot_KEY->key[Key_W] - RS_t->Robot_KEY->key[Key_S])*30;
				chassis_t->Vy_set = (RS_t->Robot_KEY->key[Key_D] - RS_t->Robot_KEY->key[Key_A])*30;
	//			chassis_t->Wz_set = -offsetAngle()*40;//到时候调大一点
			if(offsetAngle() < pi/2 && offsetAngle() > -pi/2)
				chassis_t->Wz_set = -offsetAngle()*150;
			else if(offsetAngle() > pi/2 && offsetAngle() < -pi/2)
				chassis_t->Wz_set = offsetAngle()*150;
			    
			break;
	  }
	//陀螺仪模式
	  case Key_SPIN_control:
		{
			chassis_t->chassis_control = RC_SPIN_control;
//			if(RS_t->Robot_KEY->key[Key_W] == 0 && RS_t->Robot_KEY->key[Key_W] == 0)
//			{
//				chassis_t->Wz_set = 200;//到时候调大一点
////				chassis_t->Vx_set = (RS_t->Robot_KEY->key[Key_W] * cos(offsetAngle()) - RS_t->Robot_KEY->key[Key_S] * sin(offsetAngle()))*10;
////				chassis_t->Vy_set = (RS_t->Robot_KEY->key[Key_D] * sin(offsetAngle()) + RS_t->Robot_KEY->key[Key_A] * cos(offsetAngle()))*10;//x,y轴速度分解，速度映射小一点
//			}
//			else if (RS_t->Robot_KEY->key[Key_W] != 0 || RS_t->Robot_KEY->key[Key_W] != 0)
//			{
				chassis_t->Wz_set = 200;
			  VX = RS_t->Robot_KEY->key[Key_W] - RS_t->Robot_KEY->key[Key_S];
			  VY = RS_t->Robot_KEY->key[Key_D] - RS_t->Robot_KEY->key[Key_A];
			if(offsetAngle()<0)
			{
				chassis_t->Vx_set = (VX * cos(fabs(offsetAngle()) - 0.47116626) + VY * sin(fabs(offsetAngle())- 0.47116626))*30;
				chassis_t->Vy_set = (VY * cos(fabs(offsetAngle()) - 0.47116626) - VX * sin(fabs(offsetAngle())- 0.47116626))*30;//x,y轴速度分解，速度映射小一点
			}
			else if(offsetAngle()>0)
			{
				chassis_t->Vx_set = (VX * cos(fabs(offsetAngle()) + 0.47116626) - VY * sin(fabs(offsetAngle())+ 0.47116626))*30;
				chassis_t->Vy_set = (VY * cos(fabs(offsetAngle()) + 0.47116626) + VX * sin(fabs(offsetAngle())+ 0.47116626))*30;//x,y轴速度分解，速度映射小一点
			} (RS_t->Robot_KEY->key[Key_D] * sin(offsetAngle()) + RS_t->Robot_KEY->key[Key_A] * cos(offsetAngle()))*10;//x,y轴速度分解，速度映射小一点
//			}
			break;
	  }
		case Key_AIM_control:
		{
			switch(RS_t->Last_Key_control_mode)
			{
				case Key_GYRO_control:
				{
					chassis_t->chassis_control = RC_GYRO_control;
		//			if(RS_t->Robot_KEY->Key[Key_W] == push)
		//			{
		//				a++;
		//				chassis_t->Vx_set = 10;
		//			}
		//			else
		//				chassis_t->Vx_set = 0;
						chassis_t->Vx_set = (RS_t->Robot_KEY->key[Key_W] - RS_t->Robot_KEY->key[Key_S])*80;
						chassis_t->Vy_set = (RS_t->Robot_KEY->key[Key_D] - RS_t->Robot_KEY->key[Key_A])*80;
			//			chassis_t->Wz_set = -offsetAngle()*40;//到时候调大一点
						chassis_t->Wz_set = -offsetAngle()*200;
					break;
				}
			//陀螺仪模式
				case Key_SPIN_control:
				{
					chassis_t->chassis_control = RC_SPIN_control;
					if(RS_t->Robot_KEY->key[Key_W] == 0 )
					{
						chassis_t->Wz_set = 200;//到时候调大一点
						chassis_t->Vx_set = (RS_t->Robot_KEY->key[Key_W] * cos(offsetAngle()) - RS_t->Robot_KEY->key[Key_S] * sin(offsetAngle()))*10;
						chassis_t->Vy_set = (RS_t->Robot_KEY->key[Key_D] * sin(offsetAngle()) + RS_t->Robot_KEY->key[Key_A] * cos(offsetAngle()))*10;//x,y轴速度分解，速度映射小一点
					}
					else if (RS_t->Robot_KEY->key[Key_W] != 0)
					{
						chassis_t->Wz_set = 10;
						chassis_t->Vx_set = (RS_t->Robot_KEY->key[Key_W] * cos(offsetAngle()) - RS_t->Robot_KEY->key[Key_S] * sin(offsetAngle()))*10;
						chassis_t->Vy_set = (RS_t->Robot_KEY->key[Key_D] * sin(offsetAngle()) + RS_t->Robot_KEY->key[Key_A] * cos(offsetAngle()))*10;//x,y轴速度分解，速度映射小一点
					}
					break;
				}
			}
	  }
	}
}

void Key_Gimbal_Message_Handler (Robot_Status *RS_t,Robot_gimbal_t *gimbal_t)
{
//键盘云台模式控制
	switch(RS_t->Key_control_mode)
	{
		//陀螺仪模式
		case Key_GYRO_control:
		{
			if(RS_t->Last_Key_control_mode != Key_GYRO_control)
			{
				gimbal_t->yaw_target_angle = ins_fb_form_ins.YawTotalAngle;
			}
//			gimbal_t->key_gimbal = Gimbal_GYRO_control;
			gimbal_t->gimbal_control = RC_GYRO_control;
			if(RS_t->Robot_KEY->M_Y>0)
			{
				gimbal_t->pitch_target_angle -= 0.0035f * RS_t->Robot_KEY->M_Y;
			}
			else if (RS_t->Robot_KEY->M_Y<0)
			{
				gimbal_t->pitch_target_angle -= 0.0035f * RS_t->Robot_KEY->M_Y;
			}
			gimbal_t->pitch_target_angle = Pitch_Angle(gimbal_t->pitch_target_angle);
			if(RS_t->Robot_KEY->M_X < 0)
			{
				gimbal_t->yaw_target_angle -= 0.004f * RS_t->Robot_KEY->M_X;
			}
			else if (RS_t->Robot_KEY->M_X > 0) 
			{
				gimbal_t->yaw_target_angle -= 0.004f * RS_t->Robot_KEY->M_X;
			}
			
			if(RS_t->Robot_KEY->state_change[Key_X])
			{
				gimbal_t->yaw_target_angle +=90.0f/7;
			}
			gimbal_t->yaw_target_angle = Yaw_Angle(gimbal_t->yaw_target_angle);
			if (gimbal_t->yaw_target_angle - gimbal_t->last_yaw_target_angle > 180.0f)
			{
				gimbal_t->ControlRoundCount--;
			}
			else if (gimbal_t->yaw_target_angle - gimbal_t->last_yaw_target_angle < -180.0f)
			{
				gimbal_t->ControlRoundCount++;
			}
			gimbal_t->total_yaw_target_angle = 360.0f * gimbal_t->ControlRoundCount + gimbal_t->yaw_target_angle;
			gimbal_t->last_yaw_target_angle = gimbal_t->yaw_target_angle;
			
			while(gimbal_t->total_yaw_target_angle>360.0f)
					gimbal_t->total_yaw_target_angle = gimbal_t->total_yaw_target_angle - 360.0f;
			while(gimbal_t->total_yaw_target_angle<-360.0f)
					gimbal_t->total_yaw_target_angle = gimbal_t->total_yaw_target_angle + 360.0f;
			
			gimbal_t->later_yaw_target_angle = gimbal_t->total_yaw_target_angle;
			
			gimbal_t->pitch_now_angle = ins_fb_form_ins.Pitch;
			gimbal_t->yaw_now_angle = ins_fb_form_ins.YawTotalAngle;
			RS_t->Last_Key_control_mode = Key_GYRO_control;
			break;
		}
		//小陀螺模式
		case Key_SPIN_control:
		{
			if(RS_t->Last_Key_control_mode != Key_SPIN_control)
			{
				gimbal_t->yaw_target_angle = ins_fb_form_ins.YawTotalAngle;
			}
//			gimbal_t->key_gimbal = Gimbal_SPIN_control;
			gimbal_t->gimbal_control = RC_SPIN_control;
				if(RS_t->Robot_KEY->M_Y>0)
			{
				gimbal_t->pitch_target_angle -= 0.0035f * RS_t->Robot_KEY->M_Y;
			}
			else if (RS_t->Robot_KEY->M_Y<0)
			{
				gimbal_t->pitch_target_angle -= 0.0035f * RS_t->Robot_KEY->M_Y;
			}
			gimbal_t->pitch_target_angle = Pitch_Angle(gimbal_t->pitch_target_angle);
			if(RS_t->Robot_KEY->M_X < 0)
			{
				gimbal_t->yaw_target_angle -= 0.004f * RS_t->Robot_KEY->M_X;
			}
			else if (RS_t->Robot_KEY->M_X > 0) 
			{
				gimbal_t->yaw_target_angle -= 0.004f * RS_t->Robot_KEY->M_X;
			}
			
			if(RS_t->Robot_KEY->state_change[Key_X])
			{
				gimbal_t->yaw_target_angle +=90.0f/7;
			}
			gimbal_t->yaw_target_angle = Yaw_Angle(gimbal_t->yaw_target_angle);
			if (gimbal_t->yaw_target_angle - gimbal_t->last_yaw_target_angle > 180.0f)
			{
				gimbal_t->ControlRoundCount--;
			}
			else if (gimbal_t->yaw_target_angle - gimbal_t->last_yaw_target_angle < -180.0f)
			{
				gimbal_t->ControlRoundCount++;
			}
			gimbal_t->total_yaw_target_angle = 360.0f * gimbal_t->ControlRoundCount + gimbal_t->yaw_target_angle;
			gimbal_t->last_yaw_target_angle = gimbal_t->yaw_target_angle;
			
			while(gimbal_t->total_yaw_target_angle>360.0f)
					gimbal_t->total_yaw_target_angle = gimbal_t->total_yaw_target_angle - 360.0f;
			while(gimbal_t->total_yaw_target_angle<-360.0f)
					gimbal_t->total_yaw_target_angle = gimbal_t->total_yaw_target_angle + 360.0f;
			
			gimbal_t->later_yaw_target_angle = gimbal_t->total_yaw_target_angle;
			
			gimbal_t->pitch_now_angle = ins_fb_form_ins.Pitch;
			gimbal_t->yaw_now_angle = ins_fb_form_ins.YawTotalAngle;
			RS_t->Last_Key_control_mode = Key_SPIN_control;
			break;
		}
//		//自瞄模式
//		case Key_AIM_control:
//		{
//			if(RS_t->Last_Key_control_mode != Key_AIM_control)
//			{
//				gimbal_t->yaw_target_angle = ins_fb_form_ins.YawTotalAngle;
//			}
//			gimbal_t->key_gimbal = Gimbal_AIM_control;
//			gimbal_t->pitch_target_angle += pc_fb_from_pc.pitch;
//			gimbal_t->pitch_target_angle = Pitch_Angle(gimbal_t->pitch_target_angle);
//			gimbal_t->yaw_target_angle += pc_fb_from_pc.yaw;
//			gimbal_t->yaw_target_angle = Yaw_Angle(gimbal_t->yaw_target_angle);
//			
//			gimbal_t->pitch_now_angle = ins_fb_form_ins.Pitch;
//			gimbal_t->yaw_now_angle = ins_fb_form_ins.YawTotalAngle;
//			RS_t->Last_Key_control_mode = Key_AIM_control;
//			break;
//		}
		case key_err:
		{
//			gimbal_t->key_gimbal = Gimbal_err;
			gimbal_t->gimbal_control = ctrl_err;
			RS_t->Last_Key_control_mode = key_err;
		}
	}
}



//键盘发射控制
void Key_Shoot_Message_Handler (Robot_Status *RS_t)
{
	last_Q=RS_t->Robot_KEY->key[Key_Q];
//	if(RS_t->Robot_KEY->state_change[Key_r])
//	{
//		Magazine(&shoot_cmd_t);//关闭弹仓
//	}
//    if(RS_t->Robot_KEY->state_change[Key_G])
//	  {		
//		  
//		    if( shoot_cmd_t.Last_Shoot_mode != Shoot_Stop)
//		    {
//			 shoot_cmd_t.Shoot_mode = Shoot_Stop;
//	  	     shoot_cmd_t.fric1_speed = 0;
//	  		 shoot_cmd_t.fric2_speed = 0;	
//			}
//			else
//			 shoot_cmd_t.Shoot_mode =  shoot_cmd_t.Last_Shoot_mode;
//		     shoot_cmd_t.fric1_speed = FRIC_SPEED1;
//		     shoot_cmd_t.fric2_speed = FRIC_SPEED2;
//				
//			
//	  		
//	  
//	  }
//	if(RS_t->Robot_KEY->state_change[Key_Q])
//	{
//		Cuttinggun(&shoot_cmd_t);//切换发射模式（切枪）	
//		Q++;
//		if(Q==2)
//			Q=1;
//	}
//	if(RS_t->Robot_KEY->mouse_state_change[0])
//	{
		shoot_cmd_t.flag=1;
		Shoot(&shoot_cmd_t);		
//	}
//	else
//	{
//		shoot_cmd_t.Ready_Flag = 1;
//	}
}

//开启弹仓或关闭弹仓
void Magazine (Robot_shoot_t *shoot_cmd_t)
{
	if (shoot_cmd_t->mag_mode == CLOSE)
	    shoot_cmd_t->mag_mode = OPEN;
	else
		  shoot_cmd_t->mag_mode = CLOSE;
}


//切换射击状态（第一把单发，第二把双发，第三把连发，第四次按关闭射击状态机）
void Cuttinggun(Robot_shoot_t *shoot_cmd_t)
{

//	switch()
//  {
//		case 1: shoot_cmd_t->Shoot_mode = Ready_Shoot ;break;
////		case 2: shoot_cmd_t->Ready_Flag = 1,shoot_cmd_t->fric1_speed = FRIC_SPEED1,shoot_cmd_t->fric2_speed = FRIC_SPEED2;break;
////		case 3: shoot_cmd_t->Ready_Flag = 1,shoot_cmd_t->fric1_speed = FRIC_SPEED1,shoot_cmd_t->fric2_speed = FRIC_SPEED2;break;
//	}		
	
}


////鼠标控制发射
void Shoot(Robot_shoot_t *shoot_cmd_t)
{

//	  if(Robot_Status_t.Robot_KEY->state_change[Key_R])		//Single_Shoot_State
//	  {
//			shoot_cmd_t->Shoot_mode = Single_Shoot;
//	  }
	  static uint8_t last_press_l = 0;
//      if (Robot_Status_t.Robot_KEY->press_l == 1 && last_press_l == 0&&shoot_cmd_t->Shoot_mode == Single_Shoot)
//      {
//   					
////   	shoot_cmd_t->Shoot_mode = Single_Shoot;
//   		shoot_cmd_t->fric1_speed = FRIC_SPEED1;
//   		shoot_cmd_t->fric2_speed = FRIC_SPEED2;
//   		shoot_cmd_t->angle_set -= 1804.816281f;
//   
//   	  }
//   	  else 
//      {	
//   	    shoot_cmd_t->Shoot_mode = Ready_Shoot ;
//        shoot_cmd_t->fric1_speed = FRIC_SPEED1;
//        shoot_cmd_t->fric2_speed = FRIC_SPEED2;
//        shoot_cmd_t->Last_Shoot_mode = Ready_Shoot;
//     	
//       }

//   	last_press_l = Robot_Status_t.Robot_KEY->press_l;
//	   if(Robot_Status_t.Robot_KEY->state_change[Key_F])		//Continuous_Shoot_State
//	  {
//		  
//		    shoot_cmd_t->Shoot_mode = Continuous_Shoot;
//		 
//	  }
  
	   if((Robot_Status_t.Robot_KEY->press_l))
		  {
 
		    shoot_cmd_t->Last_Shoot_mode = Continuous_Shoot;
		    shoot_cmd_t->Shoot_mode = Continuous_Shoot; 
		    shoot_cmd_t->fric1_speed = -FRIC_SPEED1;
		    shoot_cmd_t->fric2_speed = -FRIC_SPEED2;
			shoot_cmd_t->trigger_speed = -CONTINUOUS_TRIGGER_SPEED;
		  }
		  else 
	      {	
			shoot_cmd_t->Shoot_mode = Ready_Shoot ;
            shoot_cmd_t->fric1_speed = -FRIC_SPEED1;
		    shoot_cmd_t->fric2_speed = -FRIC_SPEED2;
		    shoot_cmd_t->Last_Shoot_mode = Ready_Shoot;
	  		
	      }
}
//	  

	  

//	if(shoot_cmd_t->Shoot_mode == Single_Shoot)
//	{
//		if(Robot_Status_t.Robot_KEY->mouse_state_change[0] == 1)//按下0->1
//		{
//				shoot_cmd_t->angle_set = -31.5;
//		//		shoot_cmd_t->angle_set+=1;
//	
//		}
//		else if(Robot_Status_t.Robot_KEY->mouse_state_change[0] == 0)//松开1->0
//		{
//				shoot_cmd_t->angle_set = 0;
//		}
//		
//	}
//	else if(shoot_cmd_t->Shoot_mode == Double_Shoot)
//	{
//		
////		shoot_cmd_t->angle_set+=49.0;
//		if(Robot_Status_t.Robot_KEY->mouse_state_change[0] == 1)//按下0->1
//		{
//				shoot_cmd_t->angle_set = -63;
//		//		shoot_cmd_t->angle_set+=1;
//		}
//		else if(Robot_Status_t.Robot_KEY->mouse_state_change[0] == 0)//松开1->0
//		{
//				shoot_cmd_t->angle_set = 0;
//		}
//	}


//将遥控器的数据转化为底盘速度数据
int16_t RC_to_Chassis (int16_t RC_data,int16_t XianFu)
{
	return XianFu*RC_data/660;
}

//计算底盘与云台的角度差（无需改动）
double offsetAngle (void)
{
	double Angle;
	int16_t diff;
	diff = gimbal_cmd_t.yaw_motor_mid - gimbai_fb_from_gimbal.yaw_angle;
	if(diff<10 && diff >0)
		diff = 0;
	else if(diff>-10 && diff<0)
		diff =0;
	if(diff > 4096)
		diff = -(8192-diff);
	else if(diff < -4096)
		diff = 8192+diff;
	Angle = diff*2*pi/8192;
	return Angle;
}

////计算云台电机的目标值（过零点）
//uint16_t Handle_Angle8191_PID_Over_Zero(int16_t tar, uint16_t cur)
//{
//	if(tar - cur > 4096)   
//	{
//		return (cur+8191);        
//	}
//	else if(tar - cur < -4096)
//	{
//		return (cur-8191);
//	}
//	else
//	{
//		return cur;
//	}
//}

//pitch轴角度处理
double Yaw_Angle (double angle)
{
	if(angle>360.0f)
		{
			return angle-360;
		}
	else if (angle<-360.0f)
		return angle+360;
	else 
		return angle;
}

//yaw轴角度处理
double Pitch_Angle (double angle)
{
	if(angle>45.0f)
		{
			return 45;
		}
	else if (angle<-45.0f)
		return -45;
	else 
		return angle;
}
