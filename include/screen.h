#pragma once
#include <stdint.h>

enum {
    VGA_W = 80,
    VGA_H = 25
};

// Call once, at the start of kmain, with the Multiboot v1 EAX/EBX values.
// Returns 1 for 32-bpp framebuffer, 0 for VGA text fallback.
int screen_init(uint32_t multiboot_magic, uint32_t multiboot_info_addr);

void screen_clear(uint8_t color, char ch);
void screen_put(int x, int y, uint8_t color, char ch);
void screen_present(void);
void screen_puts(int x, int y, uint8_t color, const char* str);
void screen_put_hex(int x, int y, uint8_t color, uint16_t val);
