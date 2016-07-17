#include <amsos/terminal.hpp>
#include <amsos/isr.hpp>
#include <amsos/pic.hpp>

static kern::isr* _isr = nullptr;

void
kern::set_isr(kern::isr* isr)
{
	_isr = isr;
}

extern "C"
void error_handler(uint32_t irq)
{
	kern::terminal terminal;
	terminal.print("Kernel Panic");
}

extern "C"
void IRQ_handler(uint32_t irq)
{
	if (_isr)
		_isr->call(irq - 32);
	kern::pic::send_eoi(irq - 32);
}