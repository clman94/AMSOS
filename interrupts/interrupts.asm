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
		
		%if i == 8
		 push byte 0
		%elif i >= 10 && i <=14
		 push byte 0
		%endif
		
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

extern IRQ_handler

%assign i 32
%rep 14

	global int%[i]
	int%[i]:
		pusha
		push ds
		push es
		push fs
		push gs
		mov eax, esp
		push eax
		push i
		cld
		call IRQ_handler
		pop eax
		pop eax
		pop gs
		pop fs
		pop es
		pop ds
		popa
		iret
	
	%assign i i+1
%endrep