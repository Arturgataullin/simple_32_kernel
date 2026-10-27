bits 32

section .multiboot
align 4

dd 0x1BADB002
dd 0x04
dd -(0x1BADB002 + 0x04)

dd 0, 0, 0, 0, 0
dd 0  ; graphical mode
dd 0  ; width: any
dd 0  ; height: any
dd 0  ; depth: any

section .text
global start
extern kmain
extern gdt_flush

start:
	cli

    mov esi, eax
    mov edi, ebx
	mov esp, stack_top

	call gdt_flush

	push edi
    push esi
    call kmain

.hang:
    cli
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
	resb 8192
stack_top: 
