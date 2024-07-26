#ifndef __LM75A_H
#define __LM75A_H

// LM75A Reg ADDR
#define LM75A_Addr 0x90
#define Temp_Reg 0x00   // 温度寄存器
#define Config_Reg 0x01 // 配置寄存器
// end
extern float temperature;
void LM75_Init();
uint16_t LM75_ReadTemp();
#endif