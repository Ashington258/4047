<!--
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-07-31 15:07:43
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-08-01 03:23:32
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


// 任务状态机

