// This header is intended for use in begin.cpp only

#include <amsos/idt.h>

extern "C" {
void __asm_interrupt_0();
void __asm_interrupt_1();
void __asm_interrupt_2();
void __asm_interrupt_3();
void __asm_interrupt_4();
void __asm_interrupt_5();
void __asm_interrupt_6();
void __asm_interrupt_7();
void __asm_interrupt_8();
void __asm_interrupt_9();
void __asm_interrupt_10();
void __asm_interrupt_11();
void __asm_interrupt_12();
void __asm_interrupt_13();
void __asm_interrupt_14();
void __asm_interrupt_16();
void __asm_interrupt_17();
void __asm_interrupt_18();
void __asm_interrupt_19();
void __asm_interrupt_20();
void __asm_interrupt_21();
void __asm_interrupt_22();
void __asm_interrupt_23();
void __asm_interrupt_24();
void __asm_interrupt_25();
void __asm_interrupt_26();
void __asm_interrupt_27();
void __asm_interrupt_28();
void __asm_interrupt_29();
void __asm_interrupt_30();
void __asm_interrupt_31();
void __asm_interrupt_32();
void __asm_interrupt_33();
void __asm_interrupt_34();
void __asm_interrupt_35();
void __asm_interrupt_36();
void __asm_interrupt_37();
void __asm_interrupt_38();
void __asm_interrupt_39();
void __asm_interrupt_40();
void __asm_interrupt_41();
void __asm_interrupt_42();
void __asm_interrupt_43();
void __asm_interrupt_44();
void __asm_interrupt_45();
void __asm_interrupt_46();
void __asm_interrupt_47();
}

inline
void IDT_register_exceptions()
{
	IDT_register(0,  &__asm_interrupt_0,  0);
	IDT_register(1,  &__asm_interrupt_1,  0);
	IDT_register(2,  &__asm_interrupt_2,  0);
	IDT_register(3,  &__asm_interrupt_3,  0);
	IDT_register(4,  &__asm_interrupt_4,  0);
	IDT_register(5,  &__asm_interrupt_5,  0);
	IDT_register(6,  &__asm_interrupt_6,  0);
	IDT_register(7,  &__asm_interrupt_7,  0);
	IDT_register(8,  &__asm_interrupt_8,  0);
	IDT_register(9,  &__asm_interrupt_9,  0);
	IDT_register(10, &__asm_interrupt_10, 0);
	IDT_register(11, &__asm_interrupt_11, 0);
	IDT_register(12, &__asm_interrupt_12, 0);
	IDT_register(13, &__asm_interrupt_13, 0);
	IDT_register(14, &__asm_interrupt_14, 0);
	IDT_register(16, &__asm_interrupt_16, 0);
	IDT_register(17, &__asm_interrupt_17, 0);
	IDT_register(18, &__asm_interrupt_18, 0);
	IDT_register(19, &__asm_interrupt_19, 0);
	IDT_register(20, &__asm_interrupt_20, 0);
	IDT_register(21, &__asm_interrupt_21, 0);
	IDT_register(22, &__asm_interrupt_22, 0);
	IDT_register(23, &__asm_interrupt_23, 0);
	IDT_register(24, &__asm_interrupt_24, 0);
	IDT_register(25, &__asm_interrupt_25, 0);
	IDT_register(26, &__asm_interrupt_26, 0);
	IDT_register(27, &__asm_interrupt_27, 0);
	IDT_register(28, &__asm_interrupt_28, 0);
	IDT_register(29, &__asm_interrupt_29, 0);
	IDT_register(30, &__asm_interrupt_30, 0);
	IDT_register(31, &__asm_interrupt_31, 0);
}

inline
void IDT_register_irq()
{
	IDT_register(32, &__asm_interrupt_32, 0);
	IDT_register(33, &__asm_interrupt_33, 0);
	IDT_register(34, &__asm_interrupt_34, 0);
	IDT_register(35, &__asm_interrupt_35, 0);
	IDT_register(36, &__asm_interrupt_36, 0);
	IDT_register(37, &__asm_interrupt_37, 0);
	IDT_register(38, &__asm_interrupt_38, 0);
	IDT_register(39, &__asm_interrupt_39, 0);
	IDT_register(40, &__asm_interrupt_40, 0);
	IDT_register(41, &__asm_interrupt_41, 0);
	IDT_register(42, &__asm_interrupt_42, 0);
	IDT_register(43, &__asm_interrupt_43, 0);
	IDT_register(44, &__asm_interrupt_44, 0);
	IDT_register(45, &__asm_interrupt_45, 0);
	IDT_register(46, &__asm_interrupt_46, 0);
	IDT_register(47, &__asm_interrupt_47, 0);
}