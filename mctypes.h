#ifndef MCTYPES_H
#define MCTYPES_H

#include <sys/types.h>


typedef u_int8_t *varint;
typedef u_int8_t *varlong;

typedef struct {
	varint length;
	char *content;
} string;

typedef struct {
	varint length;
	varint id;
	void *data;
} packet;

typedef struct {
	varint protocol_version;
	string server_address;
	unsigned short server_port;
	varint next_state;
}__attribute__((packed)) handshake;


typedef long ping;
typedef long pong;

int from_varint(varint x);
varint to_varint(unsigned int x);
size_t size_varint(varint x);

size_t serialize_handshake(handshake hs, void **buf);
size_t serialize_packet(packet p, void **buf);

varint deserialize_varint(void **raw);
string deserialize_string(void **raw);

#endif // MCTYPES_H
