
#include <include/terminal.h>
#include <include/pic.h>
#include <include/idt.h>
#include <include/isr.h>
#include <include/drivers/keyboard.h>
#include <include/ports.h>

#include <include/interrupts.h>

void register_isr()
{
	ISR_register(1, (isr_t)irqh_keyboard_controller);
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
	
	interrupts_register_exceptions();
	interrupts_register_irq();
	
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

