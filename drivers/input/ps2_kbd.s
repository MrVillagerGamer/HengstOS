global ints_wrapper_33
extern ps2_kbd_handler

ints_wrapper_33:
	pushad
	push ds
	push es
	push fs
	push gs
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	call ps2_kbd_handler
	pop gs
	pop fs
	pop es
	pop ds
	popad
	iretd
