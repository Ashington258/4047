/*
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-07-31 15:07:43
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-08-01 00:34:34
 * @FilePath: \4047\software\MSPM0G3507_TB6612\keil\itrt.c
 * @Description: 请填写简介
 * 联系方式:921488837@qq.com
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "board.h"
#include "key.h"
#include "led.h"
#include "motor.h"
#include "itrt.h"
#include "PID.h"
#include "wit_imu.h"

int32_t Get_Encoder_countA, encoderA_cnt, PWMA, Get_Encoder_countB, encoderB_cnt, PWMB;
uint8_t Key_Num = 0;

control_struc Yao;

uint8_t flag_10ms = 0;
uint8_t flag_15ms = 0;
uint8_t flag_20ms = 0;
uint8_t flag_40ms = 0;


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
					
	
//			pwm output
//			PWMA = Velocity_A(-0+Yao.gyro_turn_output,Yao.encoder_l);
//			PWMB = Velocity_B(-0-Yao.gyro_turn_output,Yao.encoder_r);


//			if(flag_10ms == 2)		//10ms
//			{
//				flag_10ms = 0;
//			}
//			
//			if(flag_15ms == 3)		//15ms
//			{
//				flag_15ms = 0;
//			}

//			if(flag_20ms == 4)		//20ms
//			{
//				flag_20ms = 0;


//			}

			if(flag_40ms == 8)
			{
				flag_40ms=0;
				encoderA_cnt = Get_Encoder_countA;//两个电机安装相反，所以编码器值也要相反
				encoderB_cnt = -Get_Encoder_countB;
				Get_Encoder_countA = 0;//编码器计数值清零
				Get_Encoder_countB = 0;				
				
			}
			
		}
	}
}

