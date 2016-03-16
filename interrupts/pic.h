#ifndef H_PIC
#define H_PIC

#include <stdint.h>

void PIC_sendEOI(uint8_t irq);
void PIC_remap(int off1, int off2);

void IRQ_mask_all();
void IRQ_unmask_all();
void IRQ_unmask(uint8_t irq);

#endif