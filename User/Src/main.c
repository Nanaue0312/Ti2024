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
#include "bsp_VL53L0X.h"
#include "Delay.h"
#include "LM75A.h"
#include "PID.h"
uint8_t TIMER_COUNT = 0;

// 左轮速度(round/s)
float Left_rps;
// 右轮速度(round/s)
float Right_rps;

uint8_t mode = 0;
VL53L0X_Error Status = VL53L0X_ERROR_NONE;
int main(void) {
    // 初始化各种中断
    Board_init();
    // 初始化串口
    Serial_Init();
    // 初始化编码器
    Encoder_Init();
    // 初始化OLED
    OLED_Init();
    OLED_ShowChar(1, 1, 'A');
    OLED_ShowChar(1, 2, 'B');
    OLED_ShowChar(1, 3, 'C');
    printf("Start\n");
    // 初始化MPU6050
    MPU6050_Init();
    while (mpu_dmp_init()) {
        printf("dmp error\r\n");
        delay_ms(200);
    }
    printf("Initialization Data Succeed \r\n");
    // 初始化LM75A温度传感器
    LM75_Init();
    // TODO 多路激光测距还需测试，但已基本成功
    while (vl53l0x_init(&vl53l0x_dev1, 1)) {
        printf("VL53L0X Error!!!\n\r");
        delay_ms(500);
    }
    printf("VL53L0X OK\r\n");

    while (vl53l0x_set_mode(&vl53l0x_dev1, mode)) {
        printf("Mode Set Error\r\n");
        delay_ms(500);
    }
    // 设置电机速度
    // MotorControl(0, 0);
    // 实测编码器一圈约为520
    PID_Init(&PID_Speed_Left, 10, 25, 0, 3);
    PID_Init(&PID_Speed_Right, 10, 25, 0, 3);
    // 开启定时器
    DL_TimerG_startCounter(TIMER_GLOBAL_INST);
    int i = 0;
    while (1) {
        delay_ms(500);
    };
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
                if (mpu_dmp_get_data(&pitch, &roll, &yaw) == 0) {
                    // printf("pitch =%d\r\n", (int)pitch);
                    // printf("roll =%d\r\n", (int)roll);
                    // printf("yaw =%d\r\n\r\n", (int)yaw);
                }
                if (Status == VL53L0X_ERROR_NONE) {
                    Status = VL53L0X_PerformSingleRangingMeasurement(&vl53l0x_dev1, &vl53l0x_data);
                    printf("d: %4imm\r\n", vl53l0x_data.RangeMilliMeter);
                } else {
                    printf("error\r\n");
                }
            }
            if (TIMER_COUNT % 500 == 0) {
                uint16_t temp = LM75_ReadTemp();
                if ((temp & 0x8000) == 0x8000) {
                    temp >>= 5;
                    temperature = (int16_t)temp * -0.125;
                } else {
                    temp >>= 5;
                    temperature = temp * 0.125;
                }
                TIMER_COUNT = 0;
            }
            // Position_Control();
            PID_Speed_Left.now = Left_rps;
            PID_Speed_Right.now = Right_rps;
            Incremental_PID(&PID_Speed_Left);
            Incremental_PID(&PID_Speed_Right);
            // MotorControl(PID_Speed_Left.out, PID_Speed_Right.out);
            Count_Left = 0;
            Count_Right = 0;
            break;
        default:
            break;
    }
}