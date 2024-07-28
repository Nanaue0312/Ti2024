#include "ti_msp_dl_config.h"
#include "vl53l0x_i2c.h"
#include "MyI2C.h"
#include "Delay.h"

/******************************************************************
 * º¯ Êý Ãû ³Æ£ºSend_Byte
 * º¯ Êý Ëµ Ã÷£ºÐ´ÈëÒ»¸ö×Ö½Ú
 * º¯ Êý ÐÎ ²Î£ºdatÒªÐ´ÈËµÄÊý¾Ý
 * º¯ Êý ·µ »Ø£ºÎÞ
 * ×÷       Õß£ºLC
 * ±¸       ×¢£ºÎÞ
******************************************************************/
void Send_Byte(uint8_t dat) {
    int i = 0;
    SDA_OUT();
    SCL(0); //À­µÍÊ±ÖÓ¿ªÊ¼Êý¾Ý´«Êä

    for (i = 0; i < 8; i++) {
        SDA((dat & 0x80) >> 7);
        delay_us(1);
        SCL(1);
        delay_us(5);
        SCL(0);
        delay_us(5);
        dat <<= 1;
    }
}

/******************************************************************
 * º¯ Êý Ãû ³Æ£ºRead_Byte
 * º¯ Êý Ëµ Ã÷£ºIIC¶ÁÊ±Ðò
 * º¯ Êý ÐÎ ²Î£ºack=1Ê±£¬·¢ËÍACK£¬ack=0£¬·¢ËÍnACK   
 * º¯ Êý ·µ »Ø£º¶Áµ½µÄÊý¾Ý
 * ×÷       Õß£ºLC
 * ±¸       ×¢£ºÎÞ
******************************************************************/
unsigned char Read_Byte(unsigned char ack) {
    unsigned char i, receive = 0;
    SDA_IN(); //SDAÉèÖÃÎªÊäÈë
    for (i = 0; i < 8; i++) {
        SCL(0);
        delay_us(4);
        SCL(1);
        receive <<= 1;
        if (SDA_GET())
            receive++;
        delay_us(4); //1
    }
    if (!ack)
        IIC_Send_Ack(1); //·¢ËÍnACK
    else
        IIC_Send_Ack(0); //·¢ËÍACK
    return receive;
}

//IICÐ´Ò»¸ö×Ö½ÚÊý¾Ý
u8 VL_IIC_Write_1Byte(u8 SlaveAddress, u8 REG_Address, u8 REG_data) {
    IIC_Start();
    Send_Byte(SlaveAddress);
    if (IIC_Wait_Ack()) {
        IIC_Stop(); //ÊÍ·Å×ÜÏß
        return 1;   //Ã»Ó¦´ðÔòÍË³ö
    }
    Send_Byte(REG_Address);
    IIC_Wait_Ack();
    Send_Byte(REG_data);
    IIC_Wait_Ack();
    IIC_Stop();

    return 0;
}

//IIC¶ÁÒ»¸ö×Ö½ÚÊý¾Ý
u8 VL_IIC_Read_1Byte(u8 SlaveAddress, u8 REG_Address, u8 *REG_data) {
    IIC_Start();
    Send_Byte(SlaveAddress); //·¢Ð´ÃüÁî
    if (IIC_Wait_Ack()) {
        IIC_Stop(); //ÊÍ·Å×ÜÏß
        return 1;   //Ã»Ó¦´ðÔòÍË³ö
    }
    Send_Byte(REG_Address);
    IIC_Wait_Ack();
    IIC_Start();
    Send_Byte(SlaveAddress | 0x01); //·¢¶ÁÃüÁî
    IIC_Wait_Ack();
    *REG_data = Read_Byte(0);
    IIC_Stop();

    return 0;
}

//IICÐ´n×Ö½ÚÊý¾Ý
u8 VL_IIC_Write_nByte(u8 SlaveAddress, u8 REG_Address, u16 len, u8 *buf) {
    IIC_Start();
    Send_Byte(SlaveAddress); //·¢Ð´ÃüÁî
    if (IIC_Wait_Ack()) {
        IIC_Stop(); //ÊÍ·Å×ÜÏß
        return 1;   //Ã»Ó¦´ðÔòÍË³ö
    }
    Send_Byte(REG_Address);
    IIC_Wait_Ack();
    while (len--) {
        Send_Byte(*buf++); //·¢ËÍbuffµÄÊý¾Ý
        IIC_Wait_Ack();
    }
    IIC_Stop(); //ÊÍ·Å×ÜÏß

    return 0;
}

//IIC¶Án×Ö½ÚÊý¾Ý
u8 VL_IIC_Read_nByte(u8 SlaveAddress, u8 REG_Address, u16 len, u8 *buf) {
    IIC_Start();
    Send_Byte(SlaveAddress); //·¢Ð´ÃüÁî
    if (IIC_Wait_Ack()) {
        IIC_Stop(); //ÊÍ·Å×ÜÏß
        return 1;   //Ã»Ó¦´ðÔòÍË³ö
    }
    Send_Byte(REG_Address);
    IIC_Wait_Ack();

    IIC_Start();
    Send_Byte(SlaveAddress | 0x01); //·¢¶ÁÃüÁî
    IIC_Wait_Ack();
    while (len) {
        if (len == 1) {
            *buf = Read_Byte(0);
        } else {
            *buf = Read_Byte(1);
        }
        buf++;
        len--;
    }
    IIC_Stop(); //ÊÍ·Å×ÜÏß

    return 0;
}

//VL53L0X Ð´¶à¸öÊý¾Ý
//address:µØÖ·
//index:Æ«ÒÆµØÖ·
//pdata:Êý¾ÝÖ¸Õë
//count:³¤¶È ×î´ó65535
u8 VL53L0X_write_multi(u8 address, u8 index, u8 *pdata, u16 count) {
    u8 status = STATUS_OK;

    if (VL_IIC_Write_nByte(address, index, count, pdata)) {
        status = STATUS_FAIL;
    }
    return status;
}

//VL53L0X ¶Á¶à¸öÊý¾Ý
//address:µØÖ·
//index:Æ«ÒÆµØÖ·
//pdata:Êý¾ÝÖ¸Õë
//count:³¤¶È ×î´ó65535
u8 VL53L0X_read_multi(u8 address, u8 index, u8 *pdata, u16 count) {
    u8 status = STATUS_OK;

    if (VL_IIC_Read_nByte(address, index, count, pdata)) {
        status = STATUS_FAIL;
    }

    return status;
}

//VL53L0X Ð´1¸öÊý¾Ý(µ¥×Ö½Ú)
//address:µØÖ·
//index:Æ«ÒÆµØÖ·
//data:Êý¾Ý(8Î»)
u8 VL53L0X_write_byte(u8 address, u8 index, u8 data) {
    u8 status = STATUS_OK;

    status = VL53L0X_write_multi(address, index, &data, 1);

    return status;
}

//VL53L0X Ð´1¸öÊý¾Ý(Ë«×Ö½Ú)
//address:µØÖ·
//index:Æ«ÒÆµØÖ·
//data:Êý¾Ý(16Î»)
u8 VL53L0X_write_word(u8 address, u8 index, u16 data) {
    u8 status = STATUS_OK;

    u8 buffer[2];

    //½«16Î»Êý¾Ý²ð·Ö³É8Î»
    buffer[0] = (u8)(data >> 8);   //¸ß°ËÎ»
    buffer[1] = (u8)(data & 0xff); //µÍ°ËÎ»

    if (index % 2 == 1) {
        //´®ÐÐÍ¨ÐÅ²»ÄÜ´¦Àí¶Ô·Ç2×Ö½Ú¶ÔÆë¼Ä´æÆ÷µÄ×Ö½Ú
        status = VL53L0X_write_multi(address, index, &buffer[0], 1);
        status = VL53L0X_write_multi(address, index, &buffer[0], 1);
    } else {
        status = VL53L0X_write_multi(address, index, buffer, 2);
    }

    return status;
}

//VL53L0X Ð´1¸öÊý¾Ý(ËÄ×Ö½Ú)
//address:µØÖ·
//index:Æ«ÒÆµØÖ·
//data:Êý¾Ý(32Î»)
u8 VL53L0X_write_dword(u8 address, u8 index, u32 data) {
    u8 status = STATUS_OK;

    u8 buffer[4];

    //½«32Î»Êý¾Ý²ð·Ö³É8Î»
    buffer[0] = (u8)(data >> 24);
    buffer[1] = (u8)((data & 0xff0000) >> 16);
    buffer[2] = (u8)((data & 0xff00) >> 8);
    buffer[3] = (u8)(data & 0xff);

    status = VL53L0X_write_multi(address, index, buffer, 4);

    return status;
}

//VL53L0X ¶Á1¸öÊý¾Ý(µ¥×Ö½Ú)
//address:µØÖ·
//index:Æ«ÒÆµØÖ·
//data:Êý¾Ý(8Î»)
u8 VL53L0X_read_byte(u8 address, u8 index, u8 *pdata) {
    u8 status = STATUS_OK;

    status = VL53L0X_read_multi(address, index, pdata, 1);

    return status;
}

//VL53L0X ¶Á¸öÊý¾Ý(Ë«×Ö½Ú)
//address:µØÖ·
//index:Æ«ÒÆµØÖ·
//data:Êý¾Ý(16Î»)
u8 VL53L0X_read_word(u8 address, u8 index, u16 *pdata) {
    u8 status = STATUS_OK;

    u8 buffer[2];

    status = VL53L0X_read_multi(address, index, buffer, 2);

    *pdata = ((u16)buffer[0] << 8) + (u16)buffer[1];

    return status;
}

//VL53L0X ¶Á1¸öÊý¾Ý(ËÄ×Ö½Ú)
//address:µØÖ·
//index:Æ«ÒÆµØÖ·
//data:Êý¾Ý(32Î»)
u8 VL53L0X_read_dword(u8 address, u8 index, u32 *pdata) {
    u8 status = STATUS_OK;

    u8 buffer[4];

    status = VL53L0X_read_multi(address, index, buffer, 4);

    *pdata = ((u32)buffer[0] << 24) + ((u32)buffer[1] << 16) + ((u32)buffer[2] << 8) + ((u32)buffer[3]);

    return status;
}
