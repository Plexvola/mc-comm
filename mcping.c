#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include "mctypes.h"
#include "mcnetwork.h"

int main(int argc, char *argv[])
{
	char *port_s, *address;
	unsigned short port;
	unsigned int protocol_ver, p_flag = 0;
	if (argc == 1) {
		printf
			("Usage: mcping ADDRESS[:PORT]\nPings a Minecraft server using the PING protocol.\n\n  -h\t\t\tdisplay this help and exit\n  -p PROTOCOL NUMBER\tUses specified protocol number.\n");
		exit(EXIT_SUCCESS);
	}
	int opt;
	while ((opt = getopt(argc, argv, "hp:")) != -1) {
		switch (opt) {
		case 'h':
			exit(EXIT_SUCCESS);
		case 'p':
			protocol_ver = atoi(optarg);
			p_flag = 1;
			break;
		default:
			break;
		}
	}
	address = argv[1];
	if ((port_s = strchr(address, ':')) != NULL) {
		address[strlen(address) - strlen(port_s)] = '\0';
		port_s++;
	} else
		port_s = "25565";
	if (p_flag == 0) {
		protocol_ver = 756;
	}
	port = atoi(port_s);

	int s;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror("bad socket\n");

	connect_mc_server(s, address, port);
	send_handshake(s, protocol_ver, address, port, 1);
	send_packet(s, 0x0, 0, NULL);	// request packet (0x0 with no fields)

	packet response = recv_packet(s);
#ifdef _DEBUG
	printf("length:\t%u\n", from_varint(response.length));
	printf("id:\t%d\n", from_varint(response.id));
	for (int i = 0; i < from_varint(response.length) - size_varint(response.id); ++i) {
		printf("%02X ", ((unsigned char*) response.data)[i]);
	}
	printf("\n");
#endif
	string json = deserialize_string(&response.data);

#ifdef _DEBUG
	printf("json length:\t%u\n", from_varint(json.length));
#endif

	for (int i = 0; i < from_varint(json.length); ++i) {
		printf("%c", json.content[i]);
	}
	printf("\n");

	close(s);
	return 0;
}
