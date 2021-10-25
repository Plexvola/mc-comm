#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "mctypes.h"
#include "mcnetwork.h"




int main(int argc, char *argv[])
{
	char *port_s, *address;
	unsigned short port;
	size_t json_size;
	int protocol_ver;
	int p_flag = 0;
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
	send_packet(s, 0x0, 0, NULL); // request packet (0x0 with no fields)

	// json_size = from_varint(recv_varint(s));
	// printf("%lu\n", json_size);
	close(s);
	return 0;
}
