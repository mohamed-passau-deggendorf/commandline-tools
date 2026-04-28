#define _GNU_SOURCE
#include <unistd.h>
#include <fcntl.h>
#include <sys/syscall.h>
#include <dirent.h>

int open_directory(const char *pathname) {
    return open(pathname, O_RDONLY | O_DIRECTORY);
}

int get_directory_entries(unsigned int fd, void *buffer, unsigned int size) {
    return syscall(SYS_getdents64, fd, buffer, size);
}


void close_directory(unsigned int fd) {close(fd);}
