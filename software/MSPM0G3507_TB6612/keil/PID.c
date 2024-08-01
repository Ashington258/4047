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



//用于初始化pid参数的函数
void PID_Init(PID *pid,float p,float i,float d,float maxI,float maxOut)
{
    pid->kp=p;
    pid->ki=i;
    pid->kd=d;
    pid->maxIntegral=maxI;
    pid->maxOutput=maxOut;
}

//进行一次pid计算
//参数为(pid结构体,目标值,反馈值)，计算结果放在pid结构体的output成员中
void PID_Calc(PID *pid,float reference,float feedback)
{
 	//更新数据
    pid->lastError=pid->error;//将旧error存起来
    pid->error=reference-feedback;//计算新error
    //计算微分
    float dout=(pid->error-pid->lastError)*pid->kd;
    //计算比例
    float pout=pid->error*pid->kp;
    //计算积分
    pid->integral+=pid->error*pid->ki;
    //积分限幅
    if(pid->integral > pid->maxIntegral) pid->integral=pid->maxIntegral;
    else if(pid->integral < -pid->maxIntegral) pid->integral=-pid->maxIntegral;
    //计算输出
    pid->output=pout+dout+pid->integral;
    //输出限幅
    if(pid->output > pid->maxOutput) pid->output=pid->maxOutput;
    else if(pid->output < -pid->maxOutput) pid->output=-pid->maxOutput;
}


