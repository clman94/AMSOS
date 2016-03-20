#include "pic.h"
#include "../system/ports.h"

#define PIC_EOI		  0x20

#define	PIC1          0x20
#define	PIC1_COMMAND  PIC1
#define	PIC1_DATA    (PIC1 + 1)

#define PIC2          0xA0
#define PIC2_COMMAND  PIC2
#define PIC2_DATA    (PIC2 + 1)

#define ICW1_ICW4     0x01
#define ICW1_INIT     0x10

#define ICW4_8086     0x01

void PIC_sendEOI(uint8_t irq)
{
	if (irq >= 8)
		port_out_b(PIC2_COMMAND, PIC_EOI);
	
	port_out_b(PIC1_COMMAND, PIC_EOI);
}

void PIC_remap(int off1, int off2)
{
	unsigned char a1, a2;
 
	a1 = port_in_b(PIC1_DATA);
	a2 = port_in_b(PIC2_DATA);
 
	port_out_b(PIC1_COMMAND, ICW1_INIT+ICW1_ICW4);
	io_wait();
	port_out_b(PIC2_COMMAND, ICW1_INIT+ICW1_ICW4);
	io_wait();
	port_out_b(PIC1_DATA, off1);
	io_wait();
	port_out_b(PIC2_DATA, off2);
	io_wait();
	port_out_b(PIC1_DATA, 4);
	io_wait();
	port_out_b(PIC2_DATA, 2);
	io_wait();
 
	port_out_b(PIC1_DATA, ICW4_8086);
	io_wait();
	port_out_b(PIC2_DATA, ICW4_8086);
	io_wait();
 
	port_out_b(PIC1_DATA, a1);
	port_out_b(PIC2_DATA, a2);
}

void IRQ_mask_all()
{
	port_out_b(PIC1_DATA, 0xFF);
	port_out_b(PIC2_DATA, 0xFF);
}

void IRQ_unmask_all()
{
	port_out_b(PIC1_DATA, 0x00);
	port_out_b(PIC2_DATA, 0x00);
}

void IRQ_unmask(uint8_t irq)
{
    uint16_t port;
    uint8_t value;
 
    if(irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = port_in_b(port) & ~(1 << irq);
    port_out_b(port, value);        
}