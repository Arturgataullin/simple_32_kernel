#pragma once
#include <stdint.h>

enum {
    VGA_W = 80,
    VGA_H = 25
}; // compile time const to use in static arrays

void screen_clear(uint8_t color, char ch);
void screen_put(int x, int y, uint8_t color, char ch);
void screen_present(void);
