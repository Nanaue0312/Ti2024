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
#include <stdint.h>
#include <stdio.h>
#include "Motor.h"
#include "UserFilters.h"
#include "Encoder.h"
#include "Serial.h"
#include "OLED.h"

uint8_t TIMER_COUNT = 0;

// 左轮速度(round/s)
float Speed_Left;
// 右轮速度(round/s)
float Speed_Right;

int main(void) {
    SYSCFG_DL_init();
    // 开启定时器10ms中断
    NVIC_EnableIRQ(TIMER_GLOBAL_INST_INT_IRQN);
    Serial_Init();
    Encoder_Init();
    // 初始化OLED
    OLED_Init();
    OLED_Clear();
    OLED_ShowChar(1, 1, 'A', 16);
    OLED_ShowString(0, 0, "Hello World", 16);
    printf("Start\n");
    // MotorControl(3000, 3000);
    // 开启定时器
    DL_TimerG_startCounter(TIMER_GLOBAL_INST);
    while (1) {
    }
}

void TIMER_GLOBAL_INST_IRQHandler() {
    TIMER_COUNT++;
    switch (DL_TimerG_getPendingInterrupt(TIMER_GLOBAL_INST)) {
        case DL_TIMERG_INTERRUPT_ZERO_EVENT:
            if (TIMER_COUNT % 100 == 0) {
                TIMER_COUNT = 0;
                Speed_Left = (float)Count_Left / 520;
                Speed_Right = (float)Count_Right / 520;
                // FILTER_Speed_LEFT.In = Speed_Left;
                // FILTER_Speed_LEFT.gain = 256;
                // Difference_Integration_Filtering(&FILTER_Speed_LEFT);
                // FILTER_Speed_RIGHT.In = Speed_Right;
                // FILTER_Speed_RIGHT.gain = 256;
                // Difference_Integration_Filtering(&FILTER_Speed_RIGHT);
                printf("%.2f,%.2f\n", Speed_Left, Speed_Right);
                Count_Left = 0;
                Count_Right = 0;
            }
            break;
        default:
            break;
    }
}