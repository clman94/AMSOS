#ifndef H_PIC
#define H_PIC

#include <stdint.h>

#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

void PIC_sendEOI(uint8_t irq);
void PIC_remap(int off1, int off2);

#define IRQ_TIMER               0x0
#define IRQ_CONTROLLER_KEYBOARD 0x1
#define IRQ_CASCADED_SIGNALS    0x2
#define IRQ_CONTROLLER_COM_2_4  0x3
#define IRQ_CONTROLLER_COM_1_3  0x4
#define IRQ_PARALLEL_2_3        0x5
#define IRQ_CONTROLLER_FLOPPY   0x6
#define IRQ_PARALLEL_1          0x7

#define IRQ_REALTIME_CLOCK      0x8
#define IRQ_ACPI                0x9
#define IRQ_OPEN_1              0xA
#define IRQ_OPEN_2              0xB
#define IRQ_CONTROLLER_MOUSE    0xC
#define IRQ_COPROCESSOR         0xD
#define IRQ_ATA_PRIMARY         0xE
#define IRQ_ATA_SECONDARY       0xF

void IRQ_disable_all();
void IRQ_enable_all();
void IRQ_enable(uint8_t irq);

#endif