#include "include/screen.h"

static volatile uint16_t* const VGA = (uint16_t*)0xB8000; // VGA symb consist of 2 bytes
static uint16_t back[VGA_W * VGA_H];

static inline uint16_t cell(uint8_t color, char ch) {
    return ((uint16_t)color << 8) | (uint8_t)ch;
}

void screen_clear(uint8_t color, char ch) {
    uint16_t v = cell(color, ch);
    for (int i = 0; i < VGA_H * VGA_W; i++) back[i] = v;
}

void screen_put(int x, int y, uint8_t color, char ch) {
    if ((unsigned)x >= VGA_W || (unsigned)y >= VGA_H) return;
    back[VGA_W * y + x] = cell(color, ch);
}

void screen_present(void) {
    for (int i = 0; i < VGA_H * VGA_W; i++) return VGA[i] = back[i];
}

