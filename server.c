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
	int err;
	int conn;
	int bytes;
	struct sockaddr_un addr = {
		.sun_family = AF_UNIX,
		.sun_path = SOCKET_NAME,
	};

	unlink(SOCKET_NAME);
	conn = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if (conn == -1)
		exit(EXIT_FAILURE);

	err = bind(conn, (const struct sockaddr *)&addr, sizeof(addr));
	if (err)
		exit(EXIT_FAILURE);

	err = listen(conn, 20);
	if (err)
		exit(EXIT_FAILURE);

	for (;;) {
		int data_socket;
		int sum = 0;
		char buffer[] = "0123456789abcdef0123456789abcdef";

		data_socket = accept(conn, NULL, NULL);
		if (data_socket < 0)
			exit(EXIT_FAILURE);

		for(;;) {
			bytes = read(data_socket, buffer, sizeof(buffer));
			if (bytes < 0)
				exit(EXIT_FAILURE);

			if (!strncmp(buffer, "END", 3))
				break;

			sum += atoi(buffer);
		}

		sprintf(buffer, "%d", sum);
		bytes = write(data_socket, buffer, sizeof(buffer));
		if (bytes < 0)
			exit(EXIT_FAILURE);

		close(data_socket);
	}

	close(conn);
	unlink(SOCKET_NAME);
	exit(EXIT_SUCCESS);
}
