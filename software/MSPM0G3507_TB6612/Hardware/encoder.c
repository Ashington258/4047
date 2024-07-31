/*
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-07-31 15:07:43
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-07-31 18:18:19
 * @FilePath: \MSPM0G3507_TB6612\Hardware\encoder.c
 * @Description: ����д���
 * ��ϵ��ʽ:921488837@qq.com
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "empty.h"
#include "encoder.h"
#include "led.h"
uint32_t gpio_interrup;
volatile bool sensor_vector[4] = {0, 0, 0, 0};

// idenfictasion of sensor_vector

/*******************************************************
�������ܣ��ⲿ�ж�ģ��������ź�
��ں�������
����  ֵ����
***********************************************************/
void GROUP1_IRQHandler(void)
{
	
	// ʹ����˾D153C����ģ��ʱ��E1A����PA15��E1B����PA16��E2A����PA17��E2B����PA22
	// ��ȡ�ж��ź�
	gpio_interrup = DL_GPIO_getEnabledInterruptStatus(GPIOA, ENCODERA_E1A_PIN | ENCODERA_E1B_PIN | ENCODERB_E2A_PIN | ENCODERB_E2B_PIN);
	// encoderA
	if ((gpio_interrup & ENCODERA_E1A_PIN) == ENCODERA_E1A_PIN)
	{
		if (!DL_GPIO_readPins(GPIOA, ENCODERA_E1B_PIN))
		{
			Get_Encoder_countA--;
		}
		else
		{
			Get_Encoder_countA++;
		}
	}
	else if ((gpio_interrup & ENCODERA_E1B_PIN) == ENCODERA_E1B_PIN)
	{
		if (!DL_GPIO_readPins(GPIOA, ENCODERA_E1A_PIN))
		{
			Get_Encoder_countA++;
		}
		else
		{
			Get_Encoder_countA--;
		}
	}
	// encoderB
	if ((gpio_interrup & ENCODERB_E2A_PIN) == ENCODERB_E2A_PIN)
	{
		if (!DL_GPIO_readPins(GPIOA, ENCODERB_E2B_PIN))
		{
			Get_Encoder_countB--;
		}
		else
		{
			Get_Encoder_countB++;
		}
	}
	else if ((gpio_interrup & ENCODERB_E2B_PIN) == ENCODERB_E2B_PIN)
	{
		if (!DL_GPIO_readPins(GPIOA, ENCODERB_E2A_PIN))
		{
			Get_Encoder_countB++;
		}
		else
		{
			Get_Encoder_countB--;
		}
	}
	DL_GPIO_clearInterruptStatus(GPIOA, ENCODERA_E1A_PIN | ENCODERA_E1B_PIN | ENCODERB_E2A_PIN | ENCODERB_E2B_PIN);

	
	
	
	
	
	
	
	// infrared_tube_pair_sensor
	// SNESOR_1
	if (DL_GPIO_readPins(infrared_tube_pair_sensor_1_PORT, infrared_tube_pair_sensor_1_PIN))
	{
		// refreash the sensor_vector[1]
		sensor_vector[0] = 1;
	}
	if (!DL_GPIO_readPins(infrared_tube_pair_sensor_1_PORT, infrared_tube_pair_sensor_1_PIN))
	{
		sensor_vector[0] = 0;
	}

	// SENASOR_2
	if (DL_GPIO_readPins(infrared_tube_pair_sensor_2_PORT, infrared_tube_pair_sensor_2_PIN))
	{
		// refreash the sensor_vector[1]
		sensor_vector[1] = 1;
	}
	if (!DL_GPIO_readPins(infrared_tube_pair_sensor_2_PORT, infrared_tube_pair_sensor_2_PIN))
	{
		sensor_vector[1] = 0;
	}

	// SENASOR_3
	if (DL_GPIO_readPins(infrared_tube_pair_sensor_3_PORT, infrared_tube_pair_sensor_3_PIN))
	{
		// refreash the sensor_vector[1]
		sensor_vector[2] = 1;
	}
	if (!DL_GPIO_readPins(infrared_tube_pair_sensor_3_PORT, infrared_tube_pair_sensor_3_PIN))
	{
		sensor_vector[2] = 0;
	}

	// SENASOR_4
	if (DL_GPIO_readPins(infrared_tube_pair_sensor_4_PORT, infrared_tube_pair_sensor_4_PIN))
	{
		// refreash the sensor_vector[1]
		sensor_vector[3] = 1;
	}
	if (!DL_GPIO_readPins(infrared_tube_pair_sensor_4_PORT, infrared_tube_pair_sensor_4_PIN))
	{
		// refreash the sensor_vector[1]
		sensor_vector[3] = 0;
	}
}


/**
 * �Ƚ�����uint8_t���͵������Ƿ���ͬ
 *
 * @param array1 ��һ�����飬����4��uint8_tԪ��
 * @param array2 �ڶ������飬����4��uint8_tԪ��
 * @return ������������ÿ����ӦԪ�ض���ͬ���򷵻�true�����򷵻�false��
 */
bool compare_arrays(uint8_t array1[4], uint8_t array2[4])
{
	// ���������ÿ��Ԫ�أ��Ƚ����������Ƿ���ͬ
	for (int i = 0; i < 4; i++)
	{
		// ������ֶ�ӦԪ�ز���ͬ������������false
		if (array1[i] != array2[i])
		{
			return false;
		}
	}
	// �������Ԫ�ض���ͬ���򷵻�true
	return true;
}

// �Ƚ����������Ƿ����

/**
 * ���ݴ���������������ʻ���ߡ�
 *
 * ������ͨ���Ƚϴ�����������Ԥ�����ģʽ����������������Ӧ�ò�ȡ����ʻ���ԡ�
 * ÿ����ʻ���Զ�Ӧһ���ض��Ĵ���������ģʽ������ֱ�С���ת����ת������ת�ʹ���ת��
 *
 * @param sensor_vector һ������4��Ԫ�ص��޷����ַ����飬���������ļ������
 *                      �������ֵ������Ԥ�����ģʽ�������бȽϣ��Ծ�����ʻ���ԡ�
 */
void openrational_decision(uint8_t sensor_vector[4])
{
	// ����ֱ��ģʽ�Ĵ���������ģʽ
	uint8_t straight[4] = {0, 0, 0, 0};
	// ������תģʽ�Ĵ���������ģʽ
	uint8_t left_turn[4] = {0, 1, 0, 0};
	// ������תģʽ�Ĵ���������ģʽ
	uint8_t right_turn[4] = {0, 0, 1, 0};
	// �������תģʽ�Ĵ���������ģʽ
	uint8_t big_left_turn[4] = {1, 0, 0, 0};
	// �������תģʽ�Ĵ���������ģʽ
	uint8_t big_right_turn[4] = {0, 0, 0, 1};

	// ͨ���Ƚϴ�����������Ԥ����ģʽ������������ʻ����
	if (compare_arrays(sensor_vector, straight))
	{
		// ֱ�в��Դ����
		// ֱ��
	}
	else if (compare_arrays(sensor_vector, left_turn))
	{
		// ��ת���Դ����
		// ��ת
	}
	else if (compare_arrays(sensor_vector, right_turn))
	{
		// ��ת���Դ����
		// ��ת
	}
	else if (compare_arrays(sensor_vector, big_left_turn))
	{
		// ����ת���Դ����
		// ����ת
	}
	else if (compare_arrays(sensor_vector, big_right_turn))
	{
		// ����ת���Դ����
		// ����ת
	}
}
