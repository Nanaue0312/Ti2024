#include "ti_msp_dl_config.h"

void MotorControl(float speedL, float speedR) {
    if (speedR >= 0) {
        DL_GPIO_setPins(GRP_RIGHT_AIN_PORT, GRP_RIGHT_AIN_RIGHT_AIN2_PIN);
        DL_GPIO_clearPins(GRP_RIGHT_AIN_PORT, GRP_RIGHT_AIN_RIGHT_AIN1_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_Motor_INST, speedR, GPIO_PWM_Motor_C0_IDX);
    } else {
        DL_GPIO_setPins(GRP_RIGHT_AIN_PORT, GRP_RIGHT_AIN_RIGHT_AIN1_PIN);
        DL_GPIO_clearPins(GRP_RIGHT_AIN_PORT, GRP_RIGHT_AIN_RIGHT_AIN2_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_Motor_INST, -speedR, GPIO_PWM_Motor_C0_IDX);
    }
    if (speedL >= 0) {
        DL_GPIO_setPins(GRP_LEFT_BIN_PORT, GRP_LEFT_BIN_LEFT_BIN2_PIN);
        DL_GPIO_clearPins(GRP_LEFT_BIN_PORT, GRP_LEFT_BIN_LEFT_BIN1_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_Motor_INST, speedL, GPIO_PWM_Motor_C1_IDX);
    } else {
        DL_GPIO_setPins(GRP_LEFT_BIN_PORT, GRP_LEFT_BIN_LEFT_BIN1_PIN);
        DL_GPIO_clearPins(GRP_LEFT_BIN_PORT, GRP_LEFT_BIN_LEFT_BIN2_PIN);
        DL_TimerA_setCaptureCompareValue(PWM_Motor_INST, -speedL, GPIO_PWM_Motor_C1_IDX);
    }
}