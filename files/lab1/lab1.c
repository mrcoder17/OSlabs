#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <libgen.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 2048


char* reverse_string(char *str) {
    int len = strlen(str);
    char temp;
    char *dot = strrchr(str, '.');
    if (dot != NULL){
        len = (int) (dot - str);
    }

    for (int i = 0; i < len / 2; i++) {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
    return str;
}

char* get_reversed_dir(const char *dir_path) {
    char *reversed_dir_path = malloc(strlen(dir_path) + 1);
    char dir_name[strlen(dir_path) + 1];
    char base_name[strlen(dir_path) + 1];

    strcpy(dir_name, dir_path);
    strcpy(base_name, basename(dir_name));
    reverse_string(base_name);
    sprintf(reversed_dir_path, "%s/%s", dirname(dir_name), base_name);
    return reversed_dir_path;
}

void copy_regular_file(const char *src_path, const char *dest_path) {
    int src_fd = open(src_path, O_RDONLY);
    if (src_fd == -1) {
        perror("open to read err");
        exit(1);
    }
    off_t file_size = lseek(src_fd, 0, SEEK_END);
    if (file_size == -1) {
        perror("lseek err");
        exit(1);
    }

    int dest_fd = open(dest_path, O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (dest_fd == -1) {
        perror("open to write err");
        exit(1);
    }
    char buffer[BUFFER_SIZE];
    off_t bytes_read = 0;
    while (bytes_read < file_size) {
        off_t remaining_bytes = file_size - bytes_read;
        size_t bytes_to_read = (remaining_bytes < BUFFER_SIZE) ? remaining_bytes : BUFFER_SIZE;

        off_t offset = lseek(src_fd, -bytes_to_read, SEEK_CUR);
        if (offset == -1) {
            perror("lseek err");
            exit(1);
        }

        ssize_t bytes_read_now = read(src_fd, buffer, bytes_to_read);
        if (bytes_read_now == -1) {
            perror("read err");
            exit(1);
        }

        for (ssize_t i = bytes_read_now - 1; i >= 0; i--) {
            char ch = buffer[i];
            ssize_t bytes_written = write(dest_fd, &ch, 1);
            if (bytes_written == -1) {
                perror("write err");
                exit(1);
            }
        }
        bytes_read += bytes_read_now;
    }
}

void copy_reverse_dir(char *src_path, char *dest_path){
    DIR *dir = opendir(src_path);
    if (!dir) {
        perror("opendir");
        return;
    }
    struct dirent *entry;
    if (mkdir(dest_path, 0777) == -1) {
        perror("mkdir error\n");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        char sub_path[1024];
        char sub_dest_path[1024];
        sprintf(sub_path, "%s/%s", src_path, entry->d_name);
        sprintf(sub_dest_path, "%s/%s", dest_path, reverse_string(entry->d_name));

        struct stat file_info;
        if (stat(sub_path, &file_info) == -1) {
            perror("stat error");
            exit(1);
        }

        if (S_ISDIR(file_info.st_mode)) {
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                copy_reverse_dir(sub_path, sub_dest_path);
            }
        }
        else if (S_ISREG(file_info.st_mode)){
            copy_regular_file(sub_path, sub_dest_path);
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        perror("argv");
        exit(1);
    }
    copy_reverse_dir(argv[1], get_reversed_dir(argv[1]));
    return 0;
}
