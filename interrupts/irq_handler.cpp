#include "pic.h"
#include "registers.h"

#include "../display/terminal.h"

#include <stdint.h>

void* irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

typedef void(*routine_t)(REG_x86_interrupt);

extern "C"
void IRQ_handler(uint32_t irq, REG_x86_interrupt regs)
{
	routine_t routine = (routine_t)irq_routines[irq - 32];
	if (routine){
		term_prints("IRQ");
		for(;;){}
		routine(regs);
	}
	
	//term_prints("I");
	
	PIC_sendEOI(irq - 32);
}
