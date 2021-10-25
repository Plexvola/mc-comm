#include "mctypes.h"
#include <stdlib.h>
#include <string.h>

varint to_varint(unsigned int x)
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

int from_varint(varint x)
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

size_t size_varint(varint x)
{
	size_t v_size;
	for (v_size = 1; (x[v_size - 1] & 0x80) != 0; v_size++);
	return v_size;
}

size_t serialize_handshake(handshake hs, void **buf)
{
	size_t size = 0;
	size_t varint_s;
	*buf = malloc(size);

	varint_s = size_varint(hs.protocol_version);
	*buf = realloc(*buf, size+varint_s);
	memcpy(*buf + size, hs.protocol_version, varint_s);
	size += varint_s;

	varint_s = size_varint(hs.server_address.length);
	*buf = realloc(*buf, size+varint_s);
	memcpy(*buf + size, hs.server_address.length, varint_s);
	size += varint_s;

	*buf = realloc(*buf, size+from_varint(hs.server_address.length));
	memcpy(*buf + size, hs.server_address.content, from_varint(hs.server_address.length));
	size += from_varint(hs.server_address.length);

	*buf = realloc(*buf, size+sizeof(unsigned short));
	memcpy(*buf + size, &hs.server_port, sizeof(unsigned short));
	size += sizeof(unsigned short);

	varint_s = size_varint(hs.next_state);
	*buf = realloc(*buf, size+varint_s);
	memcpy(*buf + size, hs.next_state, varint_s);
	size += varint_s;

	return size;
}

size_t serialize_packet(packet p, void **buf)
{
	size_t v_size = size_varint(p.length);
	size_t p_size = from_varint(p.length) + v_size; // total size of packet
	size_t size = 0;

	*buf = malloc(p_size);
	memcpy(*buf, p.length, v_size);
	size += v_size;

	for (v_size = 1; (p.id[v_size - 1] & 0x80) != 0; v_size++);
	memcpy(*buf+size, p.id, v_size);
	size += v_size;

	memcpy(*buf+size, p.data, from_varint(p.length) - v_size);

	return p_size;
}

