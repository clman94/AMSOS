extern error_handler

%assign i 0
%rep 31

	global int%[i]
	int%[i]:
		cli
		pusha
		push ds
		push es
		push fs
		push gs
		push i
		cld
		call error_handler
		pop eax
		pop gs
		pop fs
		pop es
		pop ds
		popa
		iret
		
	%assign i i+1
	%if i==15
		%assign i i+1
	%endif

%endrep

extern timer_handler
global int32
int32:
	pusha
	push ds
	push es
	push fs
	push gs
	mov eax, esp
	push eax
	cld
	call timer_handler
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	iret

extern keyboard_handler
global int33
int33:
	pusha
	push ds
	push es
	push fs
	push gs
	cld
	call keyboard_handler
	pop gs
	pop fs
	pop es
	pop ds
	popa
	iret