
struct REG_x86_segment{
	unsigned int gs, fs, es, ds;
};

struct REG_x86_general{
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
};

struct REG_x86_interrupt{
	REG_x86_segment segment;
	REG_x86_gen general;
	unsigned int eip, cs, eflags, useresp, ss;
};

struct REG_x86_error{
	unsigned int code;
	REG_x86_segment segment;
	REG_x86_gen general;
	unsigned int eip, cs, eflags, useresp, ss;
};