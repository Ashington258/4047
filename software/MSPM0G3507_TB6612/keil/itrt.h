#ifndef itrt_h
#define itrt_h

#include "stdio.h"
#include "ti_msp_dl_config.h"

extern int32_t Get_Encoder_countA,encoderA_cnt,PWMA,Get_Encoder_countB,encoderB_cnt,PWMB;
extern uint8_t Key_Num;

typedef struct{
	
	int encoder_l;
	int encoder_r;
	float gyro_turn_output;
	float angle_turn_output;
	float error_yaw;
} control_struc;

extern control_struc Yao;







#endif