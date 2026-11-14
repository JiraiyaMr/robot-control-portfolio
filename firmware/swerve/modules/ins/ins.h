/**
 ******************************************************************************
 * @file    ins_task.h
 * @author  Wang Hongxi
 * @version V2.0.0
 * @date    2022/2/23
 * @brief
 ******************************************************************************
 * @attention
 *
 ******************************************************************************
 */
#ifndef __INS_H
#define __INS_H

#include "stdint.h"
#include "BMI088driver.h"
#include "MahonyAHRS.h"
#include "pub_sub.h"
#include "QuaternionEKF.h"
#include  "bsp_dwt.h"

#define X 0
#define Y 1
#define Z 2

#define INS_TASK_PERIOD 1

typedef struct
{
    float q[4]; 

    float Gyro[3];  
    float Accel[3]; 
    float MotionAccel_b[3]; 
    float MotionAccel_n[3];

    float AccelLPF; 
	
    float xn[3];
    float yn[3];
    float zn[3];

    float atanxz;
    float atanyz;

    float Roll;
    float Pitch;
    float Yaw;
    float YawTotalAngle;
} INS_t;


/**
 * @brief 
 * 
 */
typedef struct
{
    uint8_t flag;

    float scale[3];

    float Yaw;
    float Pitch;
    float Roll;
} IMU_Param_t;

typedef struct
{
	float Yaw;
  float Pitch;
  float Roll;
	float YawTotalAngle;
}IMU_feedback;

void ins_task (void const * argument);

void Data_Assign (INS_t *ins);

void get_angle(float q[4], INS_t *ins);

//extern INS_t INS;

//void INS_Init(void);
//void INS_Task(void);
//void IMU_Temperature_Ctrl(void);

//void QuaternionUpdate(float *q, float gx, float gy, float gz, float dt);
//void QuaternionToEularAngle(float *q, float *Yaw, float *Pitch, float *Roll);
//void EularAngleToQuaternion(float Yaw, float Pitch, float Roll, float *q);
void BodyFrameToEarthFrame(const float *vecBF, float *vecEF, float *q);
void EarthFrameToBodyFrame(const float *vecEF, float *vecBF, float *q);

#endif
