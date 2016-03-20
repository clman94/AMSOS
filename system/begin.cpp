
#include "../display/terminal.h"

#include "../interrupts/interrupts.h"
#include "../interrupts/pic.h"
#include "../interrupts/idt.h"
#include "../interrupts/isr.h"
#include "../drivers/keyboard.h"

#include "ports.h"

void register_isr()
{
	ISR_register(1, (isr_t)irqh_keyboard_controller);
}

void register_exceptions()
{
	IDT_register(0,  &int0, 0);
	IDT_register(1,  &int1, 0);
	IDT_register(2,  &int2, 0);
	IDT_register(3,  &int3, 0);
	IDT_register(4,  &int4, 0);
	IDT_register(5,  &int5, 0);
	IDT_register(6,  &int6, 0);
	IDT_register(7,  &int7, 0);
	IDT_register(8,  &int8, 0);
	IDT_register(9,  &int9, 0);
	IDT_register(10, &int10, 0);
	IDT_register(11, &int11, 0);
	IDT_register(12, &int12, 0);
	IDT_register(13, &int13, 0);
	IDT_register(14, &int14, 0);
	IDT_register(16, &int16, 0);
	IDT_register(17, &int17, 0);
	IDT_register(18, &int18, 0);
	IDT_register(19, &int19, 0);
	IDT_register(20, &int20, 0);
	IDT_register(21, &int21, 0);
	IDT_register(22, &int22, 0);
	IDT_register(23, &int23, 0);
	IDT_register(24, &int24, 0);
	IDT_register(25, &int25, 0);
	IDT_register(26, &int26, 0);
	IDT_register(27, &int27, 0);
	IDT_register(28, &int28, 0);
	IDT_register(29, &int29, 0);
	IDT_register(30, &int30, 0);
	IDT_register(31, &int31, 0);
}

void register_irq()
{
	IDT_register(32, &int32, 0);
	IDT_register(33, &int33, 0);
	IDT_register(34, &int34, 0);
	IDT_register(35, &int35, 0);
	IDT_register(36, &int36, 0);
	IDT_register(37, &int37, 0);
	IDT_register(38, &int38, 0);
	IDT_register(39, &int39, 0);
	IDT_register(40, &int40, 0);
	IDT_register(41, &int41, 0);
	IDT_register(42, &int42, 0);
	IDT_register(43, &int43, 0);
	IDT_register(44, &int44, 0);
	IDT_register(45, &int45, 0);
	IDT_register(46, &int46, 0);
	IDT_register(47, &int47, 0);
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
	
	PIC_remap(PIC1_OFFSET, PIC2_OFFSET);
	
	term_prints("Complete\n");
	term_prints("Register Interrupts...");
	
	IRQ_disable_all();
	
	register_exceptions();
	register_irq();
	register_isr();
	
	IDTR_load();
	
	IRQ_enable(IRQ_TIMER);
	IRQ_enable(IRQ_CONTROLLER_KEYBOARD);

	timer_phase(200);
	
	term_prints("Complete\n");
}

extern "C"
void kern_main(){
	keyboard_enable_buffer();
	keyboard_enable_direct();
	
	while(true){
		uint8_t key = keyboard_get_ascii(true);
		if(key != 0){
			bool shift = keyboard_key_status(SCANCODE_LSHIFT);
			term_printc(shift ? (key - 'a' + 'A') : key);
		}
	}
}

