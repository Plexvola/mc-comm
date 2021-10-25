#include "mctypes.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

varint writeVarInt(unsigned int x)
{
	varint n = malloc(sizeof(u_int8_t));
	unsigned int size = 1;
	while(1) {
		n = reallocarray(n, size, sizeof(u_int8_t));
		n[size-1] = x & 0x7F;
		x >>= 7;
		if(x == 0)
			return n;
		n[size-1] |= 0x80;
		size++;
	}
	return n;
}

int readVarInt(varint x)
{
	unsigned int offset = 0;
	unsigned int res = 0;
	do {
		if(offset == 5) exit(EXIT_FAILURE);
		res |= (x[offset] & 0b01111111) << offset*7;
		offset++;
	} while ((x[offset-1] & 0b10000000) != 0);
	return (int) res;
}
