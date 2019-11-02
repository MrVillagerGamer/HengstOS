[bits 32]
[org 0x0]

_start:
	jmp $

sys_read:
	dd 1
	dd sys_read_param

sys_read_param:
	dd filename
	dd filebuf
	dd 512

sys_puts:
	dd 0
	dd sys_puts_param

sys_puts_param:
	dd str
	dd 0x07

filename: db "Z:\HengstOS\System32\test.txt", 0
filebuf: times 512 db 0
str: db "Hello, user world!", 0
