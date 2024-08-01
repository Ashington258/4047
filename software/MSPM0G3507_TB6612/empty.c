#include "empty.h"
#include "task_fsm.h"

extern int32_t Get_Encoder_countA, encoderA_cnt, PWMA, Get_Encoder_countB, encoderB_cnt, PWMB;

void oled_show(void);


int main(void)
{
	SYSCFG_DL_init();

	DL_Timer_startCounter(PWM_1_INST);
	NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
	NVIC_ClearPendingIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
	NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
	
	NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
	NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
	NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);

	OLED_Init();
	OLED_Clear();
	
//	PID_Init(&PID_Controller,1,0,0,-100,100);
	
//  KEY_Init();
//	Speed(20,20);
	while (1)
	{
//		StateMachine();
		
		oled_show();
	}
}

void oled_show(void)
{
	OLED_ShowSignedNum(0,0,imuData.gyro.gyroY,4,12,1);OLED_ShowSignedNum(50,0,(int)imuData.angle.yaw,4,12,1);
	OLED_ShowSignedNum(0,10,ins_mode,4,12,1);OLED_ShowSignedNum(50,10,target,4,12,1);
	OLED_ShowSignedNum(0,20,ins.dis_ins,4,12,1);OLED_ShowSignedNum(50,20,ins.yaw_ins,4,12,1);
	OLED_ShowSignedNum(0,30,ins.cod_realtime.x,4,12,1);OLED_ShowSignedNum(50,30,ins.cod_realtime.y,4,12,1);
	OLED_ShowSignedNum(0,40,n,4,12,1);OLED_ShowSignedNum(50,40,ins.cod_saved[2].x,4,12,1);
	
//	OLED_ShowSignedNum(0,20,Encoder_Distance,4,16,1);
//	OLED_ShowSignedNum(0, 0, imuData.angle.roll, 4, 16, 1);
//	OLED_ShowSignedNum(0, 20, imuData.angle.pitch, 4, 16, 1);
//	OLED_ShowSignedNum(0, 40, imuData.angle.yaw, 4, 16, 1);
	
	OLED_Refresh();
}
