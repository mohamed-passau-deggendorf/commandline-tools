#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stddef.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char stat_buffer[255];

    if (stat(argv[1],stat_buffer) != 0) {
        perror("stat");
        return EXIT_FAILURE;
    }


    if ( (( (*((unsigned int*)(stat_buffer + 24))) & 0xF000) == 0x4000) ) {
        if (rmdir(argv[1]) != 0) {
            perror("rmdir");
            return EXIT_FAILURE;
        }

    } else {

        if (unlink(argv[1]) != 0) {
            perror("unlink");
            return EXIT_FAILURE;
        }

    }

    return EXIT_SUCCESS;
}
