#ifndef __USERFILTERS_H
#define __USERFILTERS_H

#include <stdint.h>
typedef struct {
    int16_t In;
    int16_t Difference;
    int16_t DiffSIGN;
    int32_t DiffIntegral;
    int16_t gain;
    int16_t out;
} DIFF_INT_FILTER;

void Difference_Integration_Filtering(DIFF_INT_FILTER *P);
#endif