/*
 * File server.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "connection.h"

int main(int argc, char *argv[])
{
	int conn;
	struct sockaddr_un addr = {
		.sun_family = AF_UNIX,
		.sun_path = SOCKET_NAME,
	};

	unlink(SOCKET_NAME);
	conn = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	bind(conn, (const struct sockaddr *)&addr, sizeof(addr));
	listen(conn, 20);
	for (;;) {
		int data_socket;
		int sum = 0;
		char buffer[] = "0123456789abcdef0123456789abcdef";

		data_socket = accept(conn, NULL, NULL);
		for(;;) {
			read(data_socket, buffer, sizeof(buffer));
			if (!strncmp(buffer, "END", 3))
				break;

			sum += atoi(buffer);
		}

		sprintf(buffer, "%d", sum);
		write(data_socket, buffer, sizeof(buffer));
		close(data_socket);
	}

	close(conn);
	unlink(SOCKET_NAME);
	exit(EXIT_SUCCESS);
}
