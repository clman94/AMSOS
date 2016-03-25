
#include <include/mm.h>
#include <include/ports.h>

uint32_t avaliable_memory = 0;

uint32_t get_total_memory()
{
	return avaliable_memory;
}

void chk_memory_CMOS()
{
    uint32_t l, h;
 
    port_out_b(0x70, 0x30);
    l = port_in_b(0x71);
	
    port_out_b(0x70, 0x31);
    h = port_in_b(0x71);
 
    avaliable_memory = l | h << 8;
}

void init_memory_manager()
{
	chk_memory_CMOS();
}
