#include <amsos/kernel.hpp>
#include <amsos/stack.hpp>
#include <amsos/isr.hpp>

static kern::kernel_core* global_core;

void
kern::isr::set_isr(uint8_t irq, isr_t handler)
{
	routines[irq] = handler;
}

void
kern::isr::call(uint8_t irq)
{
	if (routines[irq])
		routines[irq]();
}

void isr_test()
{
	global_core->get_terminal().print("a");
}

extern "C"
void kern_main()
{
	kern::kernel_core core;
	global_core = &core;
	core.initualize();
	
	core.get_isr().set_isr(0, isr_test);
	
	kern::stack stack;
	stack.set_memory(0x01000000, 100);
	
	core.get_terminal().print("yay\n");
	core.get_terminal().print("This OS works\n");
	while(true){}
}
