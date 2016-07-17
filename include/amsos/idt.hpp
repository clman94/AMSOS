#ifndef AMSOS_IDT_HPP
#define AMSOS_IDT_HPP

#include <stdint.h>

#include <amsos/pointer.hpp>

namespace kern{
	
class idt
{
	struct descriptor
	{
		uint16_t low_offset;
		uint16_t selector;
		uint8_t  zero;
		uint8_t settings;
		uint16_t high_offset;
	} _packed;
	
	struct idtr
	{
		uint16_t limit;
		descriptor* base;
	} _packed;
	
	idtr IDTR;
	descriptor IDT[256];
	
public:

	typedef void (*handler)();
	void load_IDTR();
	void register_all();
	void register_int(uint32_t index, handler h, uint32_t dpl);
};

static inline void enable_interrupts(bool on)
{
	if(on)
		__asm__ __volatile__("sti");
	else
		__asm__ __volatile__("cli");
}

}



#endif