#include <time.h>
#include <stdio.h>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <errno.h>
#include <sys/select.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#ifdef WIN32
#define close(fd)			closesocket(fd)

int set_nonblock(int fd)
{
	int	iMode = 0;
	int iResult = ioctlsocket(fd, FIONBIO, &iMode);
	return iResult != NO_ERROR;
}

int is_processing(int fd, int ret)
{
	return WSAGetLastError() ==  WSAEWOULDBLOCK;
}

int inet_pton(int af, const char * src, void * dst)
{
	int len = sizeof(struct sockaddr);
	return WSAStringToAddress(src, af, NULL, dst, &len) != SOCKET_ERROR;
}

#else // *nix
int set_nonblock(int fd)
{
	int		flags;
	flags = fcntl(fd, F_GETFL, 0);
 	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int is_processing(int fd, int ret)
{
	return errno == EINPROGRESS;
}
#endif

int scan_main(int argc, char *argv[])
{
	clock_t	start_time, end_time;
	float costtime;
	struct timeval	timeout;
	fd_set wfds;
	int current, len, error, retval;
	int s;
	struct sockaddr_in scan;
	int ret;
	int begin_port = 1;
	int end_port = 1024;
	
	scan.sin_family = AF_INET;
	scan.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, argv[1], &scan.sin_addr);

	if (argc >= 3) {
		begin_port = atoi(argv[2]);
		if (argc >= 4)
			end_port = atoi(argv[3]);
		else
			end_port = begin_port;
	}

	start_time = clock();

	for (current = begin_port; current <= end_port; current ++) {

		scan.sin_port = htons(current);
		s = socket(AF_INET, SOCK_STREAM, 0);
		set_nonblock(s);

		if ((ret = connect(s, (struct sockaddr *)&scan, sizeof(struct sockaddr))) < 0) {
			if (!is_processing(s, ret)) {
				close(s);
				continue;
			}

			timeout.tv_sec = 2;
			timeout.tv_usec = 0;
			FD_ZERO(&wfds);
			FD_SET(s, &wfds);
			retval = select(s + 1, NULL, &wfds, NULL, &timeout);
			if (0 == retval || -1 == retval) {
				close(s);
				continue;
			}

			if (FD_ISSET(s, &wfds)) {
				len = sizeof(error);
				if (getsockopt(s, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
					continue;
				}

			} else {
				perror("\nFD_ISSET: ");
				continue;
			}

			if (error) {
				close(s);
				continue;

			} else {
				printf("port %d is opened!\n", current);
				close(s);
				continue;
			}
		} else if (ret == 0) {
			printf("port %d is opened!\n", current);
			close(s);
		}
	}

	end_time = clock();
	costtime = (float)(end_time - start_time) / 10000;
	printf("Cost time:%f second\n", costtime);
}

