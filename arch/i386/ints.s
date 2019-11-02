extern ints_handler
global dummy_handler

%macro ints_wrapper 1
global ints_wrapper_%1
ints_wrapper_%1:
	pushad
	pushfd
	push word %1
	call ints_handler
	pop ax
	popfd
	popad
	iretd
%endmacro


dummy_handler:
	iretd
