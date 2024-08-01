/*
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-08-01 13:24:18
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-08-01 16:16:33
 * @FilePath: \4047\software\MSPM0G3507_TB6612\keil\ins.h
 * @Description: 请填写简介
 * 联系方式:921488837@qq.com
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
// 防止多重包含的宏定义
#ifndef __ins_navi_h_
#define __ins_navi_h_

// 引入必要的头文件和库
#include "itrt.h"
#include "Key.h"
#include <math.h>
#include <stdint.h>
#include <ti/driverlib/driverlib.h>
#include "ti_msp_dl_config.h"
#include "board.h"

// 定义角度和弧度的转换宏
#define ANGLE_TO_RAD(x)     ( (x) * PI / 180.0 )                                // 角度转换为弧度
#define RAD_TO_ANGLE(x)     ( (x) * 180.0 / PI )                                // 弧度转换为角度
#define PI                  ( 3.1415926535898 )                                  // PI的定义

// 定义坐标结构体
typedef struct {
    double x;
    double y;
} Coordinates;

// 定义惯性导航结构体
typedef struct {
    Coordinates cod_realtime;  // 实时坐标
    Coordinates cod_saved[10]; // 保存的坐标数组
    double dis_ins;            // 惯性导航距离
    double yaw_ins;            // 惯性导航偏航角
} ins_struc;

// 获取实时坐标函数声明
void get_realtime_coordinate(double va, double vb, double yaw, double delta_t, double wheel_base);
// 获取目标坐标函数声明
void get_target( double x1,double y1 , double x2,double y2 );
// 惯性导航函数声明
void ins_navigation(uint16_t dis_min);

// 外部变量声明
extern ins_struc ins;
extern uint8_t flag_1;
extern uint8_t n;
extern uint8_t target;
extern uint8_t ins_mode;

#endif