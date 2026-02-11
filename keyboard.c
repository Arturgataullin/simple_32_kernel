#include <stdint.h>
#include "include/keys.h"
#include "include/ports.h"

void keyboard_handler_main(void) {
	uint8_t sc = read_port(0x60);
	keys_on_scancode(sc);
    
	write_port(0x20, 0x20); // sendind EOI PIC
}