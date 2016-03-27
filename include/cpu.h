#include <stdint.h>

static inline void cpuid(uint32_t code, uint32_t *a, uint32_t *d)
{
	asm volatile("cpuid":"=a"(*a),"=d"(*d):"a"(code):"ecx","ebx");
}

static inline uint32_t cpuid_string(uint32_t code, uint32_t str[4])
{
	asm volatile("cpuid":"=a"(*str),"=b"(*(str+1)),
	             "=c"(*(str+2)),"=d"(*(str+3)):"a"(code));
	return str[0];
}

void cpu_detect();
