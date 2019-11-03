section .init
    global _start
    extern main
_start:
    push ebx
    push eax
    call main
    mov eax, sigexit
    int 0x80
    jmp $

sigexit:
    dd 9
    dd 0