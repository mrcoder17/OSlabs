#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>


void create_directory(const char *path) {
    if (mkdir(path, 0777) != 0) {
        perror("mkdir err");
        exit(1);
    }
}

void list_directory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir(path);
    if (dir == NULL) {
        perror("open dir err");
        exit(1);
    }

    printf("сontent of dir '%s':\n", path);
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

void delete_directory(const char *path) {
    if (rmdir(path) != 0) {
        perror("rmdir err");
        exit(1);
    }
}

void create_file(const char *path) {
    FILE *file = fopen(path, "w");
    if (!file) {
        perror("fopen err");
        exit(1);
    }
    fclose(file);
}

void read_file(const char *path) {
    FILE *file = fopen(path, "r");
    if (!file) {
        perror("fopen err");
        exit(1);
    }
    char ch;
    printf("contents of file '%s':\n", path);
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);
}

void delete_file(const char *path) {
    if (remove(path) != 0) {
        perror("remove err");
        exit(1);
    }
}

void create_symbolic_link(const char *target_path, const char *path) {
    if (symlink(target_path, path) != 0) {
        perror("symlink err");
        exit(1);
    }
}

void read_symbolic_link(const char *path) {
    char target_path[256];
    ssize_t length = readlink(path, target_path, sizeof(target_path) - 1);
    if (length == -1) {
        perror("readlink err");
        exit(1);
    }
}

void read_linked_file(const char *path) {
    char target_path[256];
    ssize_t length = readlink(path, target_path, sizeof(target_path) - 1);
    if (length == -1) {
        perror("readlink err");
        exit(1);
    }
    target_path[length] = '\0';

    FILE *file = fopen(target_path, "r");
    if (file == NULL) {
        perror("fopen err");
        exit(1);
    }
    char ch;
    printf("contents of linked file '%s':\n", target_path);
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }
    fclose(file);
}

void delete_symbolic_link(const char *path) {
    if (unlink(path) != 0) {
        perror("unlink err");
        exit(1);
    }
}

void create_hard_link(const char *target_path, const char *path) {
    if (link(target_path, path) != 0) {
        perror("link err");
        exit(1);
    }
}

void delete_hard_link(const char *path) {
    if (unlink(path) != 0) {
        perror("unlink err");
        exit(1);
    }
}

void print_file_permissions(const char *file_path) {
    struct stat file_stat;
    if (stat(file_path, &file_stat) != 0) {
        perror("stat err");
        exit(1);
    }
    printf("permissions of '%s':\n", file_path);
    printf("owner: %o\n", file_stat.st_mode & S_IRWXU);
    printf("group: %o\n", file_stat.st_mode & S_IRWXG);
    printf("others: %o\n", file_stat.st_mode & S_IRWXO);
}

void print_link_count(const char *file_path) {
    struct stat file_stat;
    if (stat(file_path, &file_stat) != 0) {
        perror("stat err");
        exit(1);
    }
    printf("link count of '%s': %lu\n", file_path, file_stat.st_nlink);
}

void change_file_permissions(const char *file_path, mode_t permissions) {
    if (chmod(file_path, permissions) != 0) {
        perror("chmod err");
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("./lab2 action path\n");
        exit(1);
    }

    char *action = argv[1];
    char *path = argv[2];

         if (strcmp(action, "create_directory") == 0) create_directory(path);
    else if (strcmp(action, "list_directory") == 0) list_directory(path);
    else if (strcmp(action, "delete_directory") == 0) delete_directory(path);
    else if (strcmp(action, "create_file") == 0) create_file(path);
    else if (strcmp(action, "read_file") == 0) read_file(path);
    else if (strcmp(action, "delete_file") == 0) delete_file(path);
    else if (strcmp(action, "create_symbolic_link") == 0) create_symbolic_link(path, argv[3]);
    else if (strcmp(action, "read_symbolic_link") == 0) read_symbolic_link(path);
    else if (strcmp(action, "read_linked_file") == 0) read_linked_file(path);
    else if (strcmp(action, "delete_symbolic_link") == 0) delete_symbolic_link(path);
    else if (strcmp(action, "create_hard_link") == 0) create_hard_link(path, argv[3]);
    else if (strcmp(action, "delete_hard_link") == 0) delete_hard_link(path);
    else if (strcmp(action, "print_file_permissions") == 0) print_file_permissions(path);
    else if (strcmp(action, "print_link_count") == 0) print_link_count(path);
    else if (strcmp(action, "change_file_permissions") == 0) {
        mode_t permissions = strtol(argv[3], NULL, 8);
        change_file_permissions(path, permissions);
    } else{
        printf("invalid action: %s\n", action);
        exit(1);
    }

    return 0;
}
