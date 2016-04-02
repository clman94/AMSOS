
#include <amsos/mm.h>
#include <amsos/ports.h>
#include <amsos/terminal.h>

#define DEBUG(A) term_prints("[MM] "); term_prints(A);

static uint32_t avaliable_memory = 0;

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
	DEBUG("Detecting Memory...\n");
	chk_memory_CMOS();
	
	DEBUG("Total Memory : 0x"); term_hex32(avaliable_memory); term_printc('\n');
}
