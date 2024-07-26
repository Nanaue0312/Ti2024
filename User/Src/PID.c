#include "stdio.h"
#include "PID.h"

t_PID PID_Speed_Left;
t_PID PID_Speed_Right;
// t_PID PID_Position_Left;
// t_PID PID_Position_Right;
/**
 * @brief 初始化PID
 *
 * @param pid 需要初始化的PID结构体
 * @param p p参数
 * @param i i参数
 * @param d d参数
 * @param target 目标值,时间间隔10ms
 */
void PID_Init(t_PID *pid, float p, float i, float d, float target) {
    pid->p = p;
    pid->i = i;
    pid->d = d;
    pid->target = target;
}

/**
 * @brief 重置PID积分量
 * @param pid PID结构体
 */
void PID_Reset(t_PID *pid) {
    pid->pout = 0;
    pid->dout = 0;
    pid->iout = 0;
}

/**
 * 设置PID结构体目标值
 *
 * @param pid 需要设置的PID结构体
 * @param target 目标值
 */
void PID_SetTarget(t_PID *pid, float target) { pid->target = target; }
/**
 * @brief 增量式PID
 * 
 * @param pid PID结构体
 */
void Incremental_PID(t_PID *pid) {
    pid->bias[0] = pid->target - pid->now;
    pid->pout = pid->p * (pid->bias[0] - pid->bias[1]);
    pid->iout = pid->i * pid->bias[0];
    pid->dout = pid->d * (pid->bias[0] - 2 * pid->bias[1] + pid->bias[2]);
    pid->out += pid->pout + pid->iout + pid->dout;
    pid->bias[2] = pid->bias[1];
    pid->bias[1] = pid->bias[0];
    PID_Out_Limit(pid);
}

/**
 * @brief 位置式PID控制
 * 
 * @param pid 位置式PID结构体
 */
void Position_PID(t_PID *pid) {
    pid->bias[0] = pid->target - pid->now;
    pid->pout = pid->p * pid->bias[0];
    pid->iout += pid->i * pid->bias[0];
    if (pid->iout >= 5000) {
        pid->iout = 5000;
    } else if (pid->iout <= -5000) {
        pid->iout = -5000;
    }
    pid->dout = pid->d * (pid->bias[0] - pid->bias[1]);
    pid->out = pid->pout + pid->iout + pid->dout;
    pid->bias[1] = pid->bias[0];
    PID_Out_Limit(pid);
}

void PID_Out_Limit(t_PID *pid) {
    if (pid->out >= 5000) {
        pid->out = 5000;
    } else if (pid->out <= -5000) {
        pid->out = -5000;
    }
}
