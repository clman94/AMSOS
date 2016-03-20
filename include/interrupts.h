// This header is intended for use in begin.cpp only

extern "C" {
void int0();
void int1();
void int2();
void int3();
void int4();
void int5();
void int6();
void int7();
void int8();
void int9();
void int10();
void int11();
void int12();
void int13();
void int14();
void int16();
void int17();
void int18();
void int19();
void int20();
void int21();
void int22();
void int23();
void int24();
void int25();
void int26();
void int27();
void int28();
void int29();
void int30();
void int31();
void int32();
void int33();
void int34();
void int35();
void int36();
void int37();
void int38();
void int39();
void int40();
void int41();
void int42();
void int43();
void int44();
void int45();
void int46();
void int47();
}

inline
void interrupts_register_exceptions()
{
	IDT_register(0,  &int0, 0);
	IDT_register(1,  &int1, 0);
	IDT_register(2,  &int2, 0);
	IDT_register(3,  &int3, 0);
	IDT_register(4,  &int4, 0);
	IDT_register(5,  &int5, 0);
	IDT_register(6,  &int6, 0);
	IDT_register(7,  &int7, 0);
	IDT_register(8,  &int8, 0);
	IDT_register(9,  &int9, 0);
	IDT_register(10, &int10, 0);
	IDT_register(11, &int11, 0);
	IDT_register(12, &int12, 0);
	IDT_register(13, &int13, 0);
	IDT_register(14, &int14, 0);
	IDT_register(16, &int16, 0);
	IDT_register(17, &int17, 0);
	IDT_register(18, &int18, 0);
	IDT_register(19, &int19, 0);
	IDT_register(20, &int20, 0);
	IDT_register(21, &int21, 0);
	IDT_register(22, &int22, 0);
	IDT_register(23, &int23, 0);
	IDT_register(24, &int24, 0);
	IDT_register(25, &int25, 0);
	IDT_register(26, &int26, 0);
	IDT_register(27, &int27, 0);
	IDT_register(28, &int28, 0);
	IDT_register(29, &int29, 0);
	IDT_register(30, &int30, 0);
	IDT_register(31, &int31, 0);
}

inline
void interrupts_register_irq()
{
	IDT_register(32, &int32, 0);
	IDT_register(33, &int33, 0);
	IDT_register(34, &int34, 0);
	IDT_register(35, &int35, 0);
	IDT_register(36, &int36, 0);
	IDT_register(37, &int37, 0);
	IDT_register(38, &int38, 0);
	IDT_register(39, &int39, 0);
	IDT_register(40, &int40, 0);
	IDT_register(41, &int41, 0);
	IDT_register(42, &int42, 0);
	IDT_register(43, &int43, 0);
	IDT_register(44, &int44, 0);
	IDT_register(45, &int45, 0);
	IDT_register(46, &int46, 0);
	IDT_register(47, &int47, 0);
}