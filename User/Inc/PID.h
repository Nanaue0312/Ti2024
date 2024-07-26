#ifndef __PID_H
#define __PID_H

/**
 * PID结构体
 */
typedef struct {
    float target;  // 目标值
    float now;     // 当前值
    float bias[3]; // 误差数组
    float p, i, d; // kp,ki,kd
    float pout, iout, dout;
    float out; // 占空比输出值
} t_PID;

/**
 * @brief 左轮速度环pid
 */
extern t_PID PID_Speed_Left;

/**
 * @brief 右轮速度环pid
 */
extern t_PID PID_Speed_Right;

/**
 * @brief 左轮位置环pid
 * 
 */
extern t_PID PID_Position_Left;

/**
 * @brief 右轮位置环pid
 * 
 */
extern t_PID PID_Position_Right;

void PID_Init(t_PID *pid, float p, float i, float d, float target);

void PID_Reset(t_PID *pid);

void PID_SetTarget(t_PID *pid, float target);

void Incremental_PID(t_PID *pid);

void Position_PID(t_PID *pid);

void PID_Out_Limit(t_PID *pid);

#endif