#include "mctypes.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

varint writeVarInt(unsigned int x)
{
	// unsigned int v = x;
	varint n = malloc(sizeof(u_int8_t));
	unsigned int size = 1;
	while(1) {
		n = reallocarray(n, size, sizeof(u_int8_t));
		n[size-1] = x & 0x7F;
		x >>= 7;
		if(x == 0)
			return n;
		n[size-1] |= 0b10000000;
		size++;
	}
	return n;
}
/*
writeVarInt(varint x, int s)
{
	while (1) {
		if ((x & 0xFFFFFF80) == 0) {
			if (send(s, &x, 8, 0) == -1)
				exit(EXIT_FAILURE);
			return;
		}
		if (send(s, &x, 8, 0) == -1)
			exit(EXIT_FAILURE);
		
		x >>= 7;
	}
} */
