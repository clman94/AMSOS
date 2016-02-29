#include "string.h"

void* memset(void* ptr, char value, size_t num){
	for(size_t i = 0; i < num; i++){
		*((char*)ptr + i) = value;
	}
	return ptr;
}

size_t strlen(char* s)
{
	size_t i = 0;
	while(s[i]!= '\0')
	{
		++i;
	}
	return i;
}


char* strset(char* s, char c){
	int l = strlen(s);
	
	for(int i = 0; i < l; i++)
	{
		s[i] = c;
	}
	return s;
}

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

char* strncpy(char* s1, char* s2, size_t num)
{
	for(size_t i = 0; i < num; i++)
	{
		s1[i] = s2[i];
	}
	return s1;
}