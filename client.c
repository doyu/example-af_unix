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
	int err;
	int bytes;
	int soc;
	char buffer[] = "0123456789abcdef0123456789abcdef";
	struct sockaddr_un addr = {
		.sun_family = AF_UNIX,
		.sun_path =  SOCKET_NAME,
	};

	soc = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	if (soc == -1)
		exit(EXIT_FAILURE);

	err = connect(soc, (const struct sockaddr *)&addr, sizeof(addr));
	if (err)
		exit(EXIT_FAILURE);

	for (i = 1; i < argc; ++i) {
		bytes = write(soc, argv[i], strlen(argv[i]) + 1);
		if (bytes < 0)
			break;
	}

	bytes = write(soc, "END", 3);
	if (bytes < 0)
		exit(EXIT_FAILURE);

	bytes = read(soc, buffer, sizeof(buffer));
	if (bytes < 0)
		exit(EXIT_FAILURE);

	printf("Result = %s\n", buffer);
	close(soc);
	exit(EXIT_SUCCESS);
}
