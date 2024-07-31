/*
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-07-31 15:07:43
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-07-31 17:19:37
 * @FilePath: \4047\cache\line_follow.c
 * @Description: 请填写简介
 * 联系方式:921488837@qq.com
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
#include "line_follow.h"
#include <stdint.h>
#include <stdbool.h>

// 创建循迹决策向量
uint8_t sensor_vector[4] = {0, 0, 0, 0};

// 比较两个数组是否相等
bool compare_arrays(uint8_t array1[4], uint8_t array2[4])
{
    for (int i = 0; i < 4; i++)
    {
        if (array1[i] != array2[i])
        {
            return false;
        }
    }
    return true;
}

void openrational_decision(uint8_t sensor_vector[4])
{
    uint8_t straight[4] = {0, 0, 0, 0};
    uint8_t left_turn[4] = {0, 1, 0, 0};
    uint8_t right_turn[4] = {0, 0, 1, 0};
    uint8_t big_left_turn[4] = {1, 0, 0, 0};
    uint8_t big_right_turn[4] = {0, 0, 0, 1};

    if (compare_arrays(sensor_vector, straight))
    {
        // 直行
    }
    else if (compare_arrays(sensor_vector, left_turn))
    {
        // 左转
    }
    else if (compare_arrays(sensor_vector, right_turn))
    {
        // 右转
    }
    else if (compare_arrays(sensor_vector, big_left_turn))
    {
        // 大左转
    }
    else if (compare_arrays(sensor_vector, big_right_turn))
    {
        // 大右转
    }
}
