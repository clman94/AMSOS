#include <amsos/drivers/keyboard.h>
#include <amsos/terminal.h>
#include <amsos/drivers/serial.h>

#define DEBUG(A) term_prints("[KERNEL] "); term_prints(A);
#define DEBUG_COMPLETE() term_prints("Complete\n");

char getchar()
{
	uint8_t key = keyboard_get_ascii(true);
	
	if(key){
		bool shift = keyboard_key_status(SCANCODE_LSHIFT);
		key = shift ? (key - 'a' + 'A') : key;
		term_printc(key);
	}
	return key;
}

// A basic input function
void basic_input(char* str, int max = 256)
{
	int i = 0;
	char in = 0;
	while (i < max)
	{
		in = getchar();
		if(in == '\n') break;
		str[i] = in;
		++i;
	}
	str[i + 1] = '\0';
}

extern "C"
void kern_main()
{	
	// divide by 0 test
	// unsigned int pie = 1 - 1;
	// pie = 12 / pie;
	
	keyboard_enable_buffer();
	keyboard_enable_direct();
	
	char input[256] = { '\0', };
	
	while(true)
	{
		basic_input(input);
		term_prints("piesadasd\n");
	}
}