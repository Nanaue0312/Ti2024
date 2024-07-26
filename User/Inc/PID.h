#ifndef __PID_H
#define __PID_H
/**
 * PID类型
 */
typedef enum {
    POSITION_PID, // 位置式PID
    DELTA_PID     // 增量式PID
} PID_MODE;

/**
 * PID结构体
 */
typedef struct {
    float target;  // 目标值
    float now;     // 当前值
    float bias[3]; // 误差数组
    float p, i, d; // kp,ki,kd
    float pout, iout, dout;
    float out;        // 占空比输出值
    PID_MODE pidMode; // pid类型
} t_PID;
/**
 * @brief 左轮pid
 */
extern t_PID PID_Speed_Left;
/**
 * @brief 右轮pid
 */
extern t_PID PID_Speed_Right;

#endif