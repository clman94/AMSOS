#ifndef AMSOS_ISR_HPP
#define AMSOS_ISR_HPP
#include <stdint.h>

namespace kern
{

class isr
{
public:
	typedef void(*isr_t)();
	void set_isr(uint8_t irq, isr_t handler);
	void call(uint8_t irq);
private:
	isr_t routines[16] =
	{
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	};
};

// Src in interrupt_handler.cpp
void set_isr(isr* _isr);

}

#endif