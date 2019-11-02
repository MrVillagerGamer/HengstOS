global ints_wrapper_32
extern switch_task

ints_wrapper_32:
	push ebp
	push edi
	push esi
	push edx
	push ecx
	push ebx
	push eax
	xor eax, eax
	mov ax, gs
	push eax
	mov ax, fs
	push eax
	mov ax, es
	push eax
	mov ax, ds
	push eax
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax	
	mov eax, esp
	push eax
	call switch_task
	pop eax
	pop eax
	mov ds, ax
	pop eax
	mov es, ax
	pop eax
	mov fs, ax
	pop eax
	mov gs, ax
	pop eax
	pop ebx
	pop ecx
	pop edx
	pop esi
	pop edi
	pop ebp
	iretd	

switch_task_old:
	pushad
	pushfd
	mov eax, cr3
	push eax
	mov eax, [esp+44]
	mov [eax+4], ebx
	mov [eax+8], ecx
	mov [eax+12], edx
	mov [eax+16], esi
	mov [eax+20], edi
	mov ebx, [esp+36]
	mov ecx, [esp+40]
	mov edx, [esp+20]
	add edx, 4
	mov esi, [esp+16]
	mov edi, [esp+4]
	mov [eax], ebx
	mov [eax+24], edx
	mov [eax+28], esi
	mov [eax+32], ecx
	mov [eax+36], edi
	pop ebx
	mov [eax+40], ebx
	push ebx
	mov eax, [esp+48]
	mov ebx, [eax+4]
	mov ecx, [eax+8]
	mov edx, [eax+12]
	mov esi, [eax+16]
	mov edi, [eax+20]
	mov ebp, [eax+28]
	push eax
	mov eax, [eax+36]
	push eax
	popf
	pop eax
	mov esp, [eax+24]
	push eax
	mov eax, [eax+40]
	mov cr3, eax
	pop eax
	push eax
	mov eax, [eax+32]
	xchg eax, [esp]
	mov eax, [eax]
	ret





