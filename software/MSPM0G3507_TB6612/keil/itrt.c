#include "board.h"
#include "key.h"
#include "led.h"
#include "motor.h"
#include "itrt.h"
#include "PID.h"
#include "wit_imu.h"
#include "ins.h"

int32_t Get_Encoder_countA, encoderA_cnt, PWMA, Get_Encoder_countB, encoderB_cnt, PWMB;
uint8_t Key_Num = 0;

control_struc Yao;

uint8_t flag_10ms = 0;
uint8_t flag_15ms = 0;
uint8_t flag_20ms = 0;
uint8_t flag_40ms = 0;
uint8_t flag_60ms = 0;
uint8_t flag_80ms = 0;
uint16_t count = 0;

float kp_ = 0.001;
float ki_ = 0;

void TIMER_0_INST_IRQHandler(void) // 5ms
{
	if (DL_TimerA_getPendingInterrupt(TIMER_0_INST))
	{
		if (DL_TIMER_IIDX_ZERO)
		{
			flag_10ms++;
			flag_15ms++;
			flag_20ms++;
			flag_40ms++;
			
			
			ins_navigation(20);

			
//			Yao.gyro_turn_output = -Cascade_gro_T( &s_pid.T_gyro_pid, erect_gyro_Turn, imuData.gyro.gyroY, Yao.angle_turn_output );

					


			if(flag_10ms == 2)		//10ms
			{
				flag_10ms = 0;
//				ttt++;
//				if( ttt == 100 )
//				{
//					ttt = 0;
//					Yao.target_speed += 100;
//					if( Yao.target_speed == 800 )
//					Yao.target_speed = 0;
//				}
			}
//			
//			if(flag_15ms == 3)		//15ms
//			{
//				flag_15ms = 0;
//			}

			if(flag_20ms == 4)		//20ms
			{
				flag_20ms = 0;


				Yao.angle_turn_output = -Cascade_angle_T( &s_pid.T_angle_pid, erect_angle_Turn, imuData.angle.yaw, ins.yaw_ins );
//			Yao.angle_turn_output += kp_ * ( imuData.angle.yaw - ins.yaw_ins );
				PWMA = limit(Yao.target_speed - Yao.angle_turn_output,800);
			  PWMB = limit(Yao.target_speed + Yao.angle_turn_output,800);
				
				if( Yao.stop )
				{
					Set_PWM( 0, 0 );
					
				}
				
				else
					Set_PWM( PWMA,  PWMB );
			}

			if(flag_40ms == 8)
			{
				flag_40ms=0;
				Yao.encoder_l =  Get_Encoder_countA;//两个电机安装相反，所以编码器值也要相反
				Yao.encoder_r = -Get_Encoder_countB;
				Get_Encoder_countA = 0;//编码器计数值清零
				Get_Encoder_countB = 0;				
									
				get_realtime_coordinate( Yao.encoder_l, Yao.encoder_l, imuData.angle.yaw, 0.04, 1 );
				
				
				

			}
			if(flag_60ms == 12)		//20ms
			{
				flag_60ms = 0;
				
				


			}
			if(flag_80ms == 16)		//20ms
			{
				flag_80ms = 0;


			}
			
			
			
		}
	}
}

