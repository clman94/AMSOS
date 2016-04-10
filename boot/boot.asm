
;####################
; GRUB
;####################

section .text

MULTIBOOT_PAGE_ALIGN	equ 1<<0
MULTIBOOT_MEMORY_INFO	equ 1<<1
MULTIBOOT_AOUT_KLUDGE	equ 1<<16
MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
MULTIBOOT_CHECKSUM	    equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

section .__mbHeader
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

;####################
; START
;####################

section .text

global _start
extern kern_setup
extern kern_main

align 4
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
	call kern_main
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

