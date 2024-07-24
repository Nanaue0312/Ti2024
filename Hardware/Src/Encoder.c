#include "ti_msp_dl_config.h"
#include "Encoder.h"
#include "stdbool.h"
#include "stdio.h"
int16_t Count_Left;
int16_t Count_Right;

/**
 * @brief 开启编码器引脚的中断，开始计数
 *
 */
void Encoder_Init() { NVIC_EnableIRQ(GPIO_MULTIPLE_GPIOB_INT_IRQN); }

void GROUP1_IRQHandler() {
    uint32_t GPIO_B = DL_GPIO_getEnabledInterruptStatus(GRP_ENCODER_RIGHT_PORT,
                                                        GRP_ENCODER_RIGHT_RIGHT_A_PIN | GRP_ENCODER_LEFT_LEFT_A_PIN);

    // 判断右轮A相
    if ((GPIO_B & GRP_ENCODER_RIGHT_RIGHT_A_PIN) == GRP_ENCODER_RIGHT_RIGHT_A_PIN) {
        if (DL_GPIO_readPins(GRP_ENCODER_RIGHT_PORT, GRP_ENCODER_RIGHT_RIGHT_A_PIN) == GRP_ENCODER_RIGHT_RIGHT_A_PIN) {
            // 如果A相上升沿
            // B相高电平
            if (DL_GPIO_readPins(GRP_ENCODER_RIGHT_PORT, GRP_ENCODER_RIGHT_RIGHT_B_PIN)) {
                Count_Right--;
            } else {
                Count_Right++;
            }
        } else {
            // 如果A相下降沿
            // B相高电平
            if (DL_GPIO_readPins(GRP_ENCODER_RIGHT_PORT, GRP_ENCODER_RIGHT_RIGHT_B_PIN) ==
                GRP_ENCODER_RIGHT_RIGHT_B_PIN) {
                Count_Right++;
            } else {
                // B相低电平
                Count_Right--;
            }
        }
        DL_GPIO_clearInterruptStatus(GRP_ENCODER_RIGHT_PORT, GRP_ENCODER_RIGHT_RIGHT_A_PIN);
    }
    // 判断左轮A相
    if ((GPIO_B & GRP_ENCODER_LEFT_LEFT_A_PIN) == GRP_ENCODER_LEFT_LEFT_A_PIN) {
        if (DL_GPIO_readPins(GRP_ENCODER_LEFT_PORT, GRP_ENCODER_LEFT_LEFT_A_PIN) == GRP_ENCODER_LEFT_LEFT_A_PIN) {
            // 如果A相上升沿
            // B相高电平
            if (DL_GPIO_readPins(GRP_ENCODER_LEFT_PORT, GRP_ENCODER_LEFT_LEFT_B_PIN)) {
                Count_Left++;
            } else {
                Count_Left--;
            }
        } else {
            // 如果A相下降沿
            // B相高电平
            if (DL_GPIO_readPins(GRP_ENCODER_LEFT_PORT, GRP_ENCODER_LEFT_LEFT_B_PIN) == GRP_ENCODER_LEFT_LEFT_B_PIN) {
                Count_Left--;
            } else {
                // B相低电平
                Count_Left++;
            }
        }
        DL_GPIO_clearInterruptStatus(GRP_ENCODER_LEFT_PORT, GRP_ENCODER_LEFT_LEFT_A_PIN);
    }
}
