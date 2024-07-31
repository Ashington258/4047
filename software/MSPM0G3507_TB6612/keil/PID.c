#include "PID.h"

PID_ERECT s_pid;

//GPS转向
float erect_gyro_Turn[4]    = { 70 , 0  , 0  , 0   };
float erect_angle_Turn[4]     = { 3.3 ,  0.003,  60  , 1  };      //3.7


float Cascade_gro_T( PID_INFO *pid_info , float * PID_Parm , float NowPoint , float SetPoint )
{
    float G_out;

    pid_info->iError = ( NowPoint - SetPoint );
    pid_info->SumError += pid_info->iError;

    if( PID_Parm[3] )
    {
        pid_info->SumError = limit( pid_info->SumError , PID_Parm[3] );
    }
    G_out = PID_Parm[KP] * pid_info->iError +
            PID_Parm[KI] * pid_info->SumError +
            PID_Parm[KD] * ( pid_info->iError - pid_info->LastError );
    pid_info->LastError = pid_info->iError;

    return G_out;
}
float Cascade_angle_T( PID_INFO *pid_info , float * PID_Parm , float NowPoint , float SetPoint )
{
    float G_out;

    pid_info->iError = ( NowPoint - SetPoint );
    pid_info->SumError += pid_info->iError;

    if( PID_Parm[3] )
    {
        pid_info->SumError = limit( pid_info->SumError , PID_Parm[3] );
    }
    G_out = PID_Parm[KP] * pid_info->iError +
            PID_Parm[KI] * pid_info->SumError +
            PID_Parm[KD] * ( pid_info->iError - pid_info->LastError );
    pid_info->LastError = pid_info->iError;

    return G_out;
}


/*****************---------PID参数初始化---------*****************/
void pid_para_init( PID_INFO *pid_info )
{
    pid_info->iError = 0;
    pid_info->SumError = 0;
    pid_info->PrevError = 0;
    pid_info->LastError = 0;
    pid_info->LastData = 0;
}
