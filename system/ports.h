#ifndef H_PORTS
#define H_PORTS

#include <stdint.h>

uint8_t port_in_b (uint16_t _port);
void port_out_b (uint16_t _port, uint8_t _data);

uint16_t port_in_w(uint16_t _port);
void port_out_w (uint16_t _port, uint16_t _data);

static inline void io_wait(void)
{
    __asm__ __volatile__ ( "outb %%al, $0x80" : : "a"(0) );
}

#endif