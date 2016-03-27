#ifndef LIBRARY_STRING_H
#define LIBRARY_STRING_H

#include <stdint.h>
#include <stddef.h>

#define STRING_LENGTH_LIMIT 256

static inline
void* memcpy(void* destination, const void* source, size_t num)
{
	for(size_t i = 0; i < num; i++)
	{
		((char*)destination)[i] = ((char*)source)[i];
	}
	return destination;
}

static inline
void* memset(void* ptr, int value, size_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		*((char*)ptr + i) = (char)value;
	}
	return ptr;
}

static inline
size_t strlen(const char* s)
{
	size_t i = 0;
	while (s[i] != '\0')
	{
		++i;
	}
	return i;
}


static inline
char* strset(char* s, const char c){
	int l = strlen(s);
	
	for (int i = 0; i < l; i++)
	{
		s[i] = c;
	}
	return s;
}

static inline
char* strncpy(char* s1, const char* s2, size_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		s1[i] = s2[i];
	}
	return s1;
}

static inline 
char* strcpy(char* s1, const char* s2)
{
	size_t l  = 0;
	size_t l1 = strlen(s1);
	size_t l2 = strlen(s2);
	
	if (l1 < l2) l = l1;
	else         l = l2;
	
	strncpy(s1, s2, l);
	return s1;
}

static inline 
int strcmp(const char* s1, const char* s2)
{
	int i = 0;
	while (i < STRING_LENGTH_LIMIT)
	{
		if (s1[i] == 0 && // ||?
		    s2[i] == 0)
			break;
			
		if (s1[i] != s2[i])
			return s1[i] - s2[i];
	}
	return 0;
}


#endif