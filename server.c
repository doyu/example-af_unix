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
	int ret;
	int conn;
	struct sockaddr_un addr = {
		.sun_family = AF_UNIX,
		.sun_path = SOCKET_NAME,
	};

	/*
	 * In case the program exited inadvertently on the last run,
	 * remove the socket.
	 */
	unlink(SOCKET_NAME);

	/* Create local socket. */
	conn = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if (conn == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	ret = bind(conn, (const struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	/*
	 * Prepare for accepting connections. The backlog size is set
	 * to 20. So while one request is being processed other requests
	 * can be waiting.
	 */
	ret = listen(conn, 20);
	if (ret == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	/* This is the main loop for handling connections. */
	for (;;) {
		int data_socket;
		char buffer[BUFFER_SIZE];
		int sum = 0;

		/* Wait for incoming connection. */
		data_socket = accept(conn, NULL, NULL);
		if (ret == -1) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		for(;;) {
			/* Wait for next data packet. */
			ret = read(data_socket, buffer, sizeof(buffer));
			if (ret == -1) {
				perror("read");
				exit(EXIT_FAILURE);
			}

			if (!strncmp(buffer, "END", 3))
				break;

			/* Add received summand. */
			sum += atoi(buffer);
		}

		sprintf(buffer, "%d", sum);
		ret = write(data_socket, buffer, sizeof(buffer));
		if (ret == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}

		close(data_socket);
	}

	close(conn);
	unlink(SOCKET_NAME);
	exit(EXIT_SUCCESS);
}
