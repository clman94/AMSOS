#include <stdint.h>
struct suint32_t
{
	union
	{
		uint32_t n32;
		uint16_t n16[2];
		uint8_t  n8[4];
		bool b[32];
	};
} __attribute__ ((packed));

struct suint16_t
{
	union
	{
		uint16_t n16;
		uint8_t  n8[2];
		bool b[16];
	};
} __attribute__ ((packed));
