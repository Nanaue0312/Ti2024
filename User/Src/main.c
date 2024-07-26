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

#include "PID.h"
#include "ti_msp_dl_config.h"
#include "inv_mpu.h"
#include "Board.h"
#include <stdio.h>
#include <stdlib.h>
#include "Motor.h"
#include "Encoder.h"
#include "Serial.h"
#include "OLED.h"
#include "bsp_mpu6050.h"
#include "Delay.h"
uint8_t TIMER_COUNT = 0;

// 左轮速度(round/s)
float Left_rps;
// 右轮速度(round/s)
float Right_rps;

int main(void) {
    SYSCFG_DL_init();
    // 初始化各种中断
    Board_init();
    // 初始化MPU6050
    MPU6050_Init();
    while (mpu_dmp_init()) {
        printf("dmp error\r\n");
        delay_ms(200);
    }
    printf("Initialization Data Succeed \r\n");
    // 初始化串口
    Serial_Init();
    // 初始化编码器
    Encoder_Init();
    // 初始化OLED
    OLED_Init();
    OLED_Clear();
    printf("Start\n");
    // 设置电机速度
    // MotorControl(0, 0);
    // 实测编码器一圈约为420
    // PID_Init(&PID_Position_Left, 25, 0.1, 10, 2 * 420);
    // PID_Init(&PID_Position_Right, 25, 0.1, 10, 2 * 420);
    PID_Init(&PID_Speed_Left, 10, 25, 0, 3);
    PID_Init(&PID_Speed_Right, 10, 25, 0, 3);
    // 开启定时器
    DL_TimerG_startCounter(TIMER_GLOBAL_INST);

    while (1)
        ;
}

void Position_Control() {
    PID_Position_Left.now += Count_Left;
    PID_Position_Right.now += Count_Right;
    if (PID_Position_Left.now == PID_Position_Left.target) {
        PID_Position_Left.out = 0;
    } else {
        Position_PID(&PID_Position_Left);
    }
    if (PID_Position_Right.now == PID_Position_Right.target) {
        PID_Position_Right.out = 0;
    } else {
        Position_PID(&PID_Position_Right);
    }
}

void TIMER_GLOBAL_INST_IRQHandler() {
    TIMER_COUNT++;
    switch (DL_TimerG_getPendingInterrupt(TIMER_GLOBAL_INST)) {
        case DL_TIMERG_INTERRUPT_ZERO_EVENT:
            Left_rps = (float)Count_Left * 100 / 520;
            Right_rps = (float)Count_Right * 100 / 520;
            if (TIMER_COUNT % 5 == 0) {
                // uint8_t status = mpu_dmp_get_data(&pitch, &roll, &yaw);
                // if (status == 0) {
                //     printf("pitch =%d\r\n", (int)pitch);
                //     printf("roll =%d\r\n", (int)roll);
                //     printf("yaw =%d\r\n\r\n", (int)yaw);
                // }
                TIMER_COUNT = 0;
            }
            // Position_Control();
            PID_Speed_Left.now = Left_rps;
            PID_Speed_Right.now = Right_rps;
            Incremental_PID(&PID_Speed_Left);
            Incremental_PID(&PID_Speed_Right);
            MotorControl(PID_Speed_Left.out, PID_Speed_Right.out);
            Count_Left = 0;
            Count_Right = 0;
            break;
        default:
            break;
    }
}