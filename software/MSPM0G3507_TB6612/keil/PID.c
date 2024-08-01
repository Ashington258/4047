#include "pid.h"

float Velcity_Kp=0.45,  Velcity_Ki=0.05,  Velcity_Kd; //����ٶ�����ʽPID����

/***************************************************************************
�������ܣ����������������ʽPID�ջ�����
��ڲ��������ҵ���ı�����ֵ
����ֵ  �������PWM
***************************************************************************/
int Velocity_A(int TargetVelocity, int CurrentVelocity)
{  
    int Bias;  //������ر���
	static int ControlVelocityA, Last_biasA; //��̬�������������ý�������ֵ��Ȼ����
		
	Bias=TargetVelocity-CurrentVelocity; //���ٶ�ƫ��
		
	ControlVelocityA+=Velcity_Kp*(Bias-Last_biasA)+Velcity_Ki*Bias;  //����ʽPI������
                                                                   //Velcity_Kp*(Bias-Last_bias) ����Ϊ���Ƽ��ٶ�
	                                                                //Velcity_Ki*Bias   �ٶȿ���ֵ��Bias���ϻ��ֵõ� ƫ��Խ����ٶ�Խ��
	Last_biasA=Bias;	
	if(ControlVelocityA>3600) ControlVelocityA=3600;
	else if(ControlVelocityA<-3600) ControlVelocityA=-3600;
	return ControlVelocityA; //�����ٶȿ���ֵ
	
}

/***************************************************************************
�������ܣ����������������ʽPID�ջ�����
��ڲ��������ҵ���ı�����ֵ
����ֵ  �������PWM
***************************************************************************/
int Velocity_B(int TargetVelocity, int CurrentVelocity)
{  
    int Bias;  //������ر���
	static int ControlVelocityB, Last_biasB; //��̬�������������ý�������ֵ��Ȼ����
		
	Bias=TargetVelocity-CurrentVelocity; //���ٶ�ƫ��
		
	ControlVelocityB+=Velcity_Kp*(Bias-Last_biasB)+Velcity_Ki*Bias;  //����ʽPI������
                                                                   //Velcity_Kp*(Bias-Last_bias) ����Ϊ���Ƽ��ٶ�
	                                                                 //Velcity_Ki*Bias             �ٶȿ���ֵ��Bias���ϻ��ֵõ� ƫ��Խ����ٶ�Խ��
	Last_biasB=Bias;	
	if(ControlVelocityB>3600) ControlVelocityB=3600;
	else if(ControlVelocityB<-3600) ControlVelocityB=-3600;
	return ControlVelocityB; //�����ٶȿ���ֵ
}



