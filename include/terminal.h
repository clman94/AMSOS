#ifndef H_TERMINAL
#define H_TERMINAL

#include <stdint.h>

void term_set_color(unsigned char c);
void term_printc(char c);
void term_prints(const char* s);
void term_clear();
void term_set_position(uint32_t x, uint32_t y);


#endif