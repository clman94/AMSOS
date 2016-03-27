#include <include/pic.h>
#include <include/terminal.h>
#include <include/isr.h>

isr_t routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void ISR_register(uint8_t irq, isr_t routine)
{
	routines[irq] = routine;
}

// Called by interrupts.asm
extern "C"
void IRQ_handler(uint32_t irq, REG_x86_interrupt* regs)
{
	isr_t isr = routines[irq - 32];
	if (isr){
		//term_prints("IRQ");
		(*isr)(regs);
	}
	
	//term_prints("I");
	
	PIC_sendEOI(irq - 32);
}
