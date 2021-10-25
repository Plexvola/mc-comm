#include "mctypes.h"
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

varint writeVarInt(unsigned int x)
{
	varint n = malloc(sizeof(u_int8_t));
	unsigned int size = 1;
	while (1) {
		n = reallocarray(n, size, sizeof(u_int8_t));
		n[size - 1] = x & 0x7F;
		x >>= 7;
		if (x == 0)
			return n;
		n[size - 1] |= 0x80;
		size++;
	}
	return n;
}

int readVarInt(varint x)
{
	unsigned int offset = 0;
	unsigned int res = 0;
	do {
		if (offset == 5)
			exit(EXIT_FAILURE);
		res |= (x[offset] & 0b01111111) << offset * 7;
		offset++;
	} while ((x[offset-1] & 0b10000000) != 0);
	return (int) res;
}

size_t serializeHandshake(handshake hs, void **buf)
{
	size_t size = 0;
	size_t vi_size;
	*buf = malloc(size);

	for (vi_size = 1; (hs.protocol_version[vi_size - 1] & 0x80) != 0; vi_size++);
	*buf = realloc(*buf, size+vi_size);
	memcpy(*buf + size, hs.protocol_version, vi_size);
	size += vi_size;

	for (vi_size = 1; (hs.server_address.length[vi_size - 1] & 0x80) != 0; vi_size++);
	*buf = realloc(*buf, size+vi_size);
	memcpy(*buf + size, hs.server_address.length, vi_size);
	size += vi_size;

	*buf = realloc(*buf, size+readVarInt(hs.server_address.length));
	memcpy(*buf + size, hs.server_address.content, readVarInt(hs.server_address.length));
	size += readVarInt(hs.server_address.length);

	*buf = realloc(*buf, size+sizeof(unsigned short));
	memcpy(*buf + size, &hs.server_port, sizeof(unsigned short));
	size += sizeof(unsigned short);

	for (vi_size = 1; (hs.next_state[vi_size - 1] & 0x80) != 0; vi_size++);
	*buf = realloc(*buf, size+vi_size);
	memcpy(*buf + size, hs.next_state, vi_size);
	size += vi_size;

	return size;
}
