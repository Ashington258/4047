#include "task_fsm.h"
#include <stdio.h>

// 全局变量定义
volatile int key_state = 0;     // 从GPIO获取按下的按键
volatile int task_end_flag = 0; // 任务结束标志

// 当前状态
static State current_state = IDLE;

// 状态机函数
void StateMachine(void)
{
    switch (current_state)
    {
    case IDLE:
        // 在空闲状态等待按键按下
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
    // 任务一具体实现
    printf("Executing Task 1\n");
    // 模拟任务结束
    task_end_flag = 1;
}

// 任务二
void Task2(void)
{
    // 任务二具体实现
    printf("Executing Task 2\n");
    // 模拟任务结束
    task_end_flag = 1;
}

// 任务三
void Task3(void)
{
    // 任务三具体实现
    printf("Executing Task 3\n");
    // 模拟任务结束
    task_end_flag = 1;
}

// 任务四
void Task4(void)
{
    // 任务四具体实现
    printf("Executing Task 4\n");
    // 模拟任务结束
    task_end_flag = 1;
}
