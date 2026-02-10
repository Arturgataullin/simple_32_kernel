global keyboard_handler
global load_idt

extern keyboard_handler_main

keyboard_handler:
	pusha
	call keyboard_handler_main
	popa
	iretd

load_idt:
	mov eax, [esp + 4] 
	lidt [eax]
	ret
