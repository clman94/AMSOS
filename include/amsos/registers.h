#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

struct REG_x86_segment{
	uint32_t gs, fs, es, ds;
};

struct REG_x86_general{
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
};

struct REG_x86_interrupt{
	REG_x86_segment segment;
	REG_x86_general general;
	uint32_t eip, cs, eflags, useresp, ss;
};

struct REG_x86_error{
	REG_x86_segment segment;
	REG_x86_general general;
	uint32_t err_code;
	uint32_t eip, cs, eflags, useresp, ss;
};

#endif