#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>

int main(int argc, char *argv[])
{
	char buf[1024];
	struct nlmsghdr *nlh = (struct nlmsghdr *)buf;
	int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USERSOCK);
	struct sockaddr_nl *da, sa = {
		.nl_family = AF_NETLINK,
		.nl_pid = getpid(),
	};
	struct iovec iov[] =  {
		{
			.iov_base = (void *)buf,
			.iov_len = sizeof(buf),
		},
	};
	struct msghdr msg = {
		.msg_iov = iov,
		.msg_iovlen = sizeof(iov)/sizeof(iov[0]),
	};

	printf("my pid=%d\n", sa.nl_pid);
	bind(fd, (struct sockaddr *)&sa, sizeof(sa));
	recvmsg(fd, &msg, 0);
	printf("Got: %s from pid=%d\n", NLMSG_DATA(buf), nlh->nlmsg_pid);

	da = &sa;
	da->nl_pid = nlh->nlmsg_pid;
	msg.msg_name = (void *)da;
	msg.msg_namelen = sizeof(*da);
	strcpy(NLMSG_DATA(buf), "Good-bye!");
	sendmsg(fd, &msg, 0);

	close(fd);
	exit(EXIT_SUCCESS);
}
