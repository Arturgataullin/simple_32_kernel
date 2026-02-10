#include <stdint.h>
#include "include/ports.h"

void keyboard_handler_main(void) {
	uint8_t status = read_port(0x64);
	if ((status & 1) == 0) return;
	uint8_t sc = read_port(0x60);

	write_port(0x20, 0x20); // sendind EOI PIC
}
