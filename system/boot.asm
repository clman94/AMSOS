
%include "grub.inc"

;####################
; START
;####################

BITS 32
global _start
extern kern_setup
;extern kern_main

_start:
	
	; Set stack
	mov esp, stack_top
	
	; These 2 calls are ment to organize code, 
	;   so things don't get messy when trying to 
	;   seperate hardware handling from software handling.

	; This function sets up interrupts, hardware, ect. 
	; (Interrupts are disabled beforehand and 
	;   re-enabled afterwords)
	cli
	call kern_setup
	sti
	
	; This begins the main kernel (software stuff)
	;call kern_main
	
	cli
hang:
	hlt
	jmp hang

; Create a new stack
section .bss
global stack_top
global stack_bottom

align 4
stack_bottom:
resb 16384
stack_top:

;####################
; EXTRA FUNCTIONS
;####################

	; Stuff here

;####################
; GRUB
;####################

section .text

extern code, bss, end

align 4
mboot:
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM
	dd mboot
	dd code
	dd bss
	dd end
	dd _start