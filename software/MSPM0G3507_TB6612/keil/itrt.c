#include "board.h"
#include "key.h"
#include "led.h"
#include "motor.h"
#include "itrt.h"
#include "PID.h"
#include "wit_imu.h"
#include "ins.h"

int32_t Get_Encoder_countA,encoderA_cnt,PWMA,Get_Encoder_countB,encoderB_cnt,PWMB;
uint8_t Key_Num = 0;

control_struc Yao;

uint8_t flag_10ms = 0;
uint8_t flag_15ms = 0;
uint8_t flag_20ms = 0;




void TIMER_0_INST_IRQHandler(void)		//5ms
{
	if(DL_TimerA_getPendingInterrupt(TIMER_0_INST))
	{
		if(DL_TIMER_IIDX_ZERO)
		{
			flag_10ms++;
			flag_15ms++;
			flag_20ms++;
			
			// encoder
				Yao.encoder_l = -Get_Encoder_countA;//两个电机安装相反，所以编码器值也要相反
			  Yao.encoder_r = Get_Encoder_countB;
				Get_Encoder_countA = 0;//编码器计数值清零
			  Get_Encoder_countB = 0;
			get_realtime_coordinate( Yao.encoder_l, Yao.encoder_r, imuData.angle.yaw, 0.005, 11.5 );
			// pwm output
				PWMA = Velocity_A(-0+Yao.gyro_turn_output,Yao.encoder_l);
			  PWMB = Velocity_B(-0-Yao.gyro_turn_output,Yao.encoder_r);
			  Set_PWM(PWMA,PWMB);
			if(flag_10ms == 2)		//10ms
			{
				flag_10ms = 0;
				Yao.gyro_turn_output = Cascade_gro_T( &s_pid.T_gyro_pid, erect_gyro_Turn, imuData.gyro.gyroZ, 0 );
				Yao.gyro_turn_output = limit( Yao.gyro_turn_output, 1000 );
				
				ins_navigation( 50 );
				
			}
			
			if(flag_15ms == 3)		//15ms
			{
				flag_15ms = 0;
				Yao.gyro_turn_output = Cascade_angle_T( &s_pid.T_gyro_pid, erect_angle_Turn, imuData.angle.yaw, ins.yaw_ins );
				Yao.gyro_turn_output = limit( Yao.gyro_turn_output, 100 );

			}
			
			if(flag_20ms == 4)		//20ms
			{
				flag_20ms = 0;
				
				Key_Scan();
				
			}
			
		}
		       
	}
}