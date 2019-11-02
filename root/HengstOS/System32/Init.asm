[bits 32]
[org 0x0]

_start:
	mov esi, msg
	call puts
	.loop:
		call clrcmd
		mov edi, cwd
		call getcwd
		mov esi, cwd
		call puts
		mov esi, prompt
		call puts
		mov edi, cmd
		call gets
		call execmd
		mov esi, newline
		call puts
		jmp .loop

clrcmd:
	mov ecx, 0
	.loop:
		mov byte [cmd+ecx], 0
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
	.loop:
		mov byte [edi+ecx], 0
		dec ecx
		cmp ecx, 0
		jg .loop
	popad
	ret

execmd:
	mov esi, cmd
	mov edi, chdir_tok
	mov ecx, 6
	call strncmp
	cmp ecx, 0
	je .chdir

	mov edi, echo_tok
	mov ecx, 5
	call strncmp
	cmp ecx, 0
	je .echo
	
	mov edi, type_tok
	mov ecx, 5
	call strncmp
	cmp ecx, 0
	je .type

	mov edi, help_tok
	mov ecx, 5
	call strncmp
	cmp ecx, 0
	je .help

	mov edi, cls_tok
	mov ecx, 4
	call strncmp
	cmp ecx, 0
	je .cls

	ret

	.cls:
		call clrscr
		ret

	.help:
		mov esi, help_msg
		call puts
		ret

	.type:
		mov esi, cmd+5
		mov edi, read_buffer
		mov ecx, 800
		call read
		mov esi, read_buffer
		call puts
		mov ecx, 800
		mov edi, read_buffer
		call clrbuf
		ret

	.echo:
		mov esi, cmd+5
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
	mov eax, sys_puts
	int 0x80
	popad
	ret

gets:
	pushad
	.loop:
		call getc
		cmp al, 10
		jne .char
		;mov [edi], al
		popa
		ret
	.char:
		mov [edi], al
		inc edi
		jmp .loop

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
	mov esi, cur_key
	call puts
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
chdir_tok: db "chdir ", 0
echo_tok: db "echo ", 0
type_tok: db "type ", 0
help_tok: db "help", 0
cls_tok: db "cls", 0
help_msg: db "Built-in ommands:", 10
          db "  chdir   change directory", 10
	  db "  echo    display message", 10
	  db "  type    type file", 10
	  db "  cls     clear screen", 0
read_buffer: times 800 db 0


















