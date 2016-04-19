#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>

int main(int argc, char *argv[])
{
	/*           |0123456789abcdef| skip sizeof(nlh)==16 byte */
	char buf[] = "----------------Hello World!";
	int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USERSOCK);
	struct sockaddr_nl da = {
		.nl_family = AF_NETLINK,
		.nl_pid = atoi(argv[1]),
	};
	struct iovec iov = {
		.iov_base = (void *)buf,
		.iov_len = sizeof(buf),
	};
	struct msghdr msg = {
		.msg_name = (void *)&da,
		.msg_namelen = sizeof(da),
		.msg_iov = &iov,
		.msg_iovlen = 1,
	};
	sendmsg(fd, &msg, 0);
	exit(EXIT_SUCCESS);
}
