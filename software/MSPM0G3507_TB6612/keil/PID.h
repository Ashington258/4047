#ifndef __PID_H
#define __PID_H

#include "ti_msp_dl_config.h"
#include "board.h"

typedef struct
{
   	float kp,ki,kd;//三个系数
    float error,lastError;//误差、上次误差
    float integral,maxIntegral;//积分、积分限幅
    float output,maxOutput;//输出、输出限幅
}PID;

extern PID PID_Controller;//修改参数请在empty.c修改

extern float Velcity_Kp,Velcity_Ki,Velcity_Kd; //相关速度PID参数

int Velocity_A(int TargetVelocity, int CurrentVelocity);
int Velocity_B(int TargetVelocity, int CurrentVelocity);


void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);
void PID_Calc(PID *pid,float reference,float feedback);

#endif

