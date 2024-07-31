/*
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-07-31 15:07:43
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-07-31 18:18:19
 * @FilePath: \MSPM0G3507_TB6612\Hardware\encoder.c
 * @Description: 请填写简介
 * 联系方式:921488837@qq.com
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "empty.h"
#include "encoder.h"
#include "led.h"
uint32_t gpio_interrup;
volatile bool sensor_vector[4] = {0, 0, 0, 0};

// idenfictasion of sensor_vector

/*******************************************************
函数功能：外部中断模拟编码器信号
入口函数：无
返回  值：无
***********************************************************/
void GROUP1_IRQHandler(void)
{
	
	// 使用我司D153C驱动模块时，E1A连接PA15、E1B连接PA16、E2A连接PA17，E2B连接PA22
	// 获取中断信号
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
 * 比较两个uint8_t类型的数组是否相同
 *
 * @param array1 第一个数组，包含4个uint8_t元素
 * @param array2 第二个数组，包含4个uint8_t元素
 * @return 如果两个数组的每个对应元素都相同，则返回true；否则返回false。
 */
bool compare_arrays(uint8_t array1[4], uint8_t array2[4])
{
	// 遍历数组的每个元素，比较两个数组是否相同
	for (int i = 0; i < 4; i++)
	{
		// 如果发现对应元素不相同，则立即返回false
		if (array1[i] != array2[i])
		{
			return false;
		}
	}
	// 如果所有元素都相同，则返回true
	return true;
}

// 比较两个数组是否相等

/**
 * 根据传感器向量做出行驶决策。
 *
 * 本函数通过比较传感器向量与预定义的模式向量，来决定车辆应该采取的行驶策略。
 * 每种行驶策略对应一个特定的传感器向量模式，包括直行、左转、右转、大左转和大右转。
 *
 * @param sensor_vector 一个包含4个元素的无符号字符数组，代表传感器的检测结果。
 *                      该数组的值用于与预定义的模式向量进行比较，以决定行驶策略。
 */
void openrational_decision(uint8_t sensor_vector[4])
{
	// 定义直行模式的传感器向量模式
	uint8_t straight[4] = {0, 0, 0, 0};
	// 定义左转模式的传感器向量模式
	uint8_t left_turn[4] = {0, 1, 0, 0};
	// 定义右转模式的传感器向量模式
	uint8_t right_turn[4] = {0, 0, 1, 0};
	// 定义大左转模式的传感器向量模式
	uint8_t big_left_turn[4] = {1, 0, 0, 0};
	// 定义大右转模式的传感器向量模式
	uint8_t big_right_turn[4] = {0, 0, 0, 1};

	// 通过比较传感器向量与预定义模式向量，决定行驶策略
	if (compare_arrays(sensor_vector, straight))
	{
		// 直行策略代码段
		// 直行
	}
	else if (compare_arrays(sensor_vector, left_turn))
	{
		// 左转策略代码段
		// 左转
	}
	else if (compare_arrays(sensor_vector, right_turn))
	{
		// 右转策略代码段
		// 右转
	}
	else if (compare_arrays(sensor_vector, big_left_turn))
	{
		// 大左转策略代码段
		// 大左转
	}
	else if (compare_arrays(sensor_vector, big_right_turn))
	{
		// 大右转策略代码段
		// 大右转
	}
}
