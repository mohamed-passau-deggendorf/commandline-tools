#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>

int open_directory(const char *pathname) {
    return open(pathname, O_RDONLY | O_DIRECTORY);
}

int get_directory_entries(unsigned int fd, void *buffer, unsigned int size) {
    return syscall(SYS_getdents64, fd, buffer, size);
}

void close_directory(unsigned int fd) {
    close(fd);
}

int openat_wrapper(int dirfd, const char *pathname, int flags) {
    return openat(dirfd, pathname, flags);
}

ssize_t readlinkat_wrapper(int dirfd, const char *pathname, char *buf, size_t bufsiz) {
    return readlinkat(dirfd, pathname, buf, bufsiz);
}

int getpid_wrapper(void) {
    return getpid();
}
