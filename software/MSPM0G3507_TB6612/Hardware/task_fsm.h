/*
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-08-01 04:20:35
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-08-01 04:20:43
 * @FilePath: \4047\cache\task_fsm.h
 * @Description: 请填写简介
 * 联系方式:921488837@qq.com
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef TASK_FSM_H
#define TASK_FSM_H

// 状态枚举
typedef enum
{
    IDLE,
    TASK1,
    TASK2,
    TASK3,
    TASK4
} State;

// 全局变量
//extern volatile int key_state;     // 从GPIO获取按下的按键
extern volatile int task_end_flag; // 任务结束标志

// 任务函数声明
void Task1(void);
void Task2(void);
void Task3(void);
void Task4(void);

// 状态机函数声明
void StateMachine(void);

#endif // TASK_FSM_H
