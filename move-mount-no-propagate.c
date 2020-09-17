#define _GNU_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <linux/fs.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>
#include <fcntl.h>

int open_tree(int dirfd, const char *pathname, unsigned int flags) {
	return syscall(428, dirfd, pathname, flags);
}

int move_mount(int from_dirfd, const char *from_pathname, int to_dirfd,
	       const char *to_pathname, unsigned int flags) {
	return syscall(429, from_dirfd, from_pathname, to_dirfd, to_pathname, flags);
}

#define MOVE_MOUNT_NO_PROPAGATE 0x00000100

int main(int argc, char **argv)
{
	int fd, nsfd;

	if (argc != 4) {
		printf("usage: %s <path/to/mount/src> <path/to/namespace> <path/to/mount/dst>\n", argv[0]);
		printf("[creates \"cross-namespace\" bind-mount without propagation]\n");
		return 1;
	}

	fd = open_tree(AT_FDCWD, argv[1], OPEN_TREE_CLONE);
	if (fd == -1) {
		perror("open_tree");
		return 1;
	}

	nsfd = open(argv[2], O_RDONLY);
	if (nsfd == -1) {
		perror("open");
		close(fd);
		return 1;
	}

	printf("Mounting %s into mount namespace of %s at %s\n", argv[1], argv[2], argv[3]);

	if (setns(nsfd, CLONE_NEWNS)) {
		perror("setns");
		goto err;
	}
	close(nsfd);

	if (move_mount(fd, "", AT_FDCWD, argv[3], MOVE_MOUNT_F_EMPTY_PATH | MOVE_MOUNT_NO_PROPAGATE)) {
		perror("move_mount");
		goto err;
	}

	printf("Done\n");
	close(fd);

	return 0;
err:
	close(nsfd);
	close(fd);
	return 1;
}
