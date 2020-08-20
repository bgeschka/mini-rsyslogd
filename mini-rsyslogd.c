#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>

static int get_listen_fd(int port, const char *listen_addr)
{
	static int srvsock = -1;
	if(srvsock != -1) return srvsock;

	srvsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	int t = 1;
	setsockopt(srvsock, SOL_SOCKET, SO_REUSEADDR, &t, sizeof(int));

	struct sockaddr_in sa;
	sa.sin_port = htons(port);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(listen_addr);
	bind(srvsock, (struct sockaddr *)&sa, sizeof(sa));
	listen(srvsock, 5);

	return srvsock;
}

void usage(const char *msg)
{
	fprintf(stderr, "%s\n"
			"\t mini-rsyslogd <port> <listenaddr:0.0.0.0>\n"
			"\t -- only does tcp -- \n"
			, msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	if(!argv[1]) usage("missing port");
	if(!argv[2]) usage("missing listenaddr");

	printf("starting\n");
	int listen_fd = get_listen_fd(atoi(argv[1]), argv[2]);
	struct sockaddr_in cl;
	socklen_t slen = sizeof(cl);
	int clientsock = 0;
	printf("listening...\n");
	while ((clientsock = accept(listen_fd, (struct sockaddr *)&cl, &slen)) != 0) {
		if( fork() != 0) continue;

		char buf[BUFSIZ];
		while (1) {
			int nread = read(clientsock, buf, BUFSIZ-1);
			if(nread < -1) exit(0);
			syslog(LOG_INFO, "%s", buf);
			memset(buf, 0, BUFSIZ-1);
		}
	}

	return 0;
}
