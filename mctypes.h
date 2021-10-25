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
	unsigned int server_port;
	varint next_state;
}__attribute__((packed)) handshake;


typedef long ping;
typedef long pong;

int readVarInt(varint x);
varint writeVarInt(unsigned int x);
