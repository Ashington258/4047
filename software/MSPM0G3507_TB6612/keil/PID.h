#ifndef __PID_H
#define __PID_H

#include "ti_msp_dl_config.h"
#include "board.h"

typedef struct
{
   	float kp,ki,kd;//����ϵ��
    float error,lastError;//���ϴ����
    float integral,maxIntegral;//���֡������޷�
    float output,maxOutput;//���������޷�
}PID;

extern PID PID_Controller;//�޸Ĳ�������empty.c�޸�

extern float Velcity_Kp,Velcity_Ki,Velcity_Kd; //����ٶ�PID����

int Velocity_A(int TargetVelocity, int CurrentVelocity);
int Velocity_B(int TargetVelocity, int CurrentVelocity);


void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut);
void PID_Calc(PID *pid,float reference,float feedback);

#endif

