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
	int sockfd;
	struct sockaddr_un dst = { .sun_family = AF_UNIX, .sun_path = "/tmp/socket-server", };
	char buf[] = "0123456789abcdef0123456789abcdef";

	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	sendto(sockfd, buf, sizeof(buf), 0, (const struct sockaddr *)&dst, sizeof(dst));
	close(sockfd);

	unlink(dst.sun_path);
	exit(EXIT_SUCCESS);
}
