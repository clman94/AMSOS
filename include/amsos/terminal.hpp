#ifndef AMSOS_TERMINAL_HPP
#define AMSOS_TERMINAL_HPP

#include <stdint.h>
#include <amsos/pointer.hpp>

namespace kern
{

class terminal
{
	struct character{
		uint8_t ch, color;
	} _packed;
	
	pointer<character> buffer;
	uint32_t width, height;
	uint32_t x, y;
	uint8_t color;
public:
	static const ptr_t default_buffer = 0xb8000;
	
	terminal();
	void set_buffer(ptr_t p);
	void set_position(uint32_t nx, uint32_t ny);
	void set_color(uint8_t ncolor);
	void clear();
	void set(uint32_t x, uint32_t y, char c);
	int print(char c);
	int print(const char* str);
};

}

#endif