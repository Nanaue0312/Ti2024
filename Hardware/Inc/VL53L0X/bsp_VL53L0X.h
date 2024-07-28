/*
 * ������������Ӳ�������������չ����Ӳ�����Ϲ���ȫ����Դ
 * �����������www.lckfb.com
 * ����֧�ֳ�פ��̳���κμ������⻶ӭ��ʱ����ѧϰ
 * ������̳��https:
 * ��עbilibili�˺ţ������������塿���������ǵ����¶�̬��
 * ��������׬Ǯ���������й�����ʦΪ����
 * Change Logs:
 * Date           Author       Notes
 * 2024-03-29     LCKFB-LP    first version
 */
#ifndef __VL53L0X_H
#define __VL53L0X_H

#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"
#include "vl53l0x_gen.h"
#include "vl53l0x_cali.h"
#include "Delay.h"

#define VL53L0X_Addr 0x52

#define VL53L0X_Xshut0(x)                                                                                              \
    ((x) ? (DL_GPIO_setPins(GRP_VL53L0X_PORT, GRP_VL53L0X_SHUT0_PIN))                                                  \
         : (DL_GPIO_clearPins(GRP_VL53L0X_PORT, GRP_VL53L0X_SHUT0_PIN)))
#define VL53L0X_Xshut1(x)                                                                                              \
    ((x) ? (DL_GPIO_setPins(GRP_VL53L0X_PORT, GRP_VL53L0X_SHUT1_PIN))                                                  \
         : (DL_GPIO_clearPins(GRP_VL53L0X_PORT, GRP_VL53L0X_SHUT1_PIN)))

#define VL53L0X_Xshut2(x)                                                                                              \
    ((x) ? (DL_GPIO_setPins(GRP_VL53L0X_PORT, GRP_VL53L0X_SHUT2_PIN))                                                  \
         : (DL_GPIO_clearPins(GRP_VL53L0X_PORT, GRP_VL53L0X_SHUT2_PIN)))

#define USE_I2C_2V8 1

#define Default_Mode 0
#define HIGH_ACCURACY 1
#define LONG_RANGE 2
#define HIGH_SPEED 3

typedef struct packed_2 {
    FixPoint1616_t signalLimit;
    FixPoint1616_t sigmaLimit;
    uint32_t timingBudget;
    uint8_t preRangeVcselPeriod;
    uint8_t finalRangeVcselPeriod;

} mode_data;

extern mode_data Mode_data[];
extern uint8_t AjustOK;

extern VL53L0X_Dev_t vl53l0x_dev0;
extern VL53L0X_Dev_t vl53l0x_dev1;
extern VL53L0X_Dev_t vl53l0x_dev2;
extern uint16_t distancebuffer[3];
VL53L0X_Error vl53l0x_init(VL53L0X_Dev_t *dev, u8 vl53l0_x_id);
VL53L0X_Error vl53l0x_init_all();
void print_pal_error(VL53L0X_Error Status);
void mode_string(u8 mode, char *buf);
void vl53l0x_test(void);
void vl53l0x_reset(VL53L0X_Dev_t *dev);
void vl53l0x_getdistance();
void vl53l0x_info(void);
void One_measurement(u8 mode);
#endif
