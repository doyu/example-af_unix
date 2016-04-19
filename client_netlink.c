#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>

int main(int argc, char *argv[])
{
	/*               |0123456789abcdef| skip sizeof(nlmh)==16 byte */
	char buf[1024] = "----------------Hello World!";
	struct nlmsghdr *nlh = (struct nlmsghdr *)buf;
	int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USERSOCK);
	struct sockaddr_nl *sa, da = {
		.nl_family = AF_NETLINK,
		.nl_pid = atoi(argv[1]),
	};
	struct iovec iov[] = {
		{
			.iov_base = (void *)buf,
			.iov_len = sizeof(buf),
		},
	};
	struct msghdr msg = {
		.msg_name = (void *)&da,
		.msg_namelen = sizeof(da),
		.msg_iov = iov,
		.msg_iovlen = sizeof(iov)/sizeof(iov[0]),
	};

	nlh->nlmsg_pid = getpid();
	nlh->nlmsg_len = sizeof(buf);
	sendmsg(fd, &msg, 0);
	printf("Sent from my pid=%d\n", nlh->nlmsg_pid);

	recvmsg(fd, &msg, 0);
	sa = msg.msg_name;
	printf("Got: %s from pid=%d\n", NLMSG_DATA(buf), sa->nl_pid);

	close(fd);
	exit(EXIT_SUCCESS);
}
