extern error_handler

%assign i 0
%rep 32
	global __asm_interrupt_%[i]
	__asm_interrupt_%[i]:
		cli
		
		; dummy push
		%if i != 8  && \
		    i < 10  && \
			i > 14  && \
			i != 17 && \
			i != 30
		 push byte 0
		%endif
		
		pusha
		push ds
		push es
		push fs
		push gs
		
		mov eax, esp ; stack
		push eax
		
		mov eax, i
		push eax ; irq_no
		
		cld
		call error_handler
		
		pop eax ; pop the irq # and esp pointer
		pop eax
		
		pop gs
		pop fs
		pop es
		pop ds
		popa
		add esp, 4 ; pop errorcode
		iret
		
	%assign i i+1

%endrep

extern IRQ_handler

%assign i 32
%rep 16
	global __asm_interrupt_%[i]
	__asm_interrupt_%[i]:
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
		
		add esp, 8 ; pop the irq # and esp pointer
		
		pop gs
		pop fs
		pop es
		pop ds
		popa
		iret
	
	%assign i i+1
%endrep

;;; Had this idea to encapsulate the interrupt functions
;;; Didn't work so well...

;;; In idt.cpp   : extern "C" void __asm_register_interrupts(uint32_t func);
;;; In IDTR_load : "__asm_register_interrupts((uint32_t)&IDT_register);"

; global __asm_register_interrupts
; __asm_register_interrupts:
; %assign i 0
; %rep 48
	; mov eax, [esp+4]
	; push dword 0
	; push dword __asm_interrupt_%[i]
	; push dword i
	; call eax
	
	; %assign i i+1
; %endrep
	; ret