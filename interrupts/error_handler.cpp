#include <stdint.h>

extern "C"
void error_handler(uint32_t irq)
{
	*(char*)(0xb8001) = irq;
	
	for(;;){
		asm("hlt");
	}
}
