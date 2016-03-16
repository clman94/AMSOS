
#include "../display/terminal.h"

#include "../interrupts/interrupts.h"
#include "../interrupts/pic.h"
#include "../interrupts/idt.h"

#include "ports.h"

void register_exceptions()
{
	IDT_register(0, (void(*)())int1, 0);
	IDT_register(0, (void(*)())int2, 0);
	IDT_register(0, (void(*)())int3, 0);
	IDT_register(0, (void(*)())int4, 0);
	IDT_register(0, (void(*)())int5, 0);
	IDT_register(0, (void(*)())int6, 0);
	IDT_register(0, (void(*)())int7, 0);
	IDT_register(0, (void(*)())int8, 0);
	IDT_register(0, (void(*)())int9, 0);
	IDT_register(0, (void(*)())int10, 0);
	IDT_register(0, (void(*)())int11, 0);
	IDT_register(0, (void(*)())int12, 0);
	IDT_register(0, (void(*)())int13, 0);
	IDT_register(0, (void(*)())int14, 0);
	IDT_register(0, (void(*)())int16, 0);
	IDT_register(0, (void(*)())int17, 0);
	IDT_register(0, (void(*)())int18, 0);
}

void register_irq()
{
	IDT_register(32, (void(*)())int32, 0);
	IDT_register(33, (void(*)())int33, 0);
}

void timer_phase(int hz){
    int divisor = 1193180 / hz;
    port_out_b(0x43, 0x36);
    port_out_b(0x40, divisor & 0xFF);
    port_out_b(0x40, divisor >> 8);
}

extern "C"
void kern_setup()
{
	term_clear();
	
	term_prints("Remap PIC...");
	
	PIC_remap(0x20, 0x28);
	
	term_prints("Complete\n");
	term_prints("Register Interrupts...");
	
	IRQ_mask_all();
	
	register_exceptions();
	
	timer_phase(100);
	
	IDTR_load();
	
	IRQ_unmask(0);
	//IRQ_unmask(1);
	
	term_prints("Complete\n");
}
