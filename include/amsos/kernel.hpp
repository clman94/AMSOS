
#include <stdint.h>
#include <amsos/pointer.hpp>
#include <amsos/terminal.hpp>
#include <amsos/idt.hpp>
#include <amsos/pic.hpp>
#include <amsos/isr.hpp>
#include <amsos/io.hpp>

namespace kern
{


class kernel_core
{
	isr _isr;
	idt _idt;
	terminal _terminal;
public:

	isr& get_isr()
	{
		return _isr;
	}

	void timer_phase(int hz)
	{
		int divisor = 1193180 / hz;
		port::out_8(0x43, 0x36);
		port::out_8(0x40, divisor & 0xFF);
		port::out_8(0x40, divisor >> 8);
	}

	void initualize()
	{
		enable_interrupts(false);
		
		pic::remap(0x20, 0x28);
		
		pic::disable_all();
		
		_idt.register_all();
		_idt.load_IDTR();
		
		set_isr(&_isr);
		
		pic::enable(0);
		timer_phase(2);
		
		enable_interrupts(true);
	}
	
	terminal& get_terminal()
	{
		return _terminal;
	}
};

}

