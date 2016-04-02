#include <stdint.h>

#include <amsos/registers.h>
#include <amsos/terminal.h>
#include <amsos/drivers/keyboard.h>
#include <amsos/library/io.h>

const char* error_messages[] = 
{
	"Divide-by-zero",
	"Debug",
	"Non-maskable Interrupt",
	"Breakpoint",
	"Overflow",
	"Bound Range Exceeded",
	"Invalid Opcode",
	"Device Not Available",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Invalid TSS",
	"Segment Not Present",
	"Stack-Segment Fault",
	"General Protection Fault",
	"Page Fault"
};

void display_unhandled_exception(uint32_t irq, REG_x86_error* regs)
{
	//term_clear();
	
	printf("    !! PANIC !!    \n");
	printf("UNHANDLED EXCEPTION\n\n");
	
	printf("Error : ");
	if(irq > 14) printf("Unknown");
	else         printf(error_messages[irq]);
	
	printf("\nCode  : 0x");
	term_hex32(irq);
	
	printf("\nEAX:"); term_hex32(regs->general.eax);
	printf("  EBX:"); term_hex32(regs->general.ebx);
	printf("\nECX:"); term_hex32(regs->general.ecx);
	printf("  EDX:"); term_hex32(regs->general.edx);
	while(true){
		asm("hlt");
	}
}

extern "C"
void error_handler(uint32_t irq, REG_x86_error* regs)
{
	display_unhandled_exception(irq, regs);
}
