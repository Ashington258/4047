#include "stdint.h"

#ifndef _FLY_MOTOR_PID_h
#define _FLY_MOTOR_PID_h

#define limit(a,b)     (a > b)?  b : ((a < -b)? -b : a)

#define KP 0
#define KI 1
#define KD 2
/*****************---------结构体---------*****************/
typedef struct
{
    float iError;                 // 误差
    float LastError;              // 上次误差
    float PrevError;              // 上上次误差
    float LastData;               // 上次数据
    float iErrorHistory[5];       // 历史误差
    float SumError;               // 累计误差
} PID_INFO;

typedef struct
{

    PID_INFO T_gyro_pid;                          // 动量轮串级 角速度（转向）
    PID_INFO T_angle_pid;                    // 动量轮串级 GPS角度（转向）

} PID_ERECT;

extern PID_ERECT s_pid;                 //PID

/*****************---------结构体---------*****************/


/*****************---------PID参数---------*****************/

extern float erect_yawan[4];                  //开环压弯


// 串级PID转向
extern float erect_gyro_Turn[4];
extern float erect_angle_Turn[4];

/*****************---------PID参数---------*****************/


/*****************---------函数---------*****************/

// 串级PID转向
float Cascade_gro_T(PID_INFO *pid_info, float * PID_Parm, float NowPoint, float SetPoint);
float Cascade_angle_T( PID_INFO *pid_info , float * PID_Parm , float NowPoint , float SetPoint );

// 参数初始化
void pid_para_init(PID_INFO *pid_info);

/*****************---------函数---------*****************/

#endif
