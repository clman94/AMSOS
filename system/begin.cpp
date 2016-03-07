
#include "memory/gdt.h"
#include "term.h"

extern "C"
void kern_begin()
{
	term_clear();
	
}
