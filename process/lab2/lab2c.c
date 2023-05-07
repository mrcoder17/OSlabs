#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    printf("pid %d\n", getpid());
    printf("ppid %d\n", getppid());
    sleep(20);

    pid_t child_pid = fork();
    pid_t grandchild_pid;

    switch (child_pid) {
        case -1:
            perror("fork error");
            exit(1);
        case 0:
            grandchild_pid = fork();

            switch (grandchild_pid) {
                case -1:
                    perror("fork error");
                    exit(1);
                case 0:
                    printf("grandchild process %d is running\n", getpid());
                    sleep(35);
                    printf("grandchild process %d is exiting\n", getpid());
                    exit(0);
                default:
                    printf("child process %d is exiting and becoming a zombie\n", getpid());
                    sleep(5);
                    exit(0);
            }
        default:
            sleep(1);
            printf("Parent process (PID: %d) is waiting\n", getpid());
            sleep(10);
            printf("Parent process (PID: %d) is exiting\n", getpid());
    }

    sleep(60);

    return 0;
}