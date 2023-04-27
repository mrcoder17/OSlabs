#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
    if(argc == 1){
        printf("nothing to cat\n");
        exit(1);
    }

    pid_t my_id = fork();
    switch (my_id) {
        case -1:
            exit(1);
        case 0:
            execl("/bin/cat", "cat", argv[1], (char*) 0);
            perror("execl failed");
            exit(1);
        default:
	    printf ("\nwait child process...\n");
            wait(NULL);
            printf("\nend of waiting\n");
            exit(0);
    }

    return 0;
}
