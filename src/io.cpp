#include <stdarg.h>
#include <amsos/terminal.h>
#include <amsos/library/string.h>
#include <amsos/library/io.h>

int chk_precision(const char* cc)
{
	if(!strncmp(cc, "hh", 2)) // hh
		return 0;
	if(*cc == 'h') // h
		return 1;
	if(*cc == 'l')
		return 3;
	// TODO : add more precision types
	return 2;
}

int printf(const char* f, ...)
{
	const char* cc = f;
	int len = 0;
	
	va_list vl;
	va_start(vl, f);
	while(*cc)
	{
		if(*cc == '%')
		{
			++cc; // move to next character
			
			int p = chk_precision(cc);
			if(p != 2) ++cc;
			if(p == 0 || p == 4) ++cc; // hh and ll use 2 characters
			
			if(*cc == 'c')
			{
				int a = va_arg(vl, int);
				len += term_printc(a);
			}
			if(*cc == 's')
			{
				const char* a = va_arg(vl, const char*);
				len += term_prints(a);
			}
			if(*cc == '%')
			{
				len += term_printc('%');
			}
			if(*cc == 'C')
			{
				uint32_t a = va_arg(vl, uint32_t);
				term_set_color(a);
			}
		}else 
			len += term_printc(*cc);
		++cc; // next
	}
	va_end(vl);
	return len;
}