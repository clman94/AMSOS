#ifndef H_IDT
#define H_IDT

void IDTR_load();
void IDT_register(int index, void (*handler)(), int dpl);
void enable_int(bool on);

#endif