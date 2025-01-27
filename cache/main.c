/*
 * @Author: Ashington ashington258@proton.me
 * @Date: 2024-08-01 04:21:03
 * @LastEditors: Ashington ashington258@proton.me
 * @LastEditTime: 2024-08-01 04:21:03
 * @FilePath: \4047\cache\main.c
 * @Description: 请填写简介
 * 联系方式:921488837@qq.com
 * Copyright (c) 2024 by ${git_name_email}, All Rights Reserved.
 */
/*
 * Copyright (c) 2021, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "empty.h"
#include "task_fsm.h"

extern int32_t Get_Encoder_countA, encoderA_cnt, PWMA, Get_Encoder_countB, encoderB_cnt, PWMB;

void oled_show(void);

int main(void)
{
    SYSCFG_DL_init();

    DL_Timer_startCounter(PWM_1_INST);
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_ClearPendingIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
    // 使能串口中断
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOA_INT_IRQN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);

    OLED_Init();
    OLED_Clear();

    // KEY_Init();

    while (1)
    {
        // 更新按键状态
        // key_state = ...; // 获取按键状态的具体实现

        // 调用状态机
        StateMachine();

        // 显示OLED信息
        oled_show();
    }
}

void oled_show(void)
{
    OLED_ShowSignedNum(0, 0, encoderA_cnt, 4, 16, 1);
    OLED_ShowSignedNum(0, 20, encoderB_cnt, 4, 16, 1);

    // OLED_ShowSignedNum(0, 0, imuData.angle.roll, 4, 16, 1);
    // OLED_ShowSignedNum(0, 20, imuData.angle.pitch, 4, 16, 1);
    // OLED_ShowSignedNum(0, 40, imuData.angle.yaw, 4, 16, 1);

    OLED_Refresh();
}
