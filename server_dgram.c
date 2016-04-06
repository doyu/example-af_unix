/*
 * File server.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_un src = { .sun_family = AF_UNIX, .sun_path = "/tmp/socket-server", };
	char buf[1024];

	sockfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	bind(sockfd, (struct sockaddr *)&src, sizeof(src));

	read(sockfd, buf, sizeof(buf));
	printf("buf=%s\n", buf);

	close(sockfd);

	unlink(src.sun_path);
	exit(EXIT_SUCCESS);
}
