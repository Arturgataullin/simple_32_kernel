#include "include/pit.h"
#include "include/ports.h"
//PIT - programmable interval timer

static volatile uint32_t g_ticks = 0;

void pit_irq0_tick(void) { g_ticks++; } // calls from IRQ0

uint32_t pit_ticks(void) { return g_ticks; }

// configures PIT to hz times per second
void pit_init(uint32_t hz) {
    uint32_t div = 1193182u / (hz ? hz : 60u);
    if (div < 1) div = 1;
    if (div > 0xFFFF) div = 0xFFFF;

    write_port(0x43, 0x36); // 0x43 - port for control word
    // 0x36 = 0011 0110 
    // 6-7 bits - channel; 00 - channel 0, connceting for IRQ0
    // 4-5 bits - режим доступа 11 - LSB -> MSB (сначала младший потом старший байт)
    // 1-3 bits - режим работы; 011
    // 0 bit - 0 - binary counter
    write_port(0x40, (uint8_t)(div & 0xFF)); // младший
    write_port(0x40, (uint8_t)((div >> 8) & 0xFF)); // старший
}