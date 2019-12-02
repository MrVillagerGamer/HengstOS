global gfwNoise2d
global gfwNoise1d

gfwNoise2d:
    mov eax, [esp+8]
    mov [intX], eax
    mov eax, [esp+4]
    mov [intY], eax

    push ebx
    push edx
    mov eax,00BB38435h
    mul dword [intX]
    mov ebx,eax
    mov eax,03619636Bh
    mul dword [intY]
    add eax,ebx
    pop edx
    pop ebx

    push ecx
    mov CL,AL
    rol eax,CL
    mov CL,AL
    rol eax,CL
    mov CL,AL
    rol eax,CL
    pop ecx
    ret

gfwNoise1d:
    mov eax, [esp+4]
    push eax
    push dword 0
    call gfwNoise2d
    add esp, 8
    ret

intX: dd 0
intY: dd 0