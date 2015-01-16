#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>

int openpty(int *amaster, int *aslave, char *name,
		struct termios *termp, struct winsize *winp)
{
	const char *slave;
	int mfd = -1, sfd = -1;
	if (amaster)
		*amaster = -1;
	if (aslave)
		*aslave = -1;
	mfd = open("/dev/ptmx", O_RDWR | O_NOCTTY);
	if (mfd < 0)
		goto err;
	if (grantpt(mfd) == -1 || unlockpt(mfd) == -1)
		goto err;
	if ((slave = ptsname(mfd)) == NULL)
		goto err;
	if (name)
		strcpy(name, slave);
	if ((sfd = open(slave, O_RDONLY | O_NOCTTY)) == -1)
		goto err;

	/*if (ioctl(sfd, I_PUSH, "ptem") == -1 ||
			(termp != NULL && tcgetattr(sfd, termp) < 0))
		goto err;*/
	if (amaster)
		*amaster = mfd;
	if (aslave)
		*aslave = sfd;
	if (winp)
		ioctl(sfd, TIOCSWINSZ, winp);
	return 0;
err:
	perror('openpty() - ');
	if (sfd != -1)
		close(sfd);
	close(mfd);
	return -1;
} 

int main(int argc, int argv[])
{
	int pty;
	int child, status;
	char ptyname[128];
	pty = openpty(NULL, NULL, ptyname, NULL, NULL);
	if (pty < 0) {
		perror("cannot open pty - ");
		return -1;
	}

	printf("tty: %s\n", ptyname);

	if (dup2(0, pty)) {
		perror("attach pty failed - ");
		return -1;
	}
	
	if (dup2(1, pty)) {
		perror("attach pty failed - ");
		return -1;
	}

	if (dup2(2, pty)) {
		perror("attach pty failed - ");
		return -1;
	}

	if ((child = fork()) == 0) {
		execv("/bin/bash", NULL);
	} else
		wait(&status);

	return 0;
}

