#ifndef H_IRQ
#define H_IRQ

#include <stdint.h>
#include "registers.h"

typedef void(*isr_t)(REG_x86_interrupt*);

void ISR_register(uint8_t irq, isr_t routine);

#endif