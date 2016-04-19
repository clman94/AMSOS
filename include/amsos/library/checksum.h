#ifndef LIBRARY_CHECKSUM_H
#define LIBRARY_CHECKSUM_H

#include <stdint.h>

template<typename T>
static inline
bool checksum(T* src)
{
	char sum = 0;
	for (int j = 0; j < sizeof(T); j++)
	{
		sum += ((char*)src)[j];
	}
	if (sum == 0)
		return true;
	return false;
}

template<typename T>
static inline
bool checksum_n(T* src, size_t n)
{
	char sum = 0;
	for (int j = 0; j < n; j++)
	{
		sum += ((char*)src)[j];
	}
	if (sum == 0)
		return true;
	return false;
}


#endif