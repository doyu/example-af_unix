#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>

int main(int argc, char *argv[])
{
	char buf[1024];
	int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USERSOCK);
	struct sockaddr_nl sa = {
		.nl_family = AF_NETLINK,
		.nl_pid = getpid(),
	};
	struct iovec iov =  {
		.iov_base = (void *)buf,
		.iov_len = sizeof(buf),
	};
	struct msghdr msg = {
		.msg_iov = &iov,
		.msg_iovlen = 1,
	};
	printf("pid=%d\n", sa.nl_pid);
	bind(fd, (struct sockaddr *)&sa, sizeof(sa));
	recvmsg(fd, &msg, 0);
	printf("%s\n", NLMSG_DATA(buf));
	exit(EXIT_SUCCESS);
}
