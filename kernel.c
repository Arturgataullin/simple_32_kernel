#include <stdint.h>
#include "include/ports.h"
#include "include/pit.h"
#include "include/idt.h"
#include "include/pong.h"
#include "include/serial.h"

// IDT stuctures

typedef struct __attribute__((packed)) {
	uint16_t off_lo;
	uint16_t sel;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t off_hi;
} idt_entry_t;

typedef struct __attribute__((packed)) {
	uint16_t limit;
	uint32_t base;
} idtr_t;

static idt_entry_t idt[256];
static idtr_t idtr;

extern void irq0_handler(void);
extern void keyboard_handler(void);
extern void load_idt(void* idtr_ptr);

static void idt_set_gate(uint8_t vec, void (*isr)(void)) {
	uint32_t addr = (uint32_t)(uintptr_t)isr;
	idt[vec].off_lo = (uint16_t)(addr & 0xFFFF);
	idt[vec].sel = 0x08; // kernel code segment
	idt[vec].zero = 0;
	idt[vec].type_attr = 0x8E;
	idt[vec].off_hi = (uint16_t)(addr >> 16);
}

// PIC remap & IRQ mask

static void pic_remap(uint8_t off1, uint8_t off2) {
	uint8_t a1 = read_port(0x21);
	uint8_t a2 = read_port(0xA1);

	write_port(0x20, 0x11);
	write_port(0xA0, 0x11);

	write_port(0x21, off1);
	write_port(0xA1, off2);

	write_port(0x21, 0x04);
	write_port(0xA1, 0x02);

	write_port(0x21, 0x01);
	write_port(0xA1, 0x01);

	write_port(0x21, a1);
	write_port(0xA1, a2);
}

static void pic_set_mask(uint8_t irq_line, int masked) {
	uint16_t port = (irq_line < 8) ? 0x21 : 0xA1;
	uint8_t bit = (irq_line < 8) ? irq_line : (irq_line - 8);

	uint8_t v = read_port(port);
	if (masked) v |= (1u << bit);
	else v &= ~(1u << bit);
	write_port(port, v);
}

void idt_init(void) {
	// zero IDT
	for (int i = 0; i < 256; i++) {
		idt[i] = (idt_entry_t){0};
	}

	pic_remap(0x20, 0x28);
	idt_set_gate(0x20, irq0_handler);
	idt_set_gate(0x21, keyboard_handler);

	// маскируем все входы PIC кроме IRQ1
	write_port(0x21, 0xFF);
	write_port(0xA1, 0xFF);
	pic_set_mask(1, 0);
	pic_set_mask(0, 0);

	// заполняем idtr
	idtr.limit = (uint16_t)(sizeof(idt) - 1);
	idtr.base = (uint32_t)(uintptr_t)idt;
	load_idt(&idtr);

}

void kmain(void) {
	serial_init();

	idt_init();
	pit_init(20);
	
	__asm__ volatile("sti"); // включаем прерывания

	pong_run();
}
