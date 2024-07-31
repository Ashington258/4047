<!--
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-07-31 15:07:43
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-08-01 03:58:22
 * @FilePath: \4047\cache\readme.md
 * @Description: 请填写简介
 * 联系方式:921488837@qq.com
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
-->
用于临时存放书写的函数代码


书写一个决策向量。其中sensor1，sensor2，sensor3，sensor4，的值由GPIO输入捕获获得

sensor1,sensor2,sensor3,sensor4
0,0,0,0, - 直行
0,1,0,0, - 右转
0,0,1,0, - 左转
1,0,0,0, - 右转
0,0,0,1,

X,X,X,X,


// 为任务书写状态机(C语言)

拥有全局变量：
1. key_state：从GPIO获取按下的按键
   1. key_state = 1：
   2. key_state = 2：
   3. key_state = 3：
   4. key_state = 4：
2. task_end_flag：任务结束标志，用于判断任务是否结束
   1. task_end_flag = 0：任务未结束
   2. task_end_flag = 1：任务结束



3. 空闲状态：系统入口，默认IDLE状态（自循环）
4. 任务一：当key_state = 1时候进入任务一，任务执行完毕，将任务结束信号置task_end_flag为1，并返回空闲状态
5. 任务二：当key_state = 2时候进入任务二，任务执行完毕，将任务结束信号置task_end_flag为1，并返回空闲状态
6. 任务三：当key_state = 3时候进入任务三，任务执行完毕，将任务结束信号置task_end_flag为1，并返回空闲状态
7. 任务四：当key_state = 4时候进入任务四，任务执行完毕，将任务结束信号置task_end_flag为1，并返回空闲状态
