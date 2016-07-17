#include <amsos/idt.hpp>
#include <amsos/interrupts.h>

using namespace kern;

void
idt::register_all()
{
	// Exceptions
	register_int(0,  &__asm_interrupt_0,  0);
	register_int(1,  &__asm_interrupt_1,  0);
	register_int(2,  &__asm_interrupt_2,  0);
	register_int(3,  &__asm_interrupt_3,  0);
	register_int(4,  &__asm_interrupt_4,  0);
	register_int(5,  &__asm_interrupt_5,  0);
	register_int(6,  &__asm_interrupt_6,  0);
	register_int(7,  &__asm_interrupt_7,  0);
	register_int(8,  &__asm_interrupt_8,  0);
	register_int(9,  &__asm_interrupt_9,  0);
	register_int(10, &__asm_interrupt_10, 0);
	register_int(11, &__asm_interrupt_11, 0);
	register_int(12, &__asm_interrupt_12, 0);
	register_int(13, &__asm_interrupt_13, 0);
	register_int(14, &__asm_interrupt_14, 0);
	register_int(16, &__asm_interrupt_16, 0);
	register_int(17, &__asm_interrupt_17, 0);
	register_int(18, &__asm_interrupt_18, 0);
	register_int(19, &__asm_interrupt_19, 0);
	register_int(20, &__asm_interrupt_20, 0);
	register_int(21, &__asm_interrupt_21, 0);
	register_int(22, &__asm_interrupt_22, 0);
	register_int(23, &__asm_interrupt_23, 0);
	register_int(24, &__asm_interrupt_24, 0);
	register_int(25, &__asm_interrupt_25, 0);
	register_int(26, &__asm_interrupt_26, 0);
	register_int(27, &__asm_interrupt_27, 0);
	register_int(28, &__asm_interrupt_28, 0);
	register_int(29, &__asm_interrupt_29, 0);
	register_int(30, &__asm_interrupt_30, 0);
	register_int(31, &__asm_interrupt_31, 0);
	
	// IRQs
	register_int(32, &__asm_interrupt_32, 0);
	register_int(33, &__asm_interrupt_33, 0);
	register_int(34, &__asm_interrupt_34, 0);
	register_int(35, &__asm_interrupt_35, 0);
	register_int(36, &__asm_interrupt_36, 0);
	register_int(37, &__asm_interrupt_37, 0);
	register_int(38, &__asm_interrupt_38, 0);
	register_int(39, &__asm_interrupt_39, 0);
	register_int(40, &__asm_interrupt_40, 0);
	register_int(41, &__asm_interrupt_41, 0);
	register_int(42, &__asm_interrupt_42, 0);
	register_int(43, &__asm_interrupt_43, 0);
	register_int(44, &__asm_interrupt_44, 0);
	register_int(45, &__asm_interrupt_45, 0);
	register_int(46, &__asm_interrupt_46, 0);
	register_int(47, &__asm_interrupt_47, 0);
}

void
idt::register_int(uint32_t index, handler h, uint32_t dpl)
{
	uint16_t selector = 0x08;
	uint8_t settings = 0;
	uint32_t offset = reinterpret_cast<uint32_t>(h);
	
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

void
idt::load_IDTR()
{
	IDTR.limit = 256*(sizeof(descriptor)-1);
	IDTR.base = IDT;
	__asm__ __volatile__("lidt (%0)": :"r"(&IDTR));
}

