
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define SEQFILE		"seqno"
#define MAXLINE		256

int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);
int lock_test(int fd, int type, off_t offset, int whence, off_t len);

#define read_lock(fd, offset, whence, len) \
		lock_reg(fd, F_SETLK, F_RDLCK, offset, whence, len)
#define readw_lock(fd, offset, whence, len) \
		lock_reg(fd, F_SETLKW, F_RDLCK, offset, whence, len)
#define write_lock(fd, offset, whence, len) \
		lock_reg(fd, F_SETLK, F_WRLCK, offset, whence, len)
#define writew_lock(fd, offset, whence, len) \
		lock_reg(fd, F_SETLKW, F_WRLCK, offset, whence, len)
#define un_lock(fd, offset, whence, len) \
		lock_reg(fd, F_SETLK, F_UNLCK, offset, whence, len)
#define is_read_lockable(fd, offset, whence, len) \
		!lock_test(fd, F_RDLCK, offset, whence, len)
#define is_write_lockable(fd, offset, whence, len) \
		!lock_test(fd, F_WRLCK, offset, whence, len)


int lock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;

	lock.l_type = type;
	lock.l_whence = whence;
	lock.l_start = offset;
	lock.l_len = len;

	fcntl(fd, cmd, &lock);
}

int lock_test(int fd, int type, off_t offset, int whence, off_t len)
{
	struct flock lock;

	lock.l_type = type;
	lock.l_whence = whence;
	lock.l_start = offset;
	lock.l_len = len;

	if (fcntl(fd, F_GETLK, &lock) == -1)
		return -1;
	if (lock.l_type == F_UNLCK)
		return 0;

	return lock.l_pid;
}

/**********************************************/
static void my_lock(int fd)
{
	writew_lock(fd, 0, SEEK_SET, 0);
}
static void my_unlock(int fd)
{
	un_lock(fd, 0, SEEK_SET, 0);
}

int main(int argc, char *argv[])
{
	int ret = 0;
	int fd = 0;
	int i;
	long seqno;
	char line[MAXLINE];

	fd = open(SEQFILE, O_RDWR);
	if (fd == -1)
	{
		fprintf(stderr, "%s failed to open : %s\n", SEQFILE, strerror(errno));
		exit(1);
	}

	for (i = 0; i < 20; i++)
	{
		memset(line, 0, sizeof(line));

		my_lock(fd);

		lseek(fd, 0, SEEK_SET);
		ret = read(fd, line, sizeof(line));
		line[ret] = '\0';

		sscanf(line, "%ld", &seqno);
		fprintf(stdout, "%s, pid = %ld, seq = %ld\n", argv[0], (long)getpid(), seqno);

		seqno++;
		snprintf(line, sizeof(line), "%ld\n", seqno);
		lseek(fd, 0, SEEK_SET);
		write(fd, line, strlen(line));

		my_unlock(fd);
	}

	return ret;
}



