#include "vl53l0x_gen.h"

#ifndef vu16
#define vu16 uint16_t
#endif

VL53L0X_RangingMeasurementData_t vl53l0x_data; // �������ṹ��
vu16 Distance_data = 0;                        // ����������

// VL53L0X ����ģʽ����
// dev:�豸I2C�����ṹ��
// mode: 0:Ĭ��;1:�߾���;2:������
VL53L0X_Error vl53l0x_set_mode(VL53L0X_Dev_t *dev, u8 mode) {
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;

    vl53l0x_reset(
        dev); // ��λvl53l0x(Ƶ���л�����ģʽ���׵��²ɼ��������ݲ�׼���������һ����)
    status = VL53L0X_StaticInit(dev);

    if (AjustOK != 0) // ��У׼����,д��У׼ֵ
    {
        status = VL53L0X_SetReferenceSpads(dev,
                                           Vl53l0x_data.refSpadCount,
                                           Vl53l0x_data.isApertureSpads); // �趨SpadsУ׼ֵ
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        delay_ms(2);
        status = VL53L0X_SetRefCalibration(dev, Vl53l0x_data.VhvSettings,
                                           Vl53l0x_data.PhaseCal); // �趨RefУ׼ֵ
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        delay_ms(2);
        status = VL53L0X_SetOffsetCalibrationDataMicroMeter(dev, Vl53l0x_data.OffsetMicroMeter); // �趨ƫ��У׼ֵ
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        delay_ms(2);
        status = VL53L0X_SetXTalkCompensationRateMegaCps(
            dev,
            Vl53l0x_data.XTalkCompensationRateMegaCps); // �趨����У׼ֵ
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        delay_ms(2);
    } else {
        status = VL53L0X_PerformRefCalibration(dev, &VhvSettings,
                                               &PhaseCal); // Ref�ο�У׼
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        delay_ms(2);
        status = VL53L0X_PerformRefSpadManagement(dev, &refSpadCount,
                                                  &isApertureSpads); // ִ�вο�SPAD����
        if (status != VL53L0X_ERROR_NONE)
            goto error;
        delay_ms(2);
    }
    status = VL53L0X_SetDeviceMode(dev, VL53L0X_DEVICEMODE_SINGLE_RANGING); // ʹ�ܵ��β���ģʽ
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1); // ʹ��SIGMA��Χ���
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckEnable(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1); // ʹ���ź����ʷ�Χ���
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckValue(dev,
                                        VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE,
                                        Mode_data[mode].sigmaLimit); // �趨SIGMA��Χ
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetLimitCheckValue(dev,
                                        VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE,
                                        Mode_data[mode].signalLimit); // �趨�ź����ʷ�Χ��Χ
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(
        dev, Mode_data[mode].timingBudget); // �趨��������ʱ��
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetVcselPulsePeriod(dev,
                                         VL53L0X_VCSEL_PERIOD_PRE_RANGE,
                                         Mode_data[mode].preRangeVcselPeriod); // �趨VCSEL��������
    if (status != VL53L0X_ERROR_NONE)
        goto error;
    delay_ms(2);
    status = VL53L0X_SetVcselPulsePeriod(dev,
                                         VL53L0X_VCSEL_PERIOD_FINAL_RANGE,
                                         Mode_data[mode].finalRangeVcselPeriod); // �趨VCSEL�������ڷ�Χ

error: // ������Ϣ
    if (status != VL53L0X_ERROR_NONE) {
        print_pal_error(status);
        return status;
    }
    return status;
}

// VL53L0X 单次距离测量函数
// dev:设备I2C参数结构体
// pdata:保存测量数据结构体
VL53L0X_Error vl53l0x_start_single_test(VL53L0X_Dev_t *dev, VL53L0X_RangingMeasurementData_t *pdata, char *buf) {
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    uint8_t RangeStatus;

    status = VL53L0X_PerformSingleRangingMeasurement(dev, pdata); // 执行单次测距并获取测距测量数据
    if (status != VL53L0X_ERROR_NONE)
        return status;

    RangeStatus = pdata->RangeStatus; // 获取当前测量状态
    memset(buf, 0x00, VL53L0X_MAX_STRING_LENGTH);
    VL53L0X_GetRangeStatusString(RangeStatus, buf); // 根据测量状态读取状态字符串

    Distance_data = pdata->RangeMilliMeter; // 保存最近一次测距测量数据

    return status;
}

// 启动普通测量
// dev：设备I2C参数结构体
// mode模式配置 0:默认;1:高精度;2:长距离
void vl53l0x_general_start(VL53L0X_Dev_t *dev, u8 mode) {
    static char buf[VL53L0X_MAX_STRING_LENGTH]; // 测试模式字符串字符缓冲区
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;  // 工作状态
    u8 i = 0;

    // mode_string(mode,buf);//显示当前配置的模式
    while (vl53l0x_set_mode(dev, mode)) // 配置测量模式
    {
        printf("Mode Set Error!!!\r\n");
        i++;
        if (i == 2)
            return;
    }

    while (Status == VL53L0X_ERROR_NONE) {
        Status = vl53l0x_start_single_test(dev, &vl53l0x_data, buf); // 执行一次测量
        printf("d: %4imm\r\n", Distance_data);                       // 打印测量距离
    }
    delay_ms(500);
    printf("Measurement is Error,Program Continue!\r\n"); // 如果出错会回到上面，重新开始
}

// vl53l0x普通测量模式测试
// dev:设备I2C参数结构体
void vl53l0x_general_test(VL53L0X_Dev_t *dev) {
    u8 mode = 0;

    vl53l0x_general_start(dev, mode);
    /*
  while(1)
  {
          vl53l0x_general_start(dev,mode);
          mode=0;

          delay_ms(500);
  }
  */
}
