#ifndef H_IDT
#define H_IDT

#include <stdint.h>

void IDTR_load();
void IDT_register(uint32_t index, void (*handler)(), uint32_t dpl);
void enable_int(bool on);

#endif