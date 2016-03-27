#include <include/cpu.h>
#include <include/library/string.h>
#include <include/terminal.h>

struct cpu_info
{
	uint32_t std_max;
	char     id[13];
	
	uint32_t freq;
	uint32_t freq_max;
	uint32_t bus_freq;
};

void cpu_detect()
{
	cpu_info info;
	uint32_t gregs[4] = { 0, };
	
	// Get Vendor and supported standard
	cpuid_string(0x00, gregs);
	
	info.std_max = gregs[0];
	memcpy(info.id, &gregs[1], 4);
	memcpy(&info.id[4], &gregs[3], 4);
	memcpy(&info.id[8], &gregs[2], 4);
	info.id[12] = '\0';
	
	// Get Processer Frequency
	cpuid_string(0x80860007, gregs);
	
	info.freq     = gregs[0];
	info.freq_max = gregs[1];
	info.bus_freq = gregs[2];
	
	term_prints("[CPU] #CPU INFO#");
	term_prints("\n[CPU] STD Level : 0x"); term_hex32(info.std_max);
	term_prints("\n[CPU] Vender    : "); term_prints(info.id);
	term_prints("\n[CPU] Frequency : 0x"); term_hex32(info.freq);
}