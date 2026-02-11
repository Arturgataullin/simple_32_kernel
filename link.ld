OUTPUT_FORMAT(elf32-i386)

ENTRY(start)
SECTIONS
 {
  . = 0x100000;
  .text : {  
	  KEEP(*(.multiboot))
	  *(.text*) 
   }
  .rodata : { *(.rodata*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) }
 }
