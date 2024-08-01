#include "control.h"
#include "itrt.h"
#include "motor.h"
#include "pid.h"


int16_t Get_Encoder()
{
	
	int16_t EncoderA = Velocity_A(70,encoderA_cnt);
	int16_t EncoderB = Velocity_B(70,encoderB_cnt);
	
	int16_t Encoder = (int16_t)((EncoderA+EncoderB)/2);

	return Encoder;
}


void Speed(int16_t EncoderA,int16_t EncoderB)
{
	EncoderA = Velocity_A(70,encoderA_cnt);
	EncoderB = Velocity_B(70,encoderB_cnt);
	
	int16_t speed_l = 9 * EncoderA;
	int16_t speed_r = 9 * EncoderB;
	
	Set_PWM(speed_l,speed_r);
}


void Turn_left(int16_t Aver_Encoder,int16_t v)
{
	Aver_Encoder = Get_Encoder();
	
	int16_t Speed_R= Aver_Encoder + v;
	
	Set_PWM(9*Aver_Encoder,9*Speed_R); 
	
	
}


void Turn_right(int16_t Aver_Encoder,int16_t v)
{
	Aver_Encoder = Get_Encoder();
	
	int16_t Speed_L= Aver_Encoder + v;
	
	Set_PWM(9*Speed_L,9*Aver_Encoder); 
	
}