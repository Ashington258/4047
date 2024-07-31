#ifndef __PID_H
#define __PID_H

#include "ti_msp_dl_config.h"
#include "board.h"

float Velcity_Kp=1.0,  Velcity_Ki=0.5,  Velcity_Kd; //相关速度PID参数

int Velocity_A(int TargetVelocity, int CurrentVelocity);
int Velocity_B(int TargetVelocity, int CurrentVelocity);

#endif
