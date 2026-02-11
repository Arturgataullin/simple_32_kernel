#pragma once

void serial_init(void);
void serial_putc(char c);
void serial_print(const char* str);
void serial_print_hex(uint32_t val);

