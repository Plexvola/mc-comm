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
	size_t p_size, v_size;
	unsigned char *raw_handshake, *raw_packet;
	varint packet_id;
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
		to_varint(protocol_ver),
		{to_varint(strlen(address)), address},
		port,
		to_varint(1)
	};

	size_t handshake_size = serialize_handshake(hs, (void**)&raw_handshake);
	// printf("%lu\n", hs_size);
	printf("      ");
	for (int i = 0; i < handshake_size; ++i) {
		printf("%02X ", raw_handshake[i]);
	}
	printf("\n");
	packet_id = to_varint(0x00);
	for (v_size = 1; (packet_id[v_size - 1] & 0x80) != 0; v_size++);

	packet handshake_packet = {
		to_varint(handshake_size + v_size),
		packet_id,
		(void*) raw_handshake
	};

	p_size = serialize_packet(handshake_packet, (void**)&raw_packet);
	for (int i = 0; i < p_size; ++i) {
		printf("%02X ", raw_packet[i]);
	}
	printf("\n");

	for (v_size = 1; (packet_id[v_size - 1] & 0x80) != 0; v_size++);
	packet request_packet = {
		to_varint(len_varint(to_varint(0x0))),
		to_varint(0x0),
		NULL
	};

	p_size = serialize_packet(request_packet, (void**)&raw_packet);

	return 0;
}
