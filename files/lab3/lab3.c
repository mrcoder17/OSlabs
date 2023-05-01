#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

#define PAGE_SIZE 4096
#define PAGEMAP_LENGTH 8

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("argc err");
        exit(1);
    }

    char maps_path [256];
    char pagemap_path[256];
    sprintf(pagemap_path, "/proc/%s/pagemap", argv[1]);
    sprintf(maps_path, "/proc/%s/maps", argv[1]);


    int pagemap_fd = open(pagemap_path, O_RDONLY);
    if (pagemap_fd == -1) {
        perror("open err");
        exit(1);
    }

    FILE *maps_file = fopen(maps_path, "r");
    if (!maps_file) {
        perror("fopen err");
        exit(1);
    }

    uint64_t start_addr;
    uint64_t end_addr;
    uint64_t offset;
    uint64_t page_frame_number;
    uint64_t page_flags;
    char buf[256];

    while (fgets(buf, sizeof(buf), maps_file)) {
        sscanf(buf, "%lx - %lx", &start_addr, &end_addr);
        offset = start_addr / PAGE_SIZE * PAGEMAP_LENGTH;

        if (lseek(pagemap_fd, offset, SEEK_SET) == -1) {
            perror("lseek err");
            exit(1);
        }

        if (read(pagemap_fd, &page_frame_number, PAGEMAP_LENGTH) != PAGEMAP_LENGTH) {
            perror("read pagemap err");
            exit(1);
        }

        page_frame_number = page_frame_number & 0x7fffffffffffff;
        page_flags = page_frame_number >> 55;
//        printf("start-end addr: 0x%lx - 0x%lx, // ", start_addr, end_addr );
        printf("frame num: %lu flags: %lu\n", page_frame_number, page_flags);
    }

    close(pagemap_fd);
    fclose(maps_file);
    return 0;
}
