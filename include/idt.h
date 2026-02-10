#pragma once
#include <stdint.h>

void idt_init(void);
void load_idt(void* idt_ptr);
