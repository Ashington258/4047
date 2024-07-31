#include "motor.h"

void Set_PWM(int pwma,int pwmb)
{
	//在使用我司的D153C驱动模块的时候，PA12接AIN2、PA13解AIN1,PB16接BIN2、PB0接着BIN1
	if(pwma>0) 
	{
		DL_GPIO_setPins(AIN1_PORT,AIN1_PIN_12_PIN);
		DL_GPIO_clearPins(AIN2_PORT,AIN2_PIN_13_PIN);
		DL_TimerA_setCaptureCompareValue(PWM_1_INST, ABS(pwma), GPIO_PWM_1_C0_IDX);
	}
	else
	{
		DL_GPIO_clearPins(AIN2_PORT,AIN2_PIN_13_PIN);
		DL_GPIO_setPins(AIN1_PORT,AIN1_PIN_12_PIN);
		DL_TimerA_setCaptureCompareValue(PWM_1_INST, ABS(pwma), GPIO_PWM_1_C0_IDX);
	}
	if(pwmb>0)
	{
		DL_GPIO_setPins(BIN1_PORT,BIN1_Pin_Bin1_PIN);
		DL_GPIO_clearPins(BIN2_PORT,BIN2_Pin_Bin2_PIN);
		DL_TimerA_setCaptureCompareValue(PWM_1_INST, ABS(pwmb), GPIO_PWM_1_C1_IDX);
	}
    else
	{
		DL_GPIO_setPins(BIN2_PORT,BIN2_Pin_Bin2_PIN);
		DL_GPIO_clearPins(BIN1_PORT,BIN1_Pin_Bin1_PIN);
		DL_TimerA_setCaptureCompareValue(PWM_1_INST, ABS(pwmb), GPIO_PWM_1_C1_IDX);
	}		

}



