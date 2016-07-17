#include <amsos/terminal.hpp>

using namespace kern;

terminal::terminal()
{
	width = 80;
	height = 80;
	x = 0;
	y = 0;
	color = 0x07;
	buffer = default_buffer;
}

void
terminal::set_buffer(ptr_t p)
{
	buffer = p;
}
void
terminal::set_position(uint32_t nx, uint32_t ny)
{
	x = nx;
	y = ny;
}
void
terminal::set_color(uint8_t ncolor)
{
	color = ncolor;
}

void
terminal::clear()
{
	for (ptr_t i = 0; i < width*height; i++)
	{
		character* c = buffer.get(i);
		c->ch = ' ';
		c->color = color;
	}
	x = 0;
	y = 0;
}

void
terminal::set(uint32_t x, uint32_t y, char ch)
{
	auto c = buffer.get(x + (y*width));
	c->ch = ch;
	c->color = color;
}

int
terminal::print(char c)
{
	if(c == '\b'){
		set(--x, y, ' ');
		return -1;
	}
	
	if(c == '\n') // New line
	{ 
		x = 0;
		if(++y == height)
			y = 0;
		return 1;
	}
	
	set(x, y, c);
	
	// New line when max width was reached
	if(++x == width)
	{
		x = 0;
		if(++y == height)
			y = 0;
	}
	return 1;
}

int
terminal::print(const char* str)
{
	int i = 0;
	while (str[i] != 0)
	{
		i += print(str[i]);
	}
	return i;
}
