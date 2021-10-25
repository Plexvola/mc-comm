#include <string.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "mctypes.h"

int main(int argc, char *argv[])
{
	char *port_s, *address;
	unsigned short port;
	int protocol_ver;
	int p_flag = 0;
	if (argc == 1) {
		printf
			("Usage: mcping ADDRESS[:PORT]\nPings a Minecraft server using the PING protocol.\n\n  -h\tdisplay this help and exit\n  -p [PROTOCOL NUMBER]\tUses specified protocol number.");
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

	struct addrinfo *mc_info = malloc(sizeof(struct addrinfo));
	memset(mc_info, 0, sizeof(struct addrinfo));
	struct addrinfo hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	if (getaddrinfo(address, "80", &hints, &mc_info) != 0)
		perror("bad addrinfo\n");

	struct sockaddr_in mc_sock;
	mc_sock.sin_family = AF_INET;
	mc_sock.sin_port = htons(port);
	memcpy(&mc_sock.sin_addr, mc_info->ai_addr->sa_data + 2, mc_info->ai_addrlen);	// stupidity

	if (connect(s, (struct sockaddr *) &mc_sock, sizeof(mc_sock)) < 0)
		perror("connection failed\n");

	handshake hs = {
		writeVarInt(protocol_ver),
		{writeVarInt(strlen(address)), address},
		port,
		writeVarInt(1)
	};

	unsigned char *raw_hs;
	size_t hs_size = serializeHandshake(hs, &raw_hs);
	printf("%lu\n", hs_size);
	for (int i = 0; i < hs_size; ++i) {
		printf("%02X ", raw_hs[i]);
	}
	printf("\n");
	return 0;
}
