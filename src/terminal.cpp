
#include <amsos/terminal.h>

//#include <amsos/library/endian.h>
#include <amsos/library/string.h>

// TODO : cleanup

static const int term_width  = 80;
static const int term_height = 25;
static uint32_t  term_x      = 0;
static uint32_t  term_y      = 0;

static uint8_t   term_color  = 0x07;
static uint8_t*  term_buffer = (uint8_t*)0xb8000; // default

void term_set_position(uint32_t x, uint32_t y)
{
	term_x = x;
	term_y = y;
}

uint32_t term_get_line()
{
	return term_y;
}

// Set a specific character
void term_set(uint32_t x,uint32_t y, char c)
{
	uint32_t loc = (x + (y*term_width))*2;
	term_buffer[loc] = c;
	term_buffer[loc + 1] = term_color;
}

void term_shift_up(int a)
{
	for (int i = 0; i < a; i++){
		for(uint32_t y = 0; y < term_height - 1; y++){
			uint32_t loc  = y*term_width*2;
			uint32_t down = (y + 1)*term_width*2;
			memcpy((void*)term_buffer[loc], (void*)term_buffer[down], term_width*2);
		}
	}
}

// Clears Terminal
void term_clear()
{
	// Set all characters to spaces
	for(uint32_t x = 0; x < term_width; x++){
		for(uint32_t y = 0; y < term_height; y++){
			term_set(x, y, ' ');
		}
	}
	
	// Reset cursor
	term_x = 0;
	term_y = 0;
}

void term_set_color(uint8_t c)
{
	term_color = c;
}

uint8_t term_get_color()
{
	return term_color;
}

// Prints a character
int term_printc(char c)
{
	if(c == '\b'){
		--term_x;
		term_set(term_x, term_y, ' ');
		return -1;
	}
	else if(c == '\n') // New line
	{ 
		term_x = 0;
		if(++term_y == term_height)
			term_y = 0;
		return 1;
	}else
		term_set(term_x, term_y, c);
	
	// New line when max width was reached
	if(++term_x == term_width)
	{
		term_x = 0;
		if(++term_y == term_height)
			term_y = 0;
	}
	return 1;
}

// Prints a string
int term_prints(const char* s)
{
	int i = 0;
	while(s[i] != 0)
	{
		term_printc(s[i]);
		i++;
	}
	return i;
}

int term_hex32(uint32_t num, int len)
{
	//bool l_endian = IS_L_ENDIAN();
	const char hex[] = "0123456789ABCDEF";
	char conv[9] = { '0', };
	for(int i = 0; i < (len <= 9 ? len : 9); i++)
	{
		conv[i] = hex[(num%16)];
		num /= 16;
	}
	
	char* out = "000000000\0";
	for(int i = 0; i < (len <= 9 ? len : 9); i++)
	{
		/*if(l_endian)
			out[8 - i] = conv[i];
		else*/
			out[i] = conv[i];
	}
	
	term_prints(out);
	return len;
}

int term_init(ram_dir* root)
{
	ram_entry* m = get_entry_path(root, "system/video/80X25");
	if(m == nullptr)
		return 1;
	
	term_buffer = (uint8_t*)m->range[0];
	
	return 0;
}
