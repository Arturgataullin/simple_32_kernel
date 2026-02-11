AS=nasm
CC=gcc
LD=ld

CFLAGS=-m32 -ffreestanding -fno-pic -fno-stack-protector -nostdlib -nodefaultlibs -Include 
LDFLAGS=-m elf_i386 -T link.ld

OBJS=boot.o gdt.o isr.o ports.o kernel.o keyboard.o keys.o pit.o pong.o screen.o 

all: kernel.bin

%.o: %.asm
	$(AS) -f elf32 $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

clean:
	rm -f *.o kernel.bin

