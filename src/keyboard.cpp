#include <stdint.h>

#include <amsos/ports.h>
#include <amsos/drivers/keyboard.h>

#define PS2_READWRITE 0x60

#define PRESSED_BEGIN 0x00
#define PRESSED_END   0x58
#define RELEASE_BEGIN 0x80
#define RELEASE_END   0xD8

const unsigned char scancode_set[] = 
	"\0" // none
	"\0" // escape
	"1234567890-=\b"
	"\tqwertyuiop[]"
	"\0" // enter
	"\0" // lcontrol
	"asdfghjkl;'`"
	"\0" // lshift
	"\\zxcvbnm,./"
	"\0" // rshift
	"*"
	"\0" // lalt
	" "
	"\0" // caps
	"\0\0\0\0\0\0\0\0\0\0" // F1-10
	"\0" // numlock
	"\0" // scrolllock
	"789-456+1230."
	"\0\0\0\0" // none
	"\0\0" // F11-12
	;

static bool record_status = false;
static bool key_status[PRESSED_END] = { false, };

static uint8_t key_buffer[128] = { '\0', };
static uint8_t buffer_limit = 1;

inline void push_buffer(uint8_t c)
{
	for(int i = 1; i < buffer_limit; i++)
	{
		key_buffer[i] = key_buffer[i - 1];
	}
	key_buffer[0] = c;
}

inline uint8_t pop_buffer()
{
	uint8_t first = key_buffer[0];
	
	for(int i = 0; i < (buffer_limit - 1); i++)
		key_buffer[i] = key_buffer[i - 1];
	
	key_buffer[buffer_limit - 1] = '\0';
	
	return first;
}

void keyboard_clear_buffer()
{
	for(int i = 0; i < 128; i++){
		key_buffer[i] = '\0';
	}
}

uint8_t keyboard_get_ascii(bool wait)
{
	uint8_t scancode = pop_buffer();
	
	while(wait && scancode == 0)
		scancode = pop_buffer();
	
	if(scancode <= PRESSED_END)
		return scancode_set[scancode];
	
	return 0;
}

void keyboard_enable_buffer()
{
	buffer_limit = 128;
}

void keyboard_disable_buffer()
{
	buffer_limit = 1;
}

void keyboard_enable_direct()
{
	record_status = true;
}
void keyboard_disable_direct()
{
	record_status = false;
}

bool keyboard_key_status(uint8_t scan)
{
	return key_status[scan];
}

inline void update_status(uint8_t scan)
{
	if (scan <= PRESSED_END)
		   key_status[scan] = true;
		   
	if (scan >= RELEASE_BEGIN &&
		scan <= RELEASE_END)
		   key_status[scan - RELEASE_BEGIN] = false;
}

void IRQH_keyboard_controller()
{
	uint8_t scancode = port_in_b(PS2_READWRITE);
	
	if(record_status)
		update_status(scancode);
	
	if(scancode <= PRESSED_END)
		push_buffer(scancode);
}