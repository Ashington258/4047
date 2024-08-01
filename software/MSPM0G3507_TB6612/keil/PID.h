#ifndef __PID_H
#define __PID_H

#include "ti_msp_dl_config.h"
#include "board.h"


extern float Velcity_Kp,Velcity_Ki,Velcity_Kd; //����ٶ�PID����

int Velocity_A(int TargetVelocity, int CurrentVelocity);
int Velocity_B(int TargetVelocity, int CurrentVelocity);


#endif

