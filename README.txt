PongOS UEFI framebuffer screen backend
======================================

The archive replaces only screen.c and include/screen.h, and adds
include/font8x8.h. It does NOT overwrite your modified boot.asm/kernel.c.

Important: retain the tested Multiboot graphics mode request in boot.asm and
the passing of EAX/EBX to kmain. Restore the normal kmain body instead of
framebuffer_test(), and initialize the screen BEFORE the other devices:

#include "include/screen.h"

void kmain(uint32_t magic, uint32_t mbi) {
    screen_init(magic, mbi);
    serial_init();
    idt_init();
    pit_init(20);
    __asm__ volatile("sti");
    pong_run();
}

Build and replace the kernel copied for GRUB:
    make clean && make
    grub-file --is-x86-multiboot kernel.elf && echo OK
    sudo cp kernel.elf /boot/pongos/kernel.elf
    sudo reboot

A font for the glyphs actually used by Pong is included, not a general font.
GPU framebuffer is restricted to 32-bpp RGB below 4 GiB for this first step.
If no compatible framebuffer is detected, it falls back to VGA text mode.
