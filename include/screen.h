#pragma once
#include <stdint.h>

enum {
    VGA_W = 80,
    VGA_H = 25
}; // compile time const to use in static arrays

void screen_clear(uint8_t color, char ch);
void screen_put(int x, int y, uint8_t color, char ch);
void screen_present(void);
void screen_puts(int x, int y, uint8_t color, const char* str);
void screen_put_hex(int x, int y, uint8_t color, uint16_t val);
