#include "UserFilters.h"

DIFF_INT_FILTER FILTER_Speed_LEFT;
DIFF_INT_FILTER FILTER_Speed_RIGHT;

void Difference_Integration_Filtering(DIFF_INT_FILTER *P) {
    int32_t tempdiff;
    //-------------------------------------------------------
    //数据进行滤波处理
    P->Difference = P->In - P->out;
    P->DiffIntegral = P->DiffIntegral + P->Difference;
    tempdiff = P->DiffIntegral;
    P->out = (tempdiff * P->gain) >> 15;
}