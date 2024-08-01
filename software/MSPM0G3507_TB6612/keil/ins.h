/*
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-08-01 13:24:18
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-08-01 16:16:33
 * @FilePath: \4047\software\MSPM0G3507_TB6612\keil\ins.h
 * @Description: ����д���
 * ��ϵ��ʽ:921488837@qq.com
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved. 
 */
// ��ֹ���ذ����ĺ궨��
#ifndef __ins_navi_h_
#define __ins_navi_h_

// �����Ҫ��ͷ�ļ��Ϳ�
#include "itrt.h"
#include "Key.h"
#include <math.h>
#include <stdint.h>
#include <ti/driverlib/driverlib.h>
#include "ti_msp_dl_config.h"
#include "board.h"

// ����ǶȺͻ��ȵ�ת����
#define ANGLE_TO_RAD(x)     ( (x) * PI / 180.0 )                                // �Ƕ�ת��Ϊ����
#define RAD_TO_ANGLE(x)     ( (x) * 180.0 / PI )                                // ����ת��Ϊ�Ƕ�
#define PI                  ( 3.1415926535898 )                                  // PI�Ķ���

// ��������ṹ��
typedef struct {
    double x;
    double y;
} Coordinates;

// ������Ե����ṹ��
typedef struct {
    Coordinates cod_realtime;  // ʵʱ����
    Coordinates cod_saved[10]; // �������������
    double dis_ins;            // ���Ե�������
    double yaw_ins;            // ���Ե���ƫ����
} ins_struc;

// ��ȡʵʱ���꺯������
void get_realtime_coordinate(double va, double vb, double yaw, double delta_t, double wheel_base);
// ��ȡĿ�����꺯������
void get_target( double x1,double y1 , double x2,double y2 );
// ���Ե�����������
void ins_navigation(uint16_t dis_min);

// �ⲿ��������
extern ins_struc ins;
extern uint8_t flag_1;
extern uint8_t n;
extern uint8_t target;
extern uint8_t ins_mode;

#endif