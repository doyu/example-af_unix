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
	char buffer[] = "0123456789abcdef0123456789abcdef";
	int soc;
	struct sockaddr_un addr = {
		.sun_family = AF_UNIX,
		.sun_path =  SOCKET_NAME,
	};

	soc = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	connect(soc, (const struct sockaddr *)&addr, sizeof(addr));
	for (i = 1; i < argc; ++i)
		write(soc, argv[i], strlen(argv[i]) + 1);
	write(soc, "END", 3);
	read(soc, buffer, sizeof(buffer));
	printf("Result = %s\n", buffer);
	close(soc);
	exit(EXIT_SUCCESS);
}
