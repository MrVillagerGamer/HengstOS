global ints_wrapper_128
extern sys_handler

ints_wrapper_128:
	pushad
	push gs
	push fs
	push es
	push ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	pop eax
	push eax
	call sys_handler
	pop eax
	pop ds
	pop es
	pop fs
	pop gs
	popad
	iretd
