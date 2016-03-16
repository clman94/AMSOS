#include <stdint.h>

struct IDT_descriptor
{
	uint16_t low_offset;
	uint16_t selector;
	uint8_t  zero;
	uint8_t settings;
	uint16_t high_offset;
}  __attribute__ ((packed));

struct{
    unsigned short limit;
    IDT_descriptor* base;
} __attribute__((packed)) IDTR;

IDT_descriptor IDT[256];

// Loads IDT with lidt
void IDTR_load()
{
	IDTR.limit = 256*(sizeof(IDT_descriptor)-1);
	IDTR.base = IDT;

	// load IDTR
	asm volatile("lidt (%0)": :"m"(IDTR));
}


// Register function to interrupt
void IDT_register(int index, void (*handler)(), int dpl)
{
	unsigned short selector = 0x08;
	unsigned char settings = 0;
	unsigned int offset = (unsigned int)handler;
	
	asm volatile("movw %%cs,%0" :"=g"(selector));
	
	switch(dpl){
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

// Enable or disable interrupts
void enable_int(bool on)
{
	if(on)
	{
		asm volatile("sti");
	}
	else
	{
		asm volatile("cli");
	}
}