global keyboard_handler
global load_idt
global irq0_handler

extern keyboard_handler_main
extern pit_irq0_tick

irq0_handler:
	pusha
	call pit_irq0_tick
	mov al, 0x20
	out 0x20, al
	popa
	iretd

keyboard_handler:
	pusha
	call keyboard_handler_main
	popa
	iretd

load_idt:
	mov eax, [esp + 4] 
	lidt [eax]
	ret
