#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "mctypes.h"

int main(int argc, char *argv[])
{
	if (argc == 1) {
		printf("Usage: mcping [ADDRESS[:PORT]]\nPings a Minecraft server using the PING protocol.\n");
		exit(EXIT_SUCCESS);
	}
	int opt;
	while ((opt = getopt(argc, argv, "h")) != -1) {
		switch (opt) {
			case 'h':
				exit(EXIT_SUCCESS);
			default:
				break;
		}
	}
	varint n, m;
	m = n = writeVarInt(atoi(argv[1]));
	while((*n & 0b10000000) != 0) {
		printf("%02X ", *n);
		n++;
	}
	printf("%02X\n", *n);
	printf("%d\n", readVarInt(m));
	return 0;
}
