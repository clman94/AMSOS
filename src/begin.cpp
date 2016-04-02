
#include <amsos/terminal.h>
#include <amsos/pic.h>
#include <amsos/idt.h>
#include <amsos/isr.h>
#include <amsos/ports.h>
#include <amsos/ramfs.h>
#include <amsos/mm.h>
#include <amsos/cpu.h>

#include <amsos/drivers/keyboard.h>
#include <amsos/drivers/serial.h>

#include <amsos/interrupts.h>

#include <amsos/library/io.h>

#define DEBUG(A) printf("[KERNEL] %s", A);

static ram_dir* rootdir = nullptr;

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

void debug_file_tree(ram_dir* dir)
{
	ram_entry* e = get_first_file(dir);
	while (e)
	{
		if(e->type == RAM_FILE_TYPE_DIRECTORY)
		{
			printf("----");
			printf("%s", e->name);
			printf("----\n");
			debug_file_tree((ram_dir*)e->range[0]);
		}
		else
		{
			printf("%s", e->name);
			printf("\n");
		}
		e = get_next_file(dir, e);
	}
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
	
	rootdir = create_root_dir(MEMORY_BEGIN, MEMORY_BEGIN + sizeof(ram_dir), get_total_memory()); // temp
	
	ram_dir* dir_system  = create_dir(rootdir,    "system");
	ram_dir* dir_video   = create_dir(dir_system, "video");
	make_file(dir_video, "80X25", 0xB8000, 0xB8FA0, RAM_FILE_TYPE_FILE, true);
	
	debug_file_tree(rootdir);
	
	cpu_detect();
	
	term_init(rootdir);
}


