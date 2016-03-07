#include "ports.h"

uint8_t port_in_b (uint16_t _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void port_out_b (uint16_t _port, uint8_t _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

uint16_t port_in_w(uint16_t _port)
{
    unsigned short rv;
    __asm__ __volatile__ ("in %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void port_out_w (uint16_t _port, uint16_t _data)
{
    __asm__ __volatile__ ("out %1, %0" : : "dN" (_port), "a" (_data));
}

