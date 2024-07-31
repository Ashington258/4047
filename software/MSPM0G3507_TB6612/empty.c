/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "board.h"
#include "stdio.h"
#include "Key.h"
#include "led.h"
#include "motor.h"
#include "oled.h"



//int32_t Get_Encoder_countA,encoderA_cnt,PWMA,Get_Encoder_countB,encoderB_cnt,PWMB;
//uint8_t Key_Num = 0;



void oled_show(void);


int main(void)
{
	SYSCFG_DL_init();
	DL_Timer_startCounter(PWM_0_INST);
	NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
	NVIC_ClearPendingIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
	NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
//	//使能串口中断
	NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
	NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);

	OLED_Init();
	OLED_Clear();
	
//	KEY_Init();
	
    while (1) 
    {
//		printf("%d  %d\n\r",Yao.encoder_l,Yao.encoder_r);
		oled_show();	
		LED_ON();
    }
}







void oled_show(void)
{
//      OLED_ShowString(0,8,(uint8_t*)"A",12,1);//6*12 
//      OLED_ShowString(0,20,(uint8_t*)"B",16,1);//8*16 
//      OLED_ShowString(0,36,(uint8_t*)"C",24,1);//12*24 
//		
////	OLED_ShowString(0,0*16,"Hello Word",OLED_8X16);
		
//		i--;
		OLED_ShowNum(0,0,8888,4,12,1);
		OLED_ShowSignedNum(0,10,-6666,4,12,1);
		OLED_ShowNum(0,20,8888,4,12,1);
		OLED_ShowSignedNum(0,30,-6666,4,16,1);	
//		OLED_ShowSignedNum(0,0,i,3,16,1);
			   
        OLED_Refresh();
        delay_ms(100);
}







