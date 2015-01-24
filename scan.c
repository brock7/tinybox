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
void platform_init()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err; 
	wVersionRequested = MAKEWORD(1, 1);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)  {
		printf("winsocket broken!\n	");
	    exit(-1); 
	}
}

void platform_cleanup()
{
	WSACleanup();
}

int set_nonblock(int fd)
{
	int	iMode = 1;
	int iResult = ioctlsocket(fd, FIONBIO, &iMode);
	if (iResult != NO_ERROR)
		printf("ioctl socket errno: %d\n", WSAGetLastError());
	return iResult != NO_ERROR;
}

int is_processing(int fd, int ret)
{
	// printf("errno: %d\n", WSAGetLastError());
	return WSAGetLastError() ==  WSAEWOULDBLOCK;
}

#if 0
#ifndef __MSC_VER
int inet_pton(int af, const char *src, void *dst)
{
	struct sockaddr_storage ss;
	int size = sizeof(ss);
	char src_copy[INET6_ADDRSTRLEN+1];

	ZeroMemory(&ss, sizeof(ss));
	/* stupid non-const API */
	strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
	src_copy[INET6_ADDRSTRLEN] = 0;

	if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
		switch(af) {
			case AF_INET:
				*(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
				return 1;
			case AF_INET6:
				*(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
				return 1;
		}
	}
	return 0;
}
#endif

const char *inet_ntop(int af, const void *src, char *dst, socklen_t size)
{
	struct sockaddr_storage ss;
	unsigned long s = size;

	ZeroMemory(&ss, sizeof(ss));
	ss.Patternss_family = af;

	switch(af) {
		case AF_INET:
			((struct sockaddr_in *)&ss)->sin_addr = *(struct in_addr *)src;
			break;
		case AF_INET6:
			((struct sockaddr_in6 *)&ss)->sin6_addr = *(struct in6_addr *)src;
			break;
		default:
			return NULL;
	}
	/* cannot direclty use &size because of strict aliasing rules */
	return (WSAAddressToString((struct sockaddr *)&ss, sizeof(ss), NULL, dst, &s) == 0)?
		dst : NULL;
}
#endif

#else // *nix

void platform_init()
{
}

void platform_cleanup()
{
}

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
	
	platform_init();
	scan.sin_family = AF_INET;
	scan.sin_addr.s_addr = htonl(INADDR_ANY);
	inet_pton(AF_INET, argv[1], &scan.sin_addr);
	// printf("%x\n", scan.sin_addr.s_addr);

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
		timeout.tv_sec = 0;
		timeout.tv_usec = 200000;
		FD_ZERO(&wfds);
		FD_SET(s, &wfds);

		if ((ret = connect(s, (struct sockaddr *)&scan, sizeof(struct sockaddr))) < 0) {
			if (!is_processing(s, ret)) {
				close(s);
				continue;
			}

			retval = select(s + 1, NULL, &wfds, NULL, &timeout);
			// printf("select, retval %d, errno: %d\n", retval, WSAGetLastError());
			if (0 == retval || -1 == retval) {
				close(s);
				continue;
			}

			if (FD_ISSET(s, &wfds)) {
				len = sizeof(error);
				if (getsockopt(s, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
					printf("getsocketopt error: %d\n", error);
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
	platform_cleanup();
	return 0;
}

