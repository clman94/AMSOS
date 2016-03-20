#ifndef LIBRARY_STRING_H
#define LIBRARY_STRING_H

#include <stdint.h>
#include <stddef.h>

static inline
void* memset(void* ptr, char value, size_t num){
	for(size_t i = 0; i < num; i++){
		*((char*)ptr + i) = value;
	}
	return ptr;
}

static inline
size_t strlen(char* s)
{
	size_t i = 0;
	while(s[i]!= '\0')
	{
		++i;
	}
	return i;
}


static inline
char* strset(char* s, char c){
	int l = strlen(s);
	
	for(int i = 0; i < l; i++)
	{
		s[i] = c;
	}
	return s;
}

static inline
char* strncpy(char* s1, char* s2, size_t num)
{
	for(size_t i = 0; i < num; i++)
	{
		s1[i] = s2[i];
	}
	return s1;
}

static inline 
char* strcpy(char* s1, char* s2)
{
	size_t l  = 0;
	size_t l1 = strlen(s1);
	size_t l2 = strlen(s2);
	
	if (l1 < l2) l = l1;
	else         l = l2;
	
	strncpy(s1, s2, l);
	return s1;
}


#endif