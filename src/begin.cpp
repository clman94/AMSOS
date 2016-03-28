
#include <include/terminal.h>
#include <include/pic.h>
#include <include/idt.h>
#include <include/isr.h>
#include <include/ports.h>
#include <include/ramfs.h>
#include <include/mm.h>
#include <include/cpu.h>

#include <include/drivers/keyboard.h>
#include <include/drivers/serial.h>

#include <include/interrupts.h>

#define DEBUG(A) term_prints("[KERNEL] "); term_prints(A);

void irq4()
{
	term_prints("irq4");
}

void ISR_register_drivers()
{
	ISR_register(IRQ_CONTROLLER_KEYBOARD, (isr_t)IRQH_keyboard_controller);
	ISR_register(IRQ_CONTROLLER_COM_1_3,  (isr_t)IRQH_serial_COM_1_3);
	ISR_register(IRQ_CONTROLLER_COM_2_4,  (isr_t)IRQH_serial_COM_2_4);
}

void timer_phase(int hz)
{
    int divisor = 1193180 / hz;
    port_out_b(0x43, 0x36);
    port_out_b(0x40, divisor & 0xFF);
    port_out_b(0x40, divisor >> 8);
}

extern "C"
void kern_setup()
{
	term_clear();
	
	DEBUG("Remap PIC...\n");
	
	PIC_remap(PIC1_OFFSET, PIC2_OFFSET);
	
	DEBUG("Register Interrupts...\n");
	
	IRQ_disable_all();
	
	IDT_register_exceptions();
	IDT_register_irq();
	ISR_register_drivers();
	
	IDTR_load();
	
	IRQ_enable(IRQ_TIMER);
	IRQ_enable(IRQ_CONTROLLER_KEYBOARD);
	IRQ_enable(IRQ_CONTROLLER_COM_1_3);
	IRQ_enable(IRQ_CONTROLLER_COM_2_4);
	
	DEBUG("Setting up timer...\n");
	
	timer_phase(500);
	
	DEBUG("Setting up memory...\n");
	init_memory_manager();
	
	DEBUG("Setting up ramfs...\n");
	
	ram_dir* rootdir = create_root_directory(MEMORY_BEGIN, MEMORY_BEGIN + sizeof(ram_dir), get_total_memory()); // temp
	
	cpu_detect();
}


