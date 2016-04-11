#include <amsos/drivers/keyboard.h>
#include <amsos/drivers/serial.h>

#include <amsos/library/string.h>
#include <amsos/library/io.h>
#include <amsos/terminal.h>
#include <amsos/commandline.h>
#include <amsos/ramfs.h>

#define DEBUG(A) printf("[KERNEL] %s", A);
#define DEBUG_COMPLETE() printf("Complete\n");

int getchar()
{
	uint8_t key = keyboard_get_ascii(true);
	
	if(key){
		bool shift = keyboard_key_status(SCANCODE_LSHIFT) | 
		             keyboard_key_status(SCANCODE_RSHIFT);
		key = shift ? (key - 'a' + 'A') : key;
	}
	return key;
}

// A basic input function
void basic_input(char* str, int max = 256)
{
	int i = 0;
	int in = 0;
	while (i < max)
	{
		in = getchar();
		if (in == '\b') 
		{
			i -= (i>0?1:0);
			printf("\b");
			continue;
		}
		if (in <= 0) continue;
		if (in == '\n') break;
		str[i] = in;
		printf("%c", in);
		++i;
	}
	str[i] = '\0';
}

// Temporary (from begin.cpp)
ram_dir* get_root_dir();


void list_dir_contents(ram_dir* d)
{
	ram_entry* e = get_first_file(d);
	printf("T_NAME_\n");
	while (e)
	{
		if (e->type == RAM_FILE_TYPE_DIRECTORY)
			printf("/ %s\n", e->name);
		else if (e->type == RAM_FILE_TYPE_FILE)
			printf("  %s\n", e->name);
		else
			printf("* %s\n", e->name);
		e = get_next_file(d, e);
	}
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
	
	ram_dir* cd = get_root_dir();
	
	user_settings* user = nullptr;
	
	while(true)
	{
		if(user)
			printf("[%C%s%C] %s>", 0x06, user->name, 0x07, (cd->ref ? cd->ref->name : "/"));
		else
			printf("[%CKERNEL%C] %s>", 0x04, 0x07, (cd->ref ? cd->ref->name : "/"));
		
		basic_input(input);
		printf("\nExecuting : %s\n", input);
		if(!strcmp(input, "cd"))
		{
			printf("Path : "); 
			basic_input(input);
			ram_dir* nd = get_dir_path(cd, input);
			if (nd == nullptr)
				printf("\nInvalid Path");
			else 
				cd = nd;
			
			printf("\n");
		}
		
		else if(!strcmp(input, "nu"))
		{
			ram_entry* userfile = create_file(get_root_dir(), "user", sizeof(user_settings));
			
			user = (user_settings*)userfile->range[0];
			
			printf("\nName : "); 
			basic_input(user->name, 16);
			
			printf("\n");
		}
		
		else if(!strcmp(input, "l"))
			list_dir_contents(cd);
		
		else if(!strcmp(input, "clr"))
			term_clear();
	}
}