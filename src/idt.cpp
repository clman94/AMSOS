
#include <amsos/idt.h>

struct IDT_descriptor
{
	uint16_t low_offset;
	uint16_t selector;
	uint8_t  zero;
	uint8_t settings;
	uint16_t high_offset;
} __attribute__ ((packed));

struct
{
    uint16_t limit;
    IDT_descriptor* base;
} __attribute__((packed)) IDTR;

IDT_descriptor IDT[256];

// Register function to interrupt
void IDT_register(uint32_t index, void (*handler)(), uint32_t dpl)
{
	uint16_t selector = 0x08;
	uint8_t settings = 0;
	uint32_t offset = (uint32_t)handler;
	
	__asm__ __volatile__("movw %%cs,%0" :"=g"(selector));
	
	switch(dpl)
	{
		case 0: settings = 0x8E; break;
		case 1:
		case 2:
		case 3: settings = 0xEE; break;
	}
	
	IDT[index].low_offset  = (offset & 0xFFFF);
	IDT[index].selector    = selector;
	IDT[index].zero        = 0;
	IDT[index].settings    = settings;
	IDT[index].high_offset = (offset >> 16);
}

void IDTR_load()
{
	IDTR.limit = 256*(sizeof(IDT_descriptor)-1);
	IDTR.base = IDT;

	// load IDTR
	__asm__ __volatile__("lidt (%0)": :"m"(IDTR));
}

// Enable or disable interrupts
void enable_int(bool on)
{
	if(on)
	{
		__asm__ __volatile__("sti");
	}
	else
	{
		__asm__ __volatile__("cli");
	}
}