#ifndef __OLED_H
#define __OLED_H
#include "ti_msp_dl_config.h"

#define OLED_ADDR 0x78

void OLED_Init(void);

void OLED_Init();

void OLED_SetCursor(uint8_t x, uint8_t y);

void OLED_Clear();

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);

void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);

void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);

void OLED_ShowFloatNum(uint8_t Line, uint8_t Column, double Number, uint8_t IntLength, uint8_t FraLength);
#endif
