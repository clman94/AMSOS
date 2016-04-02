#ifndef H_TERMINAL
#define H_TERMINAL

#include <stdint.h>
#include <amsos/ramfs.h>

void term_set_color(uint8_t c);
uint8_t term_get_color();
int term_printc(char c);
int term_prints(const char* s);
void term_clear();
void term_set_position(uint32_t x, uint32_t y);
int term_hex32(uint32_t num, int len = 9);
uint32_t term_get_line();
int term_init(ram_dir* root);

#endif