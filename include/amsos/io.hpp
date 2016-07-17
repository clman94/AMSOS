#ifndef AMSOS_IO_HPP
#define AMSOS_IO_HPP

#include <stdint.h>
namespace kern
{

class port
{
public:
	typedef uint16_t port_t;
	
	static inline uint8_t in_8(port_t p)
	{
		unsigned char rv;
		__asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (p));
		return rv;
	}
	
	static inline void out_8(port_t p, uint8_t d)
	{
		__asm__ __volatile__ ("outb %1, %0" : : "dN" (p), "a" (d));
	}
	
	static inline uint16_t in_16(port_t p)
	{
		unsigned short rv;
		__asm__ __volatile__ ("in %1, %0" : "=a" (rv) : "dN" (p));
		return rv;
	}
	
	static inline void out_16(port_t p, uint16_t d)
	{
		__asm__ __volatile__ ("out %1, %0" : : "dN" (p), "a" (d));
	}
	
	static inline void wait()
	{
		__asm__ __volatile__ ( "outb %%al, $0x80" : : "a"(0) );
	}
};

}


#endif