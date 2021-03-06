#define _XOPEN_SOURCE
#include <stdio.h>
#include <string.h>
// #include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
// #include <sys/types.h>
#include <sys/select.h>
#include <pty.h>
#include <termios.h>

#if 0
int openpty(int *amaster, int *aslave, char *name,
		struct termios *termp, struct winsize *winp)
{
	char *slave = NULL;
	int mfd = -1, sfd = -1;
	
	if (amaster)
		*amaster = -1;
	if (aslave)
		*aslave = -1;
	// mfd = open("/dev/ptmx", O_RDWR | O_NOCTTY);
	mfd = posix_openpt(O_RDWR | O_NOCTTY);
	if (mfd == -1)
		goto err;
	if (grantpt(mfd) == -1)
		goto err;
	if (unlockpt(mfd) == -1)
		goto err;
	if ((slave = ptsname(mfd)) == NULL)
		goto err;
	if (name)
		strcpy(name, slave);
	if ((sfd = open(slave, O_RDWR | O_NOCTTY)) == -1)
		goto err;

	/*if (ioctl(sfd, I_PUSH, "ptem") == -1 ||
			(termp != NULL && tcgetattr(sfd, termp) < 0))
		goto err; */
	if (amaster)
		*amaster = mfd;
	if (aslave)
		*aslave = sfd;
	if (winp)
		ioctl(sfd, TIOCSWINSZ, winp);
	return 0;
err:
	perror("openpty() - ");
	if (sfd != -1)
		close(sfd);
	close(mfd);
	return -1;
} 
#endif

int main(int argc, int argv[])
{
	int master, pty;
	int child, status;
	fd_set erset;
	fd_set rdset;
	char ptyname[128] = {0};
	char buf[512];
	int rr;
	char* args[] = { "/bin/bash", "-i", NULL};

	/*
	if (openpty(&master, &pty, ptyname, NULL, NULL) < 0) {
		perror("cannot open pty - ");
		return -1;
	} */

	child = forkpty(&master, NULL, NULL, NULL);
    struct termios tio_new;
	tcgetattr(0, &tio_new);
	// tio_new=tio_orig;
	tio_new.c_lflag &= ~(ECHO | ICANON); /* Clear ICANON and ECHO. */
	// tio_new.c_cc[VMIN] = 1;
	// tio_new.c_cc[VTIME] = 0;
	tcsetattr(master, TCSANOW,&tio_new);
	//tio_new.c_iflag = TTYDEF_IFLAG;
	//tio_new.c_oflag = TTYDEF_OFLAG;
	//tio_new.c_lflag = TTYDEF_LFLAG;
	tcsetattr(master, TCSAFLUSH, &tio_new);
// printf("tty: %s, pty: %d\n", ptyname, pty);
	// child = fork();
	if (child == 0) {
	
		/*
		close(master);
		ioctl(pty, TIOCSCTTY, 0);
		if (dup2(pty, STDIN_FILENO) == -1) {
			perror("attach pty failed - ");
			return -1;
		}
		
		if (dup2(pty, STDOUT_FILENO) == -1) {
			perror("attach pty failed - ");
			return -1;
		}

		if (dup2(pty, STDERR_FILENO) == -1) {
			perror("attach pty failed - ");
			return -1;
		}
		*/
		//system("readlink /proc/self/fd/0");
		//system("readlink /proc/self/fd/1");
		//system("readlink /proc/self/fd/2");
		
		execv("/bin/bash", args);
	} else {
		// close(pty);
		while(1) {
			FD_ZERO(&rdset);
			FD_SET(0, &rdset);
			FD_SET(master, &rdset);

			FD_ZERO(&erset);
			FD_SET(0, &erset);
			FD_SET(master, &erset);

			int n = select( master + 1, &rdset, NULL, &erset, NULL);
			if (n <= 0)
				break;
			if (FD_ISSET (master, &rdset)) {
				rr = read(master, buf, sizeof(buf));
				if (rr <= 0) {
					// perror("");
					break;
				}
				write(0, buf, rr);
				memset(buf, 0, sizeof(buf));
			}

			if (FD_ISSET(0, &rdset)) {
				rr = read(0, buf, sizeof(buf));
				if (rr <= 0) {
					// perror("");
					break;
				}
				write(master, buf, rr);
				memset(buf, 0, sizeof(buf));
			}

			if (FD_ISSET (master, &erset) || FD_ISSET(0, &erset)) {
				// printf("error");
				break;
			}

		// wait(&status);
		}
	}

	return 0;
}

