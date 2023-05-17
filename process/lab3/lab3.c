#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <sched.h>
#include <fcntl.h>

#define STACK_SIZE (1024 * 1024)

void recursive_func(int counter) {
    char hello[] = "hello world";
    printf("%s\n", hello);
    if (counter < 10) {
        recursive_func(counter + 1);
    }
}

int child_func(void *arg) {
    recursive_func(1);
    return 0;
}

int main() {
    int fd = open("stackfile", O_CREAT | O_TRUNC | O_RDWR, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    if (ftruncate(fd, STACK_SIZE) == -1) {
        perror("ftruncate error");
        close(fd);
        return 1;
    }

    void *stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (stack == MAP_FAILED) {
        perror("mmap error");
        close(fd);
        return 1;
    }

    close(fd);

    pid_t child_pid = clone(child_func, stack + STACK_SIZE, CLONE_VM | SIGCHLD, NULL);
    if (child_pid == -1) {
        perror("clone error");
        munmap(stack, STACK_SIZE);
        return 1;
    }

    if (waitpid(child_pid, NULL, 0) == -1) {
        munmap(stack, STACK_SIZE);
        perror("waitpid error");
        return 1;
    }

    munmap(stack, STACK_SIZE);
    return 0;
}
