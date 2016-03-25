#include <include/drivers/keyboard.h>
#include <include/terminal.h>

extern "C"
void kern_main()
{	
	// do divide by 0 test
	//int pie = 1 - 1;
	//pie = 12 / pie;
	
	keyboard_enable_buffer();
	keyboard_enable_direct();
	
	while(true)
	{
		uint8_t key = keyboard_get_ascii(true);
		
		if(key != 0)
		{
			bool shift = keyboard_key_status(SCANCODE_LSHIFT);
			term_printc(shift ? (key - 'a' + 'A') : key);
		}
	}
}