/*
 * File client.c
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "connection.h"

int main(int argc, char *argv[])
{
	int i;
	int ret;
	int soc;
	char buffer[BUFFER_SIZE];
	struct sockaddr_un addr = {
		.sun_family = AF_UNIX,
		.sun_path =  SOCKET_NAME,
	};

	/* Create local socket. */
	soc = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if (soc == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	ret = connect(soc, (const struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1) {
		fprintf(stderr, "The server is down.\n");
		exit(EXIT_FAILURE);
	}

	/* Send arguments. */
	for (i = 1; i < argc; ++i) {
		ret = write(soc, argv[i], strlen(argv[i]) + 1);
		if (ret == -1) {
			perror("write");
			break;
		}
	}

	/* Request result. */
	strcpy (buffer, "END");
	ret = write(soc, buffer, strlen(buffer) + 1);
	if (ret == -1) {
		perror("write");
		exit(EXIT_FAILURE);
	}

	/* Receive result. */
	ret = read(soc, buffer, BUFFER_SIZE);
	if (ret == -1) {
		perror("read");
		exit(EXIT_FAILURE);
	}

	printf("Result = %s\n", buffer);
	close(soc);
	exit(EXIT_SUCCESS);
}
