[bits 32]
[org 0x0]

COLOR_STDINP equ 0x07
COLOR_STDOUT equ 0x08

_start:
	mov esi, msg
	mov al, COLOR_STDINP
	call puts
	.loop:
		mov byte [cur_token], 0
		call clrcmd
		mov edi, cwd
		call getcwd
		mov esi, cwd
		mov al, COLOR_STDOUT
		call puts
		mov esi, prompt
		call puts
		mov edi, cmd
		call gets
		call tokenize
		call execmd
		mov esi, newline
		call puts
		jmp .loop

store_token:
    push ebx
    xor ebx, ebx
    mov bl, [cur_token]
    imul ebx, 4
    mov ebx, [tokens+ebx]
    add ebx, ecx
    mov [ebx], al
    pop ebx
    ret

tokenize:
    pusha
    mov esi, cmd
    xor ecx, ecx ; token char index
    xor edx, edx ; command char index
    xor bl, bl ; quote string state
    .loop:
        mov al, [si]
        cmp al, 0
        jne .process
        popa
        ret
    .process:
        cmp al, '"'
        je .quote
        cmp al, ' '
        je .space
        call store_token
        inc esi
        inc ecx
        inc edx
        jmp .loop
    .space:
        cmp bl, 0
        jne .store
        inc edx
        inc ecx
        inc esi
        xor ecx, ecx
        inc byte [cur_token]
        jmp .loop
        .store:
            mov al, ' '
            call store_token
            inc edx
            inc ecx
            inc esi
            jmp .loop
    .quote:
        inc edx
        inc esi
        not bl
        jmp .loop

clrcmd:
	mov ecx, 0
	.loop:
		mov byte [cmd+ecx], 0
		mov byte [opcode+ecx], 0
		inc ecx
		cmp ecx, 256
		jl .loop
	ret
		

strncmp:
	.loop:
		dec ecx
		cmp ecx, 0
		je .done
		mov al, [esi+ecx]
		cmp al, [edi+ecx]
		jne .done
		jmp .loop
	.done:
		ret

clrbuf:
	pushad
	xor ebx, ebx
	.loop:
		mov byte [edi+ebx], 0
		inc ebx
		cmp ebx, ecx
		jl .loop
	popad
	ret

execmd:
	mov esi, opcode
	mov edi, chdir_tok
	mov ecx, 6
	call strncmp
	cmp ecx, 0
	je .chdir
	
	mov esi, opcode
	call exec

	ret

	.cls:
		call clrscr
		ret

	.help:
		mov esi, help_msg
		mov al, COLOR_STDINP
		call puts
		ret

	.chdir:
		mov esi, cmd+6
		call chdir
		ret

clrscr:	
	pushad
	mov eax, sys_clrscr
	int 0x80
	popad
	ret

getcwd:
	pushad
	mov [sys_getcwd_param], edi
	mov eax, sys_getcwd
	int 0x80
	popad
	ret

chdir:
	pushad
	mov [sys_chdir_param], esi
	mov eax, sys_chdir
	int 0x80
	popad
	ret

read:
	pushad
	mov [sys_read_param], esi
	mov [sys_read_param+4], edi
	mov [sys_read_param+8], ecx
	mov eax, sys_read
	int 0x80
	popad
	ret

puts:
	pushad
	mov [sys_puts_param], esi
	mov [sys_puts_param+4], al
	mov eax, sys_puts
	int 0x80
	popad
	ret

exec:
	pushad
	mov [sys_exec_param], esi
	mov dword [sys_exec_param+8], tokens
	xor eax, eax
	mov al, [cur_token]
	inc eax
	mov dword [sys_exec_param+4], eax
	mov eax, sys_exec
	int 0x80
	.loop:
		mov eax, [sys_exec_param+12]
		mov [sys_state_param], eax
		mov eax, sys_state
		int 0x80
		cmp dword [sys_state_param+4], 0
		jne .loop
		popad
		ret

gets:
	pushad
	mov ebx, edi
	.loop:
		call getc
		cmp al, 10
		je .done
		cmp al, 8
		je .bksp
		jmp .char
		;mov [edi], al
	.bksp:
		cmp ebx, edi
		jng .not_bksp
		mov esi, cur_key
		push eax
		mov al, COLOR_STDINP
		call puts
		pop eax
		dec ebx
		mov byte [ebx], ' '
		jmp .loop

	.not_bksp:
		jmp .loop

	.char:
		mov esi, cur_key
		push eax
		mov al, COLOR_STDINP
		call puts
		pop eax
		mov [ebx], al
		inc ebx
		jmp .loop
	.done:
		mov esi, cur_key
		push eax
		mov al, COLOR_STDINP
		call puts
		pop eax
		popad
		ret

getc:
	pushad
	.loopnz:
		mov eax, sys_getc
		int 0x80
		mov al, [sys_getc_param]
		cmp al, 0
		jne .loopnz
	.loopz:
		mov eax, sys_getc
		int 0x80
		mov al, [sys_getc_param]
		cmp al, 0
		je .loopz
	mov [cur_key], al
	popad
	mov al, [cur_key]
	ret

cur_key: db 0, 0

sys_clrscr:
	dd 6
	dd 0

sys_read:
	dd 2
	dd sys_read_param

sys_read_param:
	dd 0
	dd 0
	dd 0

sys_getcwd:
	dd 5
	dd sys_getcwd_param

sys_getcwd_param:
	dd 0

sys_chdir:
	dd 4
	dd sys_chdir_param

sys_chdir_param:
	dd 0

sys_exec:
	dd 7
	dd sys_exec_param

sys_exec_param:
	dd 0
	dd 0
	dd 0
	dd 0

sys_state:
	dd 8
	dd sys_state_param

sys_state_param:
	dd 0
	dd 0

sys_puts:
	dd 0
	dd sys_puts_param

sys_puts_param:
	dd cmd
	db 0x07

sys_getc:
	dd 1
	dd sys_getc_param

sys_getc_param:
	db 0

cmd: times 256 db 0
prompt: db "> ", 0
msg: db "Welcome to HengstOS. Type ", 0x22, "help", 0x22, " for help.", 10, 0
cwd: times 256 db 0
newline: db 10, 0
chdir_tok: db "chdir", 0
echo_tok: db "echo", 0
type_tok: db "type", 0
help_tok: db "help", 0
cls_tok: db "cls", 0
help_msg: db "Built-in commands:", 10
          db "  chdir   change directory", 10
	  db "  echo    display message", 10
	  db "  type    type file", 10
	  db "  cls     clear screen", 0
read_buffer: times 800 db 0
cur_token: db 0
tokens:
	dd opcode
	dd token1
	dd token2
	dd token3
opcode: times 64 db 0
token1: times 64 db 0
token2: times 64 db 0
token3: times 64 db 0
argv: dd tokens
















