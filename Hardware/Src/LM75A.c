#include "ti_msp_dl_config.h"
#include "MyI2C.h"
#include "stdio.h"
#include "LM75A.h"

float temperature;

void LM75_WriteReg(uint8_t RegAddress, uint8_t data) {
    IIC_Start();
    IIC_Write(LM75A_Addr); // 左移一位
    if (IIC_Wait_Ack()) {
        printf("Error, LM75_WriteReg - Device Address NACK\n");
        IIC_Stop();
        return;
    }
    IIC_Write(RegAddress);
    if (IIC_Wait_Ack()) {
        printf("Error, LM75_WriteReg - Register Address NACK\n");
        IIC_Stop();
        return;
    }
    IIC_Write(data);
    if (IIC_Wait_Ack()) {
        printf("Error, LM75_WriteReg - Data NACK\n");
        IIC_Stop();
        return;
    }
    IIC_Stop();
}

uint8_t LM75_ReadReg(uint8_t RegAddress) {
    uint8_t Data;
    IIC_Start();
    IIC_Write(LM75A_Addr); // 左移一位
    if (IIC_Wait_Ack()) {
        printf("Error, LM75_ReadReg - Device Address NACK\n");
        IIC_Stop();
        return 0; // 返回0作为错误值
    }
    IIC_Write(RegAddress);
    if (IIC_Wait_Ack()) {
        printf("Error, LM75_ReadReg - Register Address NACK\n");
        IIC_Stop();
        return 0; // 返回0作为错误值
    }
    IIC_Start();
    IIC_Write(LM75A_Addr | 0x01); // 读操作
    if (IIC_Wait_Ack()) {
        printf("Error, LM75_ReadReg - Device Address Read NACK\n");
        IIC_Stop();
        return 0; // 返回0作为错误值
    }
    Data = IIC_Read();
    IIC_Send_Ack(1);
    IIC_Stop();
    return Data;
}

uint16_t LM75_Read2Byte(uint8_t RegAddress) {
    uint16_t Data;
    IIC_Start();
    IIC_Write(LM75A_Addr); // 左移一位
    if (IIC_Wait_Ack()) {
        printf("Error, LM75_Read2Byte - Device Address NACK\n");
        IIC_Stop();
        return 0; // 返回0作为错误值
    }
    IIC_Write(RegAddress);
    if (IIC_Wait_Ack()) {
        printf("Error, LM75_Read2Byte - Register Address NACK\n");
        IIC_Stop();
        return 0; // 返回0作为错误值
    }
    IIC_Start();
    IIC_Write(LM75A_Addr | 0x01); // 读操作
    if (IIC_Wait_Ack()) {
        printf("Error, LM75_Read2Byte - Device Address Read NACK\n");
        IIC_Stop();
        return 0; // 返回0作为错误值
    }
    Data = IIC_Read() << 8;
    IIC_Send_Ack(0);
    Data |= IIC_Read();
    IIC_Send_Ack(1);
    IIC_Stop();
    return Data;
}
void LM75_Init() { LM75_WriteReg(Config_Reg, 0x00); }

uint16_t LM75_ReadTemp() { return LM75_Read2Byte(Temp_Reg); }