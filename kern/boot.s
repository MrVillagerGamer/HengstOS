extern main
global _start
global INTERRUPT_STACK

MAGIC equ 0x1BADB002
FLAGS equ (1<<0)|(1<<1)
CHECKSUM equ -(MAGIC+FLAGS)

section .mboot
	dd MAGIC
	dd FLAGS
	dd CHECKSUM

section .text
_start:
	mov esp, KERNEL_STACK
	push ebx
	call main
	jmp $

section .bss
	resb 64*1024
KERNEL_STACK:
	resb 64*1024
INTERRUPT_STACK:
