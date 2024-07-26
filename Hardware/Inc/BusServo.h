#ifndef __BUSSERVO_H
#define __BUSSERVO_H
/**
 * @brief 用于指定不同类型总线舵机命令的枚举类型。
 */
typedef enum {
    /** 
     * @brief 更改舵机角度的命令。
     * 
     * @param id        舵机的ID。
     * @param pulse     PWM脉冲宽度（范围：500-2500，4位数字）。
     * @param time      时间持续（范围：0-9999 ms，4位数字）。
     */
    BusServo_CHANGE_ANGLE,

    /** 
     * @brief 获取舵机固件版本的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_GET_VERSION,
    /** 
     * @brief 获取舵机ID的命令。
     */
    BusServo_GET_ID,
    /** 
     * @brief 设置舵机ID的命令。
     * 
     * @param id        舵机的当前ID。
     * @param new_id    要设置的新ID。
     */
    BusServo_SET_ID,

    /** 
     * @brief 释放舵机扭矩的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_RELEASE_TORQUE,

    /** 
     * @brief 恢复舵机扭矩的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_RESUME_TORQUE,

    /** 
     * @brief 获取舵机当前工作模式的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_GET_MODE,

    /** 
     * @brief 设置舵机工作模式的命令。
     * 
     * @param id        舵机的ID。
     * @param mode      要设置的模式（1-8）。
     * 1:舵机模式 270 度顺时针
     * 2:舵机模式 270 度逆时针
     * 3:舵机模式 180 度顺时针
     * 4:舵机模式 180 度逆时针
     * 5:马达模式 360 度定圈顺时针模式
     * 6:马达模式 360 度定圈逆时针模式
     * 7:马达模式 360 度定时顺时针模式
     * 8:马达模式 360 度定时逆时针模式
     */
    BusServo_SET_MODE,

    /** 
     * @brief 获取舵机当前位置的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_GET_POSITION,

    /** 
     * @brief 暂停舵机当前操作的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_PAUSE,

    /** 
     * @brief 继续舵机暂停操作的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_CONTINUE,

    /** 
     * @brief 停止舵机当前操作的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_STOP,

    /** 
     * @brief 设置舵机通信波特率的命令。
     * 
     * @param id        舵机的ID。
     * @param rate      要设置的波特率（1-8）。
     */
    BusServo_SET_BAUD_RATE,

    /** 
     * @brief 设置舵机中心位置的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_SET_CENTER,

    /** 
     * @brief 设置舵机启动位置的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_SET_START_POSITION,

    /** 
     * @brief 移除舵机启动位置的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_REMOVE_START_POSITION,

    /** 
     * @brief 重置舵机启动位置的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_RESET_START_POSITION,

    /** 
     * @brief 设置舵机最小值的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_SET_MIN_VALUE,

    /** 
     * @brief 设置舵机最大值的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_SET_MAX_VALUE,

    /** 
     * @brief 执行工厂重置的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_FACTORY_RESET,

    /** 
     * @brief 获取舵机当前温度和电压的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_GET_TEMPERATURE_VOLTAGE,

    /** 
     * @brief 获取舵机当前温度和电压设置的命令。
     * 
     * @param id        舵机的ID。
     */
    BusServo_GET_TEMPERATURE_VOLTAGE_SETTINGS,

    /** 
     * @brief 设置释放扭矩阈值温度的命令。
     * 
     * @param id        舵机的ID。
     * @param threshold 温度阈值。
     */
    BusServo_SET_RELEASE_TORQUE_THRESHOLD

} BusServoCommandType;

extern char cmd[20];

void BusServo_SendCommand(BusServoCommandType cmd_type, uint8_t id, uint16_t param1, uint16_t param2);
#endif