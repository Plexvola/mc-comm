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
	char *data;;
} packet;


int writeVarInt(varint x);
varint writeVarInt(unsigned int x);
