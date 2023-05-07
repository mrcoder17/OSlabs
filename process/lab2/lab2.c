#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int global_var = 33;

int main() {
    int local_var = 44;

    printf("address of global: %p value: %d\n", &global_var, global_var);
    printf("address of local: %p value: %d\n", &local_var, local_var);

    printf("pid: %d\n", getpid());

    pid_t pid = fork();

    switch (pid) {
        case -1:
            perror("fork err");
            exit(1);
        case 0:
            printf("pid: %d ppid: %d\n", getpid(), getppid());

            printf("address of global in child: %p value: %d\n", &global_var, global_var);
            printf("address of local in child: %p value: %d\n", &local_var, local_var);

            global_var = 55;
            local_var = 66;

            printf("address of global in child after change: %p value: %d\n", &global_var, global_var);
            printf("address of local in child after change: %p value: %d\n", &local_var, local_var);

            exit(5);
        default:
            printf("address of global in parent: %p value: %d\n", (void*)&global_var, global_var);
            printf("address of local in parent: %p value: %d\n", (void*)&local_var, local_var);

            sleep(60);
    }


    return 0;
}