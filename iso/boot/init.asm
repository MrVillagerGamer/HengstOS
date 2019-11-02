[bits 32]
[org 0x0]

_start:
	mov eax, data
	int 0x80
	jmp $

data:
	dd 0
	dd str

str: db "Hello, user world!"

times 64*1024-($-$$) db 0
