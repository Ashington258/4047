#ifndef itrt_h
#define itrt_h

#include "board.h"

extern int32_t Get_Encoder_countA,encoderA_cnt,PWMA,Get_Encoder_countB,encoderB_cnt,PWMB;
extern uint8_t Key_Num;
extern uint8_t turn_flag;

typedef struct{
	
	int encoder_l;
	int encoder_r;
	float gyro_turn_output;
	float angle_turn_output;
	float error_yaw;
	
	int target_speed;
	
	uint8_t stop;
	
} control_struc;

extern control_struc Yao;







#endif