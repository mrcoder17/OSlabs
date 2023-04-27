#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("one arg\n");
        exit(1);
    }

    char* command = argv[1];
    pid_t cpid;
    int return_value = 0;

    switch (cpid = fork()) {
        case -1:
            perror("fork error");
            exit(1);

        case 0:
            if (system(command) == -1){
                perror("system() error");
                exit(1);
            }
            exit(0);

        default:
            printf ("\n wait child process...\n");
            if (wait(&return_value) == -1) {
                perror("wait");
                exit(1);
            }
            printf("\n end of waiting\n");
            if(WIFEXITED(return_value)){
                printf("child's exit status is: %d\n", WEXITSTATUS(return_value));
            }else if (WIFSIGNALED(return_value)){
                printf("signal is: %d\n", WTERMSIG(return_value));
       	    }
            exit(0);
    }
    return 0;

}
