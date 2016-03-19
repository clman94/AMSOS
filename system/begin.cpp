
#include "../display/terminal.h"

#include "../interrupts/interrupts.h"
#include "../interrupts/pic.h"
#include "../interrupts/idt.h"

#include "ports.h"

void register_exceptions()
{
	IDT_register(0, &int0, 0);
	IDT_register(1, &int1, 0);
	IDT_register(2, &int2, 0);
	IDT_register(3, &int3, 0);
	IDT_register(4, &int4, 0);
	IDT_register(5, &int5, 0);
	IDT_register(6, &int6, 0);
	IDT_register(7, &int7, 0);
	IDT_register(8, &int8, 0);
	IDT_register(9, &int9, 0);
	IDT_register(10, &int10, 0);
	IDT_register(11, &int11, 0);
	IDT_register(12, &int12, 0);
	IDT_register(13, &int13, 0);
	IDT_register(14, &int14, 0);
	IDT_register(16, &int16, 0);
	IDT_register(17, &int17, 0);
	IDT_register(18, &int18, 0);
}

void register_irq()
{
	IDT_register(32, &int32, 0);
	IDT_register(33, &int33, 0);
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
	register_irq();
	
	IDTR_load();
	
	IRQ_unmask(0);
	//IRQ_unmask(1);
	
	timer_phase(100);
	
	term_prints("Complete\n");
}

extern "C"
void kern_main(){
	
	while(true){}
}

