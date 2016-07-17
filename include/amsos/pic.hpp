#ifndef AMSOS_PIC_HPP
#define AMSOS_PIC_HPP

#include <stdint.h>
#include <amsos/io.hpp>

namespace kern
{

class pic
{
public:
	enum
	{
		pic_eoi       = 0x20,
		
		pic_1         = 0x20,
		pic_1_command = pic_1,
		pic_1_data    = pic_1 + 1,
		
		pic_2         = 0xA0,
		pic_2_command = pic_2,
		pic_2_data    = pic_2 + 1,
		
		icw1_icw4     = 0x01,
		icw1_init     = 0x10,
		
		icw4_8086     = 0x01,
	};
	
	static void disable_all()
	{
		port::out_8(pic_1_data, 0xFF);
		port::out_8(pic_2_data, 0xFF);
	}
	
	static void enable_all()
	{
		port::out_8(pic_1_data, 0x00);
		port::out_8(pic_2_data, 0x00);
	}
	
	static void enable(uint8_t irq)
	{
		uint16_t port;
		uint8_t value;
	 
		if(irq < 8)
		{
			port = pic_1_data;
		}
		else
		{
			port = pic_2_data;
			irq -= 8;
		}
		value = port::in_8(port) & ~(1 << irq);
		port::out_8(port, value);
	}
	
	static void send_eoi(uint8_t irq)
	{
		if (irq >= 8)
			port::out_8(pic_2_command, pic_eoi);
		port::out_8(pic_1_command, pic_eoi);
	}
	
	static void remap(int off1, int off2)
	{
		unsigned char a1, a2;
 
		a1 = port::in_8(pic_1_data);
		a2 = port::in_8(pic_2_data);
	 
		port::out_8(pic_1_command, icw1_init+icw1_icw4);
		port::wait();
		port::out_8(pic_2_command, icw1_init+icw1_icw4);
		port::wait();
		port::out_8(pic_1_data, off1);
		port::wait();
		port::out_8(pic_2_data, off2);
		port::wait();
		port::out_8(pic_1_data, 4);
		port::wait();
		port::out_8(pic_2_data, 2);
		port::wait();
	 
		port::out_8(pic_1_data, icw4_8086);
		port::wait();
		port::out_8(pic_2_data, icw4_8086);
		port::wait();
	 
		port::out_8(pic_1_data, a1);
		port::out_8(pic_2_data, a2);
	}
};



}

#endif