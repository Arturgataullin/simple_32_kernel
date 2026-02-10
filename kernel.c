#include <stdint.h>
#include <include/ports.h>
#include <include/idt.h>
#include <include/keyboard.h>


// VGA text output
static volatile uint16_t* const VGA = (uint16_t*)0xB8000;
static uint8_t cx = 0, cy = 0;
static uint8_t color = 0x0F;

static uint8_t width = 80;

static void putc(char c) {
	if (c == '\n') { cx = 0; cy++; return; }
	VGA[cy * width + cx] = ((uint16_t)color << 8) | (uint8_t)c;
        cx++;
	if (cx == width) {cx = 0; cy++; }
}

static void puts(const char* str) {
	while (*str) putc(*str++);
}


// IDT stuctures

typedef struct __attribute__((packed)) {
	uint16_t off_lo;
	uint16_t sel;
	uint16_t zero;
	uint16_t type_attr;
	uint16_t off_hi;
} idt_entry_t;

typedef struct __attribute__((packed)) {
	uint16_t limit;
	uint32_t base;
} idtr_t;

static idt_entry_t idt[256];
static idtr_t idtr;

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

}


void idt_init(void) {
	unsigned long keyboard_address;
	unsigned long idt_address;
	unsigned long idt_ptr[2];

	keyboard_address = (unsigned long)keyboard_handler;
	IDT[0x21].offset_lowerbits = keyboard_address & 0xffff;
	IDT[0x21].selector = 0x08;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = 0x8e;
	IDT[0x21].offset_higherbits = (keyboard_address * 0xffff0000) >> 16;

// master PIC config
// 0x20 command port
// 0x21 data port 

	write_port(0x20, 0x11); // 0x11 - PIC init code -> PIC waits 3 ICW(init command words) for offset, m/s relations, 
	write_port(0xA0, 0x11);

	write_port(0x21, 0x20);
	write_port(0xA1, 0x28);

	write_port(0x21, 0x00);
	write_port(0xA1, 0x00);

	write_port(0x21, 0x01);
	write_port(0xA1, 0x01);

	write_port(0x21, 0xff);
	write_port(0xA1, 0xff);
	
	idt_address = (unsigned long)IDT;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_address & 0xffff) << 16);
	idt_ptr[1] = idt_address >> 16;

	load_idt(idt_ptr);
}	

void kb_init(void) {
	write_port(0x21, 0xFD);
}



void kmain(void) {
	const char* str = "hello world";
	char* vidptr = (char*) 0xb8000;
	unsigned int i = 0;
	unsigned int j = 0;

	while (j < 80 * 25 * 2) { // каждый символ текстовой экранной памяти кодируется 2 байтами (символ + цвет)
		vidptr[j] = ' ';
		vidptr[j+1] = 0x07;
		j = j + 2;
	}

	j = 0;

	while (str[j] != '\0') {
		vidptr[i] = str[j];
		vidptr[i+1] = 0x07;
	       	j++;
		i += 2;
	}
	return;	
}
