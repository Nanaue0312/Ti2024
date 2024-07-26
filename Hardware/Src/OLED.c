#include "ti_msp_dl_config.h"
#include "MyI2C.h"
#include "OLED.h"
#include "oledfont.h"
#include "Delay.h"
//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127

/**
 * @brief OLED发送命令字节
 * @param command 要发送的命令字节
 */
void OLED_SendCommand(uint8_t command) {
    IIC_Start();
    IIC_Write(OLED_ADDR | 0x00); // 发送读命令给从机
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        return;
    }
    IIC_Write(0x00); // 发送控制字节，0x80为非连续模式，下一个字节为命令
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        return;
    }
    IIC_Write(command); // 发送数据字节
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        return;
    }
}

/**
 * @brief OLED发送数据字节
 * @param command 要发送的数据字节
 */
void OLED_SendData(uint8_t data) {
    IIC_Start();
    IIC_Write(OLED_ADDR | 0x00); // 发送读命令给从机
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        return;
    }
    IIC_Write(0x40); // 发送控制字节，0x80为非连续模式，下一个字节为数据
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        return;
    }
    IIC_Write(data); // 发送数据字节
    if (IIC_Wait_Ack()) {
        IIC_Stop();
        return;
    }
    IIC_Stop();
}

void OLED_Init() {
    delay_ms(100);
    OLED_SendCommand(0xAE); // 关闭显示
    OLED_SendCommand(0xD5);
    OLED_SendCommand(0x80); // 设置时钟分频

    OLED_SendCommand(0xA8);
    OLED_SendCommand(0x3F);

    OLED_SendCommand(0xD3);
    OLED_SendCommand(0x00);

    OLED_SendCommand(0x40);

    OLED_SendCommand(0xA1);

    OLED_SendCommand(0xC8);

    OLED_SendCommand(0xDA);
    OLED_SendCommand(0x12);

    OLED_SendCommand(0x81);
    OLED_SendCommand(0xCF);

    OLED_SendCommand(0xD9);
    OLED_SendCommand(0xF1);

    OLED_SendCommand(0xDB);
    OLED_SendCommand(0x30);

    OLED_SendCommand(0xA4);

    OLED_SendCommand(0xA6);

    OLED_SendCommand(0x8D);
    OLED_SendCommand(0x14);

    OLED_SendCommand(0xAF);

    delay_ms(100);
    OLED_Clear();
}

/**
 * @brief 设置OLED的起始坐标
 * @param y 页地址，0-7
 * @param x 列地址，0-127
*/
void OLED_SetCursor(uint8_t y, uint8_t x) {
    OLED_SendCommand(0xB0 | y);
    OLED_SendCommand(0x10 | (x & 0xF0) >> 4);
    OLED_SendCommand(0x00 | (x & 0x0F));
}

/**
 * @brief 清屏
*/
void OLED_Clear() {
    for (int i = 0; i < 8; i++) {
        OLED_SetCursor(i, 0);
        for (int j = 0; j < 128; j++) {
            OLED_SendData(0x00);
        }
    }
}

/**
  * @brief  OLED显示一个字符
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的一个字符，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char) {
    uint8_t i;
    OLED_SetCursor((Line - 1) * 2, (Column - 1) * 8); //设置光标位置在上半部分
    for (i = 0; i < 8; i++) {
        OLED_SendData(asc2_1608[Char - ' '][i]); //显示上半部分内容
    }
    OLED_SetCursor((Line - 1) * 2 + 1, (Column - 1) * 8); //设置光标位置在下半部分
    for (i = 0; i < 8; i++) {
        OLED_SendData(asc2_1608[Char - ' '][i + 8]); //显示下半部分内容
    }
}

/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String) {
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y) {
    uint32_t Result = 1;
    while (Y--) {
        Result *= X;
    }
    return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length) {
    uint8_t i;
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length) {
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0) {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    } else {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

void OLED_ShowFloatNum(uint8_t Line, uint8_t Column, double Number, uint8_t IntLength, uint8_t FraLength) {
    uint32_t PowNum, IntNum, FraNum;
    if (Number >= 0) //数字大于等于0
    {
        OLED_ShowChar(Line, Column, '+'); //显示+号
    } else                                //数字小于0
    {
        OLED_ShowChar(Line, Column, '-'); //显示-号
        Number = -Number;                 //Number取负
    }

    /*提取整数部分和小数部分*/
    IntNum = Number;  //直接赋值给整型变量，提取整数
    Number -= IntNum; //将Number的整数减掉，防止之后将小数乘到整数时因数过大造成错误
    PowNum = OLED_Pow(10, FraLength); //根据指定小数的位数，确定乘数
    FraNum = round(Number * PowNum);  //将小数乘到整数，同时四舍五入，避免显示误差
    IntNum += FraNum / PowNum;        //若四舍五入造成了进位，则需要再加给整数

    /*显示整数部分*/
    OLED_ShowNum(Line, Column + 1, IntNum, IntLength);

    /*显示小数点*/
    OLED_ShowChar(Line, Column + (IntLength + 1), '.');

    /*显示小数部分*/
    OLED_ShowNum(Line, Column + (IntLength + 2), FraNum, FraLength);
}
