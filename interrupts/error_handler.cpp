#include <stdint.h>
#include "../display/terminal.h"

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

extern "C"
void error_handler(uint32_t irq)
{
	term_clear();
	
	term_prints("    !! PANIC !!    \n");
	term_prints("UNHANDLED EXCEPTION\n\n");
	
	term_prints("Error : ");
	if(irq > 14) term_prints("Unknown");
	else         term_prints(error_messages[irq]);
	
	term_prints("\nCode  : ");
	term_printc(((irq/100)%10) + '0');
	term_printc(((irq/10)%10) + '0');
	term_printc((irq%10) + '0');
	
	for(;;){
		asm("hlt");
	}
}
