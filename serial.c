#include <stdint.h>
#include "include/serial.h"
#include "include/ports.h"

#define COM1 0x3F8

void serial_init(void) {
	write_port(COM1 + 1, 0x00);
	write_port(COM1 + 3, 0x80);
	write_port(COM1 + 0, 0x03);
	write_port(COM1 + 1, 0x00);
	write_port(COM1 + 3, 0x03);
	write_port(COM1 + 2, 0xC7);
	write_port(COM1 + 4, 0x0B);
}

static int serial_ready(void) {
	return (read_port(COM1 + 5) & 0x20) != 0;
}

void serial_putc(char c) {
	while (!serial_ready()) {}
	write_port(COM1, (uint8_t) c);
}

void serial_print(const char* str) {
	while (*str) {
		char c = *str++; 
		if (c == '\n') serial_putc('\r');
		serial_putc(c);
	}
}


void serial_print_hex(uint32_t val) {
	static const char hex[] = "0123456789ABCDE";
	serial_print("0x");
	for (int i = 60; i > 0; i -= 4)
		serial_putc(hex[(val >> i) & 0xF]);
}

