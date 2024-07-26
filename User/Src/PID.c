#include "stdio.h"
#include "PID.h"

t_PID PID_Speed_Left;
t_PID PID_Speed_Right;

void PID_Cal(t_PID *pid) {
    pid->bias[0] = pid->target - pid->now;
    if (pid->pidMode == DELTA_PID) {
        pid->pout = pid->p * (pid->bias[0] - pid->bias[1]);
        pid->iout = pid->i * pid->bias[0];
        pid->dout = pid->d * (pid->bias[0] - 2 * pid->bias[1] + pid->bias[2]);
        pid->out += pid->pout + pid->iout + pid->dout;
    } else if (pid->pidMode == POSITION_PID) {
        pid->pout = pid->p * pid->bias[0];
        pid->iout += pid->i * pid->bias[0];
        pid->dout = pid->d * (pid->bias[0] - pid->bias[1]);
        pid->out = pid->pout + pid->iout + pid->dout;
    }
    pid->bias[2] = pid->bias[1];
    pid->bias[1] = pid->bias[0];
}

void PID_Out_Limit(float *ccr) {
    if (*ccr >= 6000) {
        *ccr = 6000;
    } else if (*ccr <= -6000) {
        *ccr = -6000;
    }
}
