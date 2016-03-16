#include <stdint.h>

const int 	term_width = 80;
const int 	term_height = 25;
int 		term_x = 0;
int 		term_y = 0;

unsigned char 		term_color = 0x07;
unsigned char* 		term_buffer = (unsigned char*)0xb8000;

// Set a specific cahracter
void term_set(int x,int y, char c){
	int loc = (x + (y*term_width))*2;
	term_buffer[loc] = c;
	term_buffer[loc + 1] = term_color;
}

// Clears Terminal
void term_clear()
{
	// Set all characters to spaces
	for(int x = 0; x < term_width; x++){
		for(int y = 0; y < term_height; y++){
			term_set(x, y, ' ');
		}
	}
	
	// Reset cursor
	term_x = 0;
	term_y = 0;
}

// Prints a character
void term_printc(char c)
{
	if(c == '\n'){ // New line
		term_x = 0;
		term_y++;
		return;
	}else{
		term_set(term_x, term_y, c);
	}
	
	// New line when max width was reached
	if(++term_x == term_width){
		term_x = 0;
		if(++term_y == term_height){
			term_y = 0;
		}
	}
}

// Prints a string
void term_prints(const char* s){
	
	int i = 0;
	while(s[i] != 0){
		term_printc(s[i]);
		i++;
	}
}

// Prints a string with a temperary color
void term_prints(const char* s, unsigned char color){
	unsigned int tmpcolor = term_color;
	term_color = color;
	int i = 0;
	while(s[i] != 0){
		term_printc(s[i]);
		i++;
	}
	term_color = tmpcolor;
}
