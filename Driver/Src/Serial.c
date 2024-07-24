#include "ti_msp_dl_config.h"
#include "Serial.h"
#include "stdio.h"
#include "string.h"
/**
 * 开启串口接收中断
 */
void Serial_Init() {
    NVIC_ClearPendingIRQ(UART_0_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_0_INST_INT_IRQN);
}

void UART_0_INST_IRQHandler() {
    switch (DL_UART_Main_getPendingInterrupt(UART_0_INST)) {
        case DL_UART_MAIN_IIDX_RX:
            // 接收中断
            break;
        default:
            break;
    }
}

int fputc(int c, FILE *stream) {
    DL_UART_Main_transmitDataBlocking(UART_0_INST, c);
    return c;
}
int fputs(const char *restrict s, FILE *restrict stream) {
    uint16_t len = strlen(s);

    for (int i = 0; i < len; i++) {
        DL_UART_Main_transmitDataBlocking(UART_0_INST, s[i]);
    }
    return len;
}
int puts(const char *s) {
    int count = fputs(s, stdout);
    count += fputs("\n", stdout);
    return count;
}