ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)
SECTIONS 
{
	. = 1M;
	.text ALIGN(4096) : {
		*(.mboot)
		*(.text)
	}

	.data ALIGN (4096) : {
		*(.data)
		*(.rodata*)
	}

	.bss ALIGN (4096) : {
		_sbss = .;
		*(COMMON)
		*(.bss)
		_ebss = .;
	}

	/DISCARD/ : {
		*(.eh_frame);
		*(.comment*);
	}
}
