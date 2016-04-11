#include <amsos/registers.h>
#include <amsos/ramfs.h>


static ram_dir* rootdir = nullptr;
static ram_dir* d_proc = nullptr;  // dir of processes
static ram_entry* c_proc = nullptr; // current process entry in dir

int sched_create_k_process(const char* name)
{
	ram_dir* d = create_dir(d_proc, name);
	create_file(d, "reg", sizeof(REG_x86_interrupt));
	create_file(d, "priority", 0);
	return 0;
}

void sched_init(ram_dir* rd)
{
	rootdir = rd;
	d_proc = get_dir_path(rootdir, "system/proc");
	sched_create_k_process("kernel");
	c_proc = find_file(d_proc, "kernel");
}

void IRQH_scheduler(REG_x86_interrupt* regs)
{
	//find_file(()c_proc->range[0])
	c_proc = get_next_file(d_proc, c_proc);
	if(c_proc == nullptr)
		c_proc = get_first_file(d_proc);
	
}