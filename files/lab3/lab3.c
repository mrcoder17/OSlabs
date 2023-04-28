#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define PAGE_SIZE 4096
#define PAGEMAP_LENGTH 8

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("argc err");
        exit(1);
    }

    char pagemap_path[256];
    sprintf(pagemap_path, "/proc/%s/pagemap", argv[1]);

    int fd = open(pagemap_path, O_RDONLY);
    if (fd == -1) {
        perror("open err");
        exit(1);
    }

    uint64_t pagemap_entry;
    ssize_t bytes_read;
    uint64_t page_frame_number;
    uint64_t page_flags;

    while ((bytes_read = read(fd, &pagemap_entry, PAGEMAP_LENGTH)) == PAGEMAP_LENGTH) {
        page_frame_number = pagemap_entry & 0x7fffffffffffff;
        page_flags = pagemap_entry >> 55;
        printf("frame num: %lu flags: %lu\n", page_frame_number, page_flags);
    }

    if (bytes_read == -1) {
        perror("read err");
        exit(1);
    }
    close(fd);
    return 0;
}
