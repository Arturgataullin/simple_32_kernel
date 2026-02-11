bits 32

section .multiboot
align 4
dd 0x1BADB002
dd 0x00
dd -(0x1BADB002 + 0x00)

section .text
global start
extern kmain
extern gdt_flush

start:
	cli
	call gdt_flush
	mov esp, stack_top
	call kmain
	hlt

section .bss
align 16
stack_bottom:
	resb 8192
stack_top: 
