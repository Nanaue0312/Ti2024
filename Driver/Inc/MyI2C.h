#ifndef __MYI2C_H
#define __MYI2C_H

#ifndef GPIO_PIN_SCL_PORT
#define GPIO_PIN_SCL_PORT GPIO_I2C_PORT
#endif

#ifndef GPIO_PIN_SDA_PORT
#define GPIO_PIN_SDA_PORT GPIO_I2C_PORT
#endif
//设置SDA输出模式
#define SDA_OUT()                                                                                                      \
    {                                                                                                                  \
        DL_GPIO_initDigitalOutput(GPIO_I2C_PIN_SDA_IOMUX);                                                             \
        DL_GPIO_setPins(GPIO_PIN_SDA_PORT, GPIO_I2C_PIN_SDA_PIN);                                                      \
        DL_GPIO_enableOutput(GPIO_PIN_SDA_PORT, GPIO_I2C_PIN_SDA_PIN);                                                 \
    }

#define SDA_IN()                                                                                                       \
    { DL_GPIO_initDigitalInput(GPIO_I2C_PIN_SDA_IOMUX); }

//获取SDA引脚的电平变化
#define SDA_GET() (((DL_GPIO_readPins(GPIO_PIN_SDA_PORT, GPIO_I2C_PIN_SDA_PIN) & GPIO_I2C_PIN_SDA_PIN) > 0) ? 1 : 0)

//SDA与SCL输出
#define SDA(x)                                                                                                         \
    ((x) ? (DL_GPIO_setPins(GPIO_PIN_SDA_PORT, GPIO_I2C_PIN_SDA_PIN))                                                  \
         : (DL_GPIO_clearPins(GPIO_PIN_SDA_PORT, GPIO_I2C_PIN_SDA_PIN)))

#define SCL(x)                                                                                                         \
    ((x) ? (DL_GPIO_setPins(GPIO_PIN_SCL_PORT, GPIO_I2C_PIN_SCL_PIN))                                                  \
         : (DL_GPIO_clearPins(GPIO_PIN_SCL_PORT, GPIO_I2C_PIN_SCL_PIN)))

void IIC_Start();
void IIC_Stop();
void IIC_Send_Ack(uint8_t ack);
uint8_t IIC_Wait_Ack();
void IIC_Write(uint8_t data);
uint8_t IIC_Read();
#endif