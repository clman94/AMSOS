#include <stdint.h>

#include <amsos/registers.h>
#include <amsos/terminal.h>
#include <amsos/drivers/keyboard.h>

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
	
	term_prints("    !! PANIC !!    \n");
	term_prints("UNHANDLED EXCEPTION\n\n");
	
	term_prints("Error : ");
	if(irq > 14) term_prints("Unknown");
	else         term_prints(error_messages[irq]);
	
	term_prints("\nCode  : 0x");
	term_hex32(irq);
	
	term_prints("\nEAX:"); term_hex32(regs->general.eax);
	term_prints("  EBX:"); term_hex32(regs->general.ebx);
	term_prints("\nECX:"); term_hex32(regs->general.ecx);
	term_prints("  EDX:"); term_hex32(regs->general.edx);
	while(true){
		asm("hlt");
	}
}

extern "C"
void error_handler(uint32_t irq, REG_x86_error* regs)
{
	display_unhandled_exception(irq, regs);
}
