#include "ins.h"

IMU_feedback IMU_fb;

INS_t INS;

static float dt = 0, t = 0;
Publisher_t *ins_feedback;
uint32_t INS_DWT_Count = 0;
const float xb[3] = {1, 0, 0};
const float yb[3] = {0, 1, 0};
const float zb[3] = {0, 0, 1};

void ins_task (void const * argument)
{
//	INS_Init();
	ins_feedback = PubRegister("ins_feedback",sizeof(IMU_feedback));
	float INS_quat[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  const float gravity[3] = {0, 0, 9.81f};
  dt = DWT_GetDeltaT(&INS_DWT_Count);
   t += dt;
	for(;;)
	{
		BMI088_Read(&BMI088);
		Data_Assign(&INS);
//		MahonyAHRSupdateIMU(INS_quat,ins.Gyro[X],ins.Gyro[Y],ins.Gyro[Z],ins.Accel[X],ins.Accel[Y],ins.Accel[Z]);
//		get_angle(INS_quat,&ins);
////		IMU_QuaternionEKF_Update(ins.Gyro[X], ins.Gyro[Y], ins.Gyro[Z], ins.Accel[X], ins.Accel[Y], ins.Accel[Z], dt);
//		IMU_fb.Yaw = ins.Yaw;
//		IMU_fb.Pitch = ins.Pitch;
//		IMU_fb.Roll = ins.Roll;
//		IMU_fb.YawTotalAngle = ins.YawTotalAngle;
    
    // 核心函数,EKF更新四元数
    IMU_QuaternionEKF_Update(INS.Gyro[X], INS.Gyro[Y], INS.Gyro[Z], INS.Accel[X], INS.Accel[Y], INS.Accel[Z], dt);

    memcpy(INS.q, QEKF_INS.q, sizeof(QEKF_INS.q));
		PubPushMessage(ins_feedback,(void *)&IMU_fb);


        // 将重力从导航坐标系n转化到机体系b，随后根据加速度计数据计算运动加速度
        float gravity_b[3];
        
        // 机体系基向量转换到导航坐标系，本例选取惯性系为导航系
        BodyFrameToEarthFrame(xb, INS.xn, INS.q);
        BodyFrameToEarthFrame(yb, INS.yn, INS.q);
        BodyFrameToEarthFrame(zb, INS.zn, INS.q);
        EarthFrameToBodyFrame(gravity, gravity_b, INS.q);
        for (uint8_t i = 0; i < 3; i++) // 同样过一个低通滤波
        {
            INS.MotionAccel_b[i] = (INS.Accel[i] - gravity_b[i]) * dt / (INS.AccelLPF + dt) + INS.MotionAccel_b[i] * INS.AccelLPF / (INS.AccelLPF + dt);
        }
        BodyFrameToEarthFrame(INS.MotionAccel_b, INS.MotionAccel_n, INS.q); // 转化回导航系n

        // 获取最终数据
        INS.Yaw = QEKF_INS.Yaw;
        INS.Pitch = QEKF_INS.Pitch;
        INS.Roll = QEKF_INS.Roll;
        INS.YawTotalAngle = QEKF_INS.YawTotalAngle;
		osDelay(2);
	}
}

//获得陀螺仪数据
void Data_Assign (INS_t *ins)
{
	ins->Accel[X] = BMI088.Accel[X];
  ins->Accel[Y] = BMI088.Accel[Y];
  ins->Accel[Z] = BMI088.Accel[Z];
  ins->Gyro[X] = BMI088.Gyro[X];
  ins->Gyro[Y] = BMI088.Gyro[Y];
  ins->Gyro[Z] = BMI088.Gyro[Z];
}

void get_angle(float q[4], INS_t *ins)
{
    ins->Yaw = atan2f(2.0f*(q[0]*q[3]+q[1]*q[2]), 2.0f*(q[0]*q[0]+q[1]*q[1])-1.0f);
    ins->Pitch = asinf(-2.0f*(q[1]*q[3]-q[0]*q[2]));
    ins->Roll = atan2f(2.0f*(q[0]*q[1]+q[2]*q[3]),2.0f*(q[0]*q[0]+q[3]*q[3])-1.0f);
}

/**
 * @brief          Transform 3dvector from BodyFrame to EarthFrame
 * @param[1]       vector in BodyFrame
 * @param[2]       vector in EarthFrame
 * @param[3]       quaternion
 */

void BodyFrameToEarthFrame(const float *vecBF, float *vecEF, float *q)
{
    vecEF[0] = 2.0f * ((0.5f - q[2] * q[2] - q[3] * q[3]) * vecBF[0] +
                       (q[1] * q[2] - q[0] * q[3]) * vecBF[1] +
                       (q[1] * q[3] + q[0] * q[2]) * vecBF[2]);

    vecEF[1] = 2.0f * ((q[1] * q[2] + q[0] * q[3]) * vecBF[0] +
                       (0.5f - q[1] * q[1] - q[3] * q[3]) * vecBF[1] +
                       (q[2] * q[3] - q[0] * q[1]) * vecBF[2]);

    vecEF[2] = 2.0f * ((q[1] * q[3] - q[0] * q[2]) * vecBF[0] +
                       (q[2] * q[3] + q[0] * q[1]) * vecBF[1] +
                       (0.5f - q[1] * q[1] - q[2] * q[2]) * vecBF[2]);
}

/**
 * @brief          Transform 3dvector from EarthFrame to BodyFrame
 * @param[1]       vector in EarthFrame
 * @param[2]       vector in BodyFrame
 * @param[3]       quaternion
 */
void EarthFrameToBodyFrame(const float *vecEF, float *vecBF, float *q)
{
    vecBF[0] = 2.0f * ((0.5f - q[2] * q[2] - q[3] * q[3]) * vecEF[0] +
                       (q[1] * q[2] + q[0] * q[3]) * vecEF[1] +
                       (q[1] * q[3] - q[0] * q[2]) * vecEF[2]);

    vecBF[1] = 2.0f * ((q[1] * q[2] - q[0] * q[3]) * vecEF[0] +
                       (0.5f - q[1] * q[1] - q[3] * q[3]) * vecEF[1] +
                       (q[2] * q[3] + q[0] * q[1]) * vecEF[2]);

    vecBF[2] = 2.0f * ((q[1] * q[3] + q[0] * q[2]) * vecEF[0] +
                       (q[2] * q[3] - q[0] * q[1]) * vecEF[1] +
                       (0.5f - q[1] * q[1] - q[2] * q[2]) * vecEF[2]);
}


