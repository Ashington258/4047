#include "pid.h"

float Velcity_Kp=0.45,  Velcity_Ki=0.05,  Velcity_Kd; //相关速度增量式PID参数

/***************************************************************************
函数功能：电机编码器的增量式PID闭环控制
入口参数：左右电机的编码器值
返回值  ：电机的PWM
***************************************************************************/
int Velocity_A(int TargetVelocity, int CurrentVelocity)
{  
    int Bias;  //定义相关变量
	static int ControlVelocityA, Last_biasA; //静态变量，函数调用结束后其值依然存在
		
	Bias=TargetVelocity-CurrentVelocity; //求速度偏差
		
	ControlVelocityA+=Velcity_Kp*(Bias-Last_biasA)+Velcity_Ki*Bias;  //增量式PI控制器
                                                                   //Velcity_Kp*(Bias-Last_bias) 作用为限制加速度
	                                                                //Velcity_Ki*Bias   速度控制值由Bias不断积分得到 偏差越大加速度越大
	Last_biasA=Bias;	
	if(ControlVelocityA>3600) ControlVelocityA=3600;
	else if(ControlVelocityA<-3600) ControlVelocityA=-3600;
	return ControlVelocityA; //返回速度控制值
	
}

/***************************************************************************
函数功能：电机编码器的增量式PID闭环控制
入口参数：左右电机的编码器值
返回值  ：电机的PWM
***************************************************************************/
int Velocity_B(int TargetVelocity, int CurrentVelocity)
{  
    int Bias;  //定义相关变量
	static int ControlVelocityB, Last_biasB; //静态变量，函数调用结束后其值依然存在
		
	Bias=TargetVelocity-CurrentVelocity; //求速度偏差
		
	ControlVelocityB+=Velcity_Kp*(Bias-Last_biasB)+Velcity_Ki*Bias;  //增量式PI控制器
                                                                   //Velcity_Kp*(Bias-Last_bias) 作用为限制加速度
	                                                                 //Velcity_Ki*Bias             速度控制值由Bias不断积分得到 偏差越大加速度越大
	Last_biasB=Bias;	
	if(ControlVelocityB>3600) ControlVelocityB=3600;
	else if(ControlVelocityB<-3600) ControlVelocityB=-3600;
	return ControlVelocityB; //返回速度控制值
}



