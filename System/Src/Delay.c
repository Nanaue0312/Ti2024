#include "ti_msp_dl_config.h"
#include "Delay.h"
void delay_us(unsigned long us) {
    while (us--)
        delay_cycles(CPUCLK_FREQ / 1000000);
}

void delay_ms(unsigned long ms) {
    while (ms--)
        delay_cycles(CPUCLK_FREQ / 1000);
}
void delay_1us(unsigned long us) { delay_us(us); }
void delay_1ms(unsigned long ms) { delay_ms(ms); }