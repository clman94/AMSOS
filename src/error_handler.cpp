#include <stdint.h>

#include <include/registers.h>
#include <include/terminal.h>

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

void display_number(unsigned int num)
{
	char hex[] = "0123456789ABCDEF";
	char conv[9] = { '0', };
	for(int i = 0; i < 9; i++){
		conv[i] = hex[(num%16)];
		num /= 16;
	}
	
	char out[11] = {'0', 'x', '0',};
	for(int i = 0; i < 9; i++){
		out[10 - i] = conv[i];
	}
	
	term_prints(out);
}

extern "C"
void error_handler(uint32_t irq, uint32_t no, REG_x86_interrupt* regs)
{
	term_clear();
	
	term_prints("    !! PANIC !!    \n");
	term_prints("UNHANDLED EXCEPTION\n\n");
	
	term_prints("Error : ");
	if(irq > 14) term_prints("Unknown");
	else         term_prints(error_messages[irq]);
	
	term_prints("\nCode  : ");
	display_number(irq - 32);
	
	for(;;){
		asm("hlt");
	}
}
