#include "VOFA.h"
#include "main.h"
#include "usart.h"
#include "shoot.h"
#include "stdio.h"
#include "gimbal.h"
#include "chassis.h"
#include "shoot.h"

extern gimbal_move_t gimbal_t;
extern chassis_move_t chassis_t;
extern shoot_control_t shoot_control;
extern IMU_feedback ins_fb_form_ins;

uint8_t DataBuff[16];//指令内容

void UART1_Init(void)
{
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);  //receive interrupt
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  //idle interrupt
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,DataBuff,16);
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart == &huart1)
	{
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1,DataBuff,16);
		USART1_Update();//数据解析和参数赋值函数
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{	
	__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);  //receive interrupt
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);  //idle interru
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1,DataBuff,16);
}

/*
 * 解析出DataBuff中的数据
 * 返回解析得到的数据
 */
float Get_Data(void)
{
	float temp =  	(DataBuff[3]-0x30)*10000+
					(DataBuff[4]-0x30)*1000+
					(DataBuff[5]-0x30)*100+
					(DataBuff[6]-0x30)*10+
					(DataBuff[7]-0x30)*1+
					(DataBuff[9]-0x30)*0.1f+
					(DataBuff[10]-0x30)*0.01f+
					(DataBuff[11]-0x30)*0.001f;
	if(DataBuff[2] == '-') temp = -temp;
	return temp;
}

/*
 * 根据串口信息进行PID调参
 */

void USART1_Update(void)
{ 
	float data_Get = Get_Data(); // 存放接收到的数据
	
	if	(DataBuff[0]=='1' && DataBuff[1]=='P') 
	{
		gimbal_t.gimbal_yaw_angle_pid.kp = data_Get;
	} 
	else if	(DataBuff[0]=='1' && DataBuff[1]=='I') 
	{
		gimbal_t.gimbal_yaw_angle_pid.ki = data_Get;
	} 
	else if	(DataBuff[0]=='1' && DataBuff[1]=='D') 
	{
		gimbal_t.gimbal_yaw_angle_pid.kd = data_Get;
	} 
	else if	(DataBuff[0]=='1' && DataBuff[1]=='E') 
	{
		gimbal_t.gimbal_yaw_angle_pid.i_max = data_Get;
	} 
	
	
	else if	(DataBuff[0]=='2' && DataBuff[1]=='P') 
	{
		gimbal_t.gimbal_yaw_speed_pid.kp = data_Get;
	} 
	else if	(DataBuff[0]=='2' && DataBuff[1]=='I') 
	{
		gimbal_t.gimbal_yaw_speed_pid.ki = data_Get;
	} 
	else if	(DataBuff[0]=='2' && DataBuff[1]=='D') 
	{
		gimbal_t.gimbal_yaw_speed_pid.kd = data_Get;
	} 
	else if	(DataBuff[0]=='2' && DataBuff[1]=='E') 
	{
		gimbal_t.gimbal_yaw_speed_pid.i_max = data_Get;
	} 


//    float data_Get = Get_Data(); // 存放接收到的数据
//	
//	if	(DataBuff[0]=='1' && DataBuff[1]=='P') 
//	{
//		for(uint8_t i=0;i<4;i++) {chassis_t.chassis_speed_pid[i].kp = data_Get;}
//	} 
//	else if	(DataBuff[0]=='1' && DataBuff[1]=='I') 
//	{
//		for(uint8_t i=0;i<4;i++) {chassis_t.chassis_speed_pid[i].ki = data_Get;}
//	} 
//	else if	(DataBuff[0]=='1' && DataBuff[1]=='D') 
//	{
//		for(uint8_t i=0;i<4;i++) {chassis_t.chassis_speed_pid[i].kd = data_Get;}
//	} 
//	else if	(DataBuff[0]=='1' && DataBuff[1]=='E') 
//	{
//		for(uint8_t i=0;i<4;i++) {chassis_t.chassis_speed_pid[i].i_max = data_Get;}
//	} 
}

/*
 * 根据串口信息进行PID调参
 */
int fputc(int ch, FILE *f)
{      
	uint8_t tx[1] = {(uint8_t)ch};
	HAL_UART_Transmit(&huart1,tx,1,1000);  
	return ch;
}
