/*
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-08-01 04:44:07
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-08-01 04:45:54
 * @FilePath: \MSPM0G3507_TB6612\Hardware\task_fsm.c
 * @Description: 请填写简介
 * 联系方式:921488837@qq.com
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "task_fsm.h"
#include <stdio.h>
#include "encoder.h"
#include "itrt.h"
#include "Motor.h"
#include "LED.h"
#include "control.h"
#include "pid.h"

// 全局变量定义
// volatile int key_state = 0;     // 从GPIO获取按下的按键
volatile int task_end_flag = 0; // 任务结束标志

// 当前状态
static State current_state = IDLE;

// 差速
int8_t delta_v;

// 状态机函数
void StateMachine(void)
{
    switch (current_state)
    {
    case IDLE:
        // 在空闲状态等待按键按下
		LED_OFF();
        if (key_state == 1)
        {
            current_state = TASK1;
        }
        else if (key_state == 2)
        {
            current_state = TASK2;
        }
        else if (key_state == 3)
        {
            current_state = TASK3;
        }
        else if (key_state == 4)
        {
            current_state = TASK4;
        }
        break;

    case TASK1:
        Task1();
        if (task_end_flag == 1)
        {
            task_end_flag = 0;
            current_state = IDLE;
			
			
        }
        break;

    case TASK2:
        Task2();
        if (task_end_flag == 1)
        {
            task_end_flag = 0;
            current_state = IDLE;
        }
        break;

    case TASK3:
        Task3();
        if (task_end_flag == 1)
        {
            task_end_flag = 0;
            current_state = IDLE;
        }
        break;

    case TASK4:
        Task4();
        if (task_end_flag == 1)
        {
            task_end_flag = 0;
            current_state = IDLE;
        }
        break;

    default:
        current_state = IDLE;
        break;
    }
}

// 任务一
void Task1(void)
{
	Speed(70,70);
	Encoder_Enable = 1;
    // 任务一具体实现
	if(Encoder_Distance>3000 && (sensor_vector[0]|sensor_vector[1]|sensor_vector[2]|sensor_vector[3]))
	{
		Set_PWM(0,0);
		Encoder_Enable = 0;  
		LED_ON();
		
		// 模拟任务结束
		task_end_flag = 1;
	}
	//编码器记到一定数值可以强制任务结束，增加摆头
	
}

// 任务二
void Task2(void)
{
    // 任务二具体实现
//	Speed(70,70);
	
	int16_t Encoder =  Get_Encoder();
//	Turn_left(Encoder,16);//左转角度刚好
	Turn_right(Encoder,PID_Controller.output);//右转角度刚好
    // 模拟任务结束
//    task_end_flag = 1;
}



// 任务三
void Task3(void)
{
    // 任务三具体实现

    // 模拟任务结束
    task_end_flag = 1;
}

// 任务四
void Task4(void)
{
    // 任务四具体实现

    // 模拟任务结束
    task_end_flag = 1;
}


