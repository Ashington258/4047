#include "stdint.h"

#ifndef _FLY_MOTOR_PID_h
#define _FLY_MOTOR_PID_h

#define limit(a,b)     (a > b)?  b : ((a < -b)? -b : a)

#define KP 0
#define KI 1
#define KD 2
/*****************---------�ṹ��---------*****************/
typedef struct
{
    float iError;                 // ���
    float LastError;              // �ϴ����
    float PrevError;              // ���ϴ����
    float LastData;               // �ϴ�����
    float iErrorHistory[5];       // ��ʷ���
    float SumError;               // �ۼ����
} PID_INFO;

typedef struct
{

    PID_INFO T_gyro_pid;                          // �����ִ��� ���ٶȣ�ת��
    PID_INFO T_angle_pid;                    // �����ִ��� GPS�Ƕȣ�ת��

} PID_ERECT;

extern PID_ERECT s_pid;                 //PID

/*****************---------�ṹ��---------*****************/


/*****************---------PID����---------*****************/

extern float erect_yawan[4];                  //����ѹ��


// ����PIDת��
extern float erect_gyro_Turn[4];
extern float erect_angle_Turn[4];

/*****************---------PID����---------*****************/


/*****************---------����---------*****************/

// ����PIDת��
float Cascade_gro_T(PID_INFO *pid_info, float * PID_Parm, float NowPoint, float SetPoint);
float Cascade_angle_T( PID_INFO *pid_info , float * PID_Parm , float NowPoint , float SetPoint );

// ������ʼ��
void pid_para_init(PID_INFO *pid_info);

/*****************---------����---------*****************/

#endif
